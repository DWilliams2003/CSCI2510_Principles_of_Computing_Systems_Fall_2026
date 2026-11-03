#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	int fd[2];
	if (pipe(fd) == -1) {
		perror("pipe");
		return 1;
	}

	pid_t program_1_pid = fork();
	if (program_1_pid == -1) {
		perror("fork program_1");
		close(fd[0]);
		close(fd[1]);
		return 1;
	}

	if (program_1_pid == 0) {
		if (dup2(fd[1], STDOUT_FILENO) == -1) {
			perror("dup2 program_1 stdout");
			return 1;
		}
		close(fd[0]);
		close(fd[1]);

		char *arguments[] = {"program_1", NULL};
		execvp("./program_1", arguments);
		perror("execvp program_1");
		return 1;
	}

	pid_t program_2_pid = fork();
	if (program_2_pid == -1) {
		perror("fork program_2");
		close(fd[0]);
		close(fd[1]);
		waitpid(program_1_pid, NULL, 0);
		return 1;
	}

	if (program_2_pid == 0) {
		if (dup2(fd[0], STDIN_FILENO) == -1) {
			perror("dup2 program_2 stdin");
			return 1;
		}
		close(fd[0]);
		close(fd[1]);

		char *arguments[] = {"program_2", NULL};
		execvp("./program_2", arguments);
		perror("execvp program_2");
		return 1;
	}

	close(fd[0]);
	close(fd[1]);

	if (waitpid(program_1_pid, NULL, 0) == -1) {
		perror("waitpid program_1");
		return 1;
	}

	if (waitpid(program_2_pid, NULL, 0) == -1) {
		perror("waitpid program_2");
		return 1;
	}

	return 0;
}
