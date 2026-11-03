#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char input_string[1024];
    int max_args = 15;
    int max_argv_size = max_args + 2;
    char *cmd;
    char *my_argv[max_argv_size];

    printf("Enter input: ");
    fflush(stdout);

    if (fgets(input_string, sizeof(input_string), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    input_string[strcspn(input_string, "\n")] = '\0';

    cmd = strtok(input_string, " ");
    if (cmd == NULL) {
        return EXIT_SUCCESS;
    }

    my_argv[0] = cmd;
    int i = 1;
    char *token = strtok(NULL, " ");
    while (token != NULL && i <= max_args) {
        my_argv[i] = token;
        i++;
        token = strtok(NULL, " ");
    }

    if (token != NULL) {
        fprintf(stderr, "Too many arguments (maximum %d).\n", max_args);
        return EXIT_FAILURE;
    }

    my_argv[i] = NULL;
    execvp(cmd, my_argv);
    perror("execvp");
    return EXIT_FAILURE;
}
