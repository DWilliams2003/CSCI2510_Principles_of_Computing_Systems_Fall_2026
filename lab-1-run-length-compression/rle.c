//Runlength compression in C
// Usage: ./rle <input file> <output file> <runlength> <mode>
//        Where mode=0 is compress and mode=1 is decompress

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int write_all(int fd, const unsigned char *buffer, size_t length)
{
	size_t offset = 0;

	while (offset < length) {
		ssize_t written = write(fd, buffer + offset, length - offset);
		if (written < 0) {
			if (errno == EINTR) {
				continue;
			}
			return -1;
		}
		if (written == 0) {
			errno = EIO;
			return -1;
		}
		offset += (size_t)written;
	}
	return 0;
}

static int read_pattern(int fd, unsigned char *buffer, size_t capacity,
						size_t *length)
{
	size_t total = 0;

	while (total < capacity) {
		ssize_t bytes_read = read(fd, buffer + total, capacity - total);
		if (bytes_read < 0) {
			if (errno == EINTR) {
				continue;
			}
			return -1;
		}
		if (bytes_read == 0) {
			break;
		}
		total += (size_t)bytes_read;
	}

	*length = total;
	return total == 0 ? 0 : 1;
}

static int write_run(int fd, unsigned int count,
					 const unsigned char *pattern, size_t length)
{
	unsigned char counter = (unsigned char)count;

	if (write_all(fd, &counter, 1) < 0 ||
		write_all(fd, pattern, length) < 0) {
		return -1;
	}
	return 0;
}

static int compress_file(int input_fd, int output_fd, size_t pattern_size)
{
	unsigned char *current = malloc(pattern_size);
	unsigned char *next = malloc(pattern_size);
	size_t current_length;
	unsigned int count = 1;
	int status;

	if (current == NULL || next == NULL) {
		perror("malloc");
		free(current);
		free(next);
		return -1;
	}

	status = read_pattern(input_fd, current, pattern_size, &current_length);
	if (status < 0) {
		perror("read");
		free(current);
		free(next);
		return -1;
	}
	if (status == 0) {
		free(current);
		free(next);
		return 0;
	}

	for (;;) {
		size_t next_length;
		int next_status = read_pattern(input_fd, next, pattern_size,
									   &next_length);

		if (next_status < 0) {
			perror("read");
			free(current);
			free(next);
			return -1;
		}
		if (next_status == 0) {
			if (write_run(output_fd, count, current, current_length) < 0) {
				perror("write");
				free(current);
				free(next);
				return -1;
			}
			break;
		}

		if (next_length == current_length &&
			memcmp(next, current, current_length) == 0) {
			if (count == 255) {
				if (write_run(output_fd, count, current, current_length) < 0) {
					perror("write");
					free(current);
					free(next);
					return -1;
				}
				count = 1;
			} else {
				count++;
			}
		} else {
			if (write_run(output_fd, count, current, current_length) < 0) {
				perror("write");
				free(current);
				free(next);
				return -1;
			}
			{
				unsigned char *temporary = current;
				current = next;
				next = temporary;
			}
			current_length = next_length;
			count = 1;
		}
	}

	free(current);
	free(next);
	return 0;
}

static int decompress_file(int input_fd, int output_fd, size_t pattern_size)
{
	unsigned char *pattern = malloc(pattern_size);

	if (pattern == NULL) {
		perror("malloc");
		return -1;
	}

	for (;;) {
		unsigned char count;
		ssize_t bytes_read;
		size_t pattern_length;
		unsigned int repetition;
		int pattern_status;

		do {
			bytes_read = read(input_fd, &count, 1);
		} while (bytes_read < 0 && errno == EINTR);

		if (bytes_read < 0) {
			perror("read");
			free(pattern);
			return -1;
		}
		if (bytes_read == 0) {
			break;
		}
		if (count == 0) {
			fprintf(stderr, "Invalid input: run counter cannot be zero\n");
			free(pattern);
			return -1;
		}

		pattern_status = read_pattern(input_fd, pattern, pattern_size,
									  &pattern_length);
		if (pattern_status < 0) {
			perror("read");
			free(pattern);
			return -1;
		}
		if (pattern_status == 0) {
			fprintf(stderr, "Invalid input: missing run pattern\n");
			free(pattern);
			return -1;
		}

		for (repetition = 0; repetition < count; repetition++) {
			if (write_all(output_fd, pattern, pattern_length) < 0) {
				perror("write");
				free(pattern);
				return -1;
			}
		}
	}

	free(pattern);
	return 0;
}

static int parse_size(const char *text, size_t *value)
{
	char *end;
	unsigned long long parsed;

	if (text[0] == '-') {
		return -1;
	}
	errno = 0;
	parsed = strtoull(text, &end, 10);
	if (errno != 0 || end == text || *end != '\0' || parsed == 0 ||
		parsed > SIZE_MAX) {
		return -1;
	}
	*value = (size_t)parsed;
	return 0;
}

int main(int argc, char **argv)
{
	char *mode_end;
	long mode;
	size_t pattern_size;
	int input_fd;
	int output_fd;
	int status;

	if (argc != 5) {
		fprintf(stderr,
				"Usage: %s <input file> <output file> <compression length> <mode>\n",
				argv[0]);
		return EXIT_FAILURE;
	}
	if (parse_size(argv[3], &pattern_size) < 0) {
		fprintf(stderr, "Compression length must be a positive integer\n");
		return EXIT_FAILURE;
	}

	errno = 0;
	mode = strtol(argv[4], &mode_end, 10);
	if (errno != 0 || mode_end == argv[4] || *mode_end != '\0' ||
		(mode != 0 && mode != 1)) {
		fprintf(stderr, "Mode must be 0 (compress) or 1 (decompress)\n");
		return EXIT_FAILURE;
	}

	input_fd = open(argv[1], O_RDONLY);
	if (input_fd < 0) {
		perror("open input");
		return EXIT_FAILURE;
	}
	output_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,
					 S_IRUSR | S_IWUSR);
	if (output_fd < 0) {
		perror("open output");
		if (close(input_fd) < 0) {
			perror("close input");
		}
		return EXIT_FAILURE;
	}

	status = mode == 0 ? compress_file(input_fd, output_fd, pattern_size)
					   : decompress_file(input_fd, output_fd, pattern_size);

	if (close(input_fd) < 0) {
		perror("close input");
		status = -1;
	}
	if (close(output_fd) < 0) {
		perror("close output");
		status = -1;
	}

	return status < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
