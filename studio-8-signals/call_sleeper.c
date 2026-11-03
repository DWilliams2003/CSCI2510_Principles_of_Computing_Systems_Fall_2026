//A simple program that fork()s and exec()s the ./sleep program

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_sigint(int signum)
{
	(void) signum;
	printf("Caller is ignoring SIGINT.\n");
}

int main(void)
{
	pid_t ret;

	if (signal(SIGINT, handle_sigint) == SIG_ERR) {
		perror("Could not register SIGINT handler");
		return EXIT_FAILURE;
	}

	printf("Forking sleeper...\n");

	ret = fork();
	if (ret == -1) {
		perror("Could not fork");
		return EXIT_FAILURE;
	}

	if (ret == 0) {
		int exec_ret;
		char* cmd = "./sleep";
		char* myargv[] = {"sleep", NULL};
		exec_ret = execvp( cmd, myargv );
		if (exec_ret == -1) {
			perror("Error calling execvp");
			_exit(EXIT_FAILURE);
		}
	}

	printf("Waiting for sleeper %d...\n", ret);
	while (waitpid(ret, NULL, 0) == -1) {
		if (errno != EINTR) {
			perror("Could not wait for sleeper");
			return EXIT_FAILURE;
		}
	}

	printf("Parent finished waiting and returned successfully!\n");
	return 0;
}
