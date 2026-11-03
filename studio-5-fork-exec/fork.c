#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    char *child_argv[] = {"ls", "-l", NULL};
    char *child_prog = child_argv[0];
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (child_pid == 0) {
        sleep(3);
        execvp(child_prog, child_argv);
        perror("execvp");
        _exit(EXIT_FAILURE);
    } else {
        if (waitpid(child_pid, NULL, 0) < 0) {
            perror("waitpid");
            return EXIT_FAILURE;
        }
        printf("Parent process ID: %d\n", (int)getpid());
    }
    return 0;
}