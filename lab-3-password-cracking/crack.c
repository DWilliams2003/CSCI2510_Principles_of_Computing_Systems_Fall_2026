#define _XOPEN_SOURCE

#include <crypt.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int thread_id;
	int thread_count;
	int keysize;
	const char *target;
	const char *salt;
	int alphabet_size;
	int first_character;
} thread_args;

static pthread_mutex_t found_mutex = PTHREAD_MUTEX_INITIALIZER;
static int found_password = 0;

static uint64_t alphabet_power(int length, int alphabet_size)
{
	uint64_t result = 1;

	for (int i = 0; i < length; i++)
		result *= (uint64_t)alphabet_size;

	return result;
}

static void password_from_index(char *password, uint64_t index, int length,
		int alphabet_size, int first_character)
{
	for (int i = length - 1; i >= 0; i--) {
		password[i] = (char)(first_character + (index % (uint64_t)alphabet_size));
		index /= (uint64_t)alphabet_size;
	}
	password[length] = '\0';
}

static void *check_passwords(void *argument)
{
	thread_args *args = argument;
	uint64_t total = alphabet_power(args->keysize, args->alphabet_size);
	uint64_t part = total / (uint64_t)args->thread_count;
	uint64_t remainder = total % (uint64_t)args->thread_count;
	uint64_t start = (uint64_t)args->thread_id * part;
	uint64_t end;
	struct crypt_data data = {0};
	char password[9];

	if ((uint64_t)args->thread_id < remainder)
		start += (uint64_t)args->thread_id;
	else
		start += remainder;

	end = start + part;
	if ((uint64_t)args->thread_id < remainder)
		end++;

	for (uint64_t index = start; index < end; index++) {
		pthread_mutex_lock(&found_mutex);
		if (found_password) {
			pthread_mutex_unlock(&found_mutex);
			break;
		}
		pthread_mutex_unlock(&found_mutex);

		password_from_index(password, index, args->keysize,
				args->alphabet_size, args->first_character);
		char *result = crypt_r(password, args->salt, &data);
		if (result != NULL && strcmp(result, args->target) == 0) {
			pthread_mutex_lock(&found_mutex);
			if (!found_password) {
				found_password = 1;
				printf("%s\n", password);
				fflush(stdout);
			}
			pthread_mutex_unlock(&found_mutex);
			break;
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	if ((argc != 4 && argc != 5) || (argc == 5 && strcmp(argv[4], "--ascii") != 0)) {
		fprintf(stderr, "Usage: crack <threads> <keysize> <target> [--ascii]\n");
		return 1;
	}

	char *end;
	long thread_count = strtol(argv[1], &end, 10);
	if (*end != '\0' || thread_count < 1) {
		fprintf(stderr, "Usage: crack <threads> <keysize> <target> [--ascii]\n");
		return 1;
	}

	long keysize = strtol(argv[2], &end, 10);
	if (*end != '\0' || keysize < 1 || keysize > 8) {
		fprintf(stderr, "Usage: crack <threads> <keysize> <target> [--ascii]\n");
		return 1;
	}

	if (strlen(argv[3]) < 2) {
		fprintf(stderr, "Usage: crack <threads> <keysize> <target> [--ascii]\n");
		return 1;
	}

	int extended_charset = argc == 5;
	int alphabet_size = extended_charset ? 94 : 26;
	int first_character = extended_charset ? 33 : 'a';

	char salt[3];
	salt[0] = argv[3][0];
	salt[1] = argv[3][1];
	salt[2] = '\0';

	for (int length = 1; length <= keysize; length++) {
		pthread_t *threads = malloc((size_t)thread_count * sizeof(*threads));
		thread_args *args = malloc((size_t)thread_count * sizeof(*args));
		if (threads == NULL || args == NULL) {
			free(threads);
			free(args);
			fprintf(stderr, "Unable to allocate thread data\n");
			return 1;
		}

		for (int i = 0; i < thread_count; i++) {
			args[i].thread_id = i;
			args[i].thread_count = (int)thread_count;
			args[i].keysize = length;
			args[i].target = argv[3];
			args[i].salt = salt;
			args[i].alphabet_size = alphabet_size;
			args[i].first_character = first_character;

			if (pthread_create(&threads[i], NULL, check_passwords, &args[i]) != 0) {
				fprintf(stderr, "Unable to create thread\n");
				for (int j = 0; j < i; j++)
					pthread_join(threads[j], NULL);
				free(threads);
				free(args);
				return 1;
			}
		}

		for (int i = 0; i < thread_count; i++)
			pthread_join(threads[i], NULL);

		free(threads);
		free(args);

		pthread_mutex_lock(&found_mutex);
		int found = found_password;
		pthread_mutex_unlock(&found_mutex);
		if (found)
			return 0;
	}

	return 0;
}