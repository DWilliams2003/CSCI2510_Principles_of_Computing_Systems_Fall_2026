// SLUSH - the SLU shell.

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 4096
#define MAX_COMMANDS 128
#define MAX_ARGUMENTS 64

typedef struct {
    char *argv[MAX_ARGUMENTS + 1];
    size_t argc;
} Command;

static volatile sig_atomic_t interrupt_received = 0;

static void handle_interrupt(int signal_number)
{
    (void)signal_number;
    interrupt_received = 1;
}

static int parse_line(char *line, Command commands[], size_t *command_count)
{
    size_t current_command = 0;
    memset(commands, 0, MAX_COMMANDS * sizeof(*commands));

    char *cursor = line;
    while (*cursor != '\0') {
        while (isspace((unsigned char)*cursor)) {
            cursor++;
        }
        if (*cursor == '\0') {
            break;
        }

        if (*cursor == '(') {
            if (commands[current_command].argc == 0) {
                fprintf(stderr, "Invalid null command\n");
                return -1;
            }
            if (current_command + 1 >= MAX_COMMANDS) {
                fprintf(stderr, "Too many commands\n");
                return -1;
            }
            current_command++;
            cursor++;
            continue;
        }

        char *argument = cursor;
        while (*cursor != '\0' && !isspace((unsigned char)*cursor)
               && *cursor != '(') {
            cursor++;
        }

        int found_separator = (*cursor == '(');
        if (*cursor != '\0') {
            *cursor = '\0';
            cursor++;
        }

        Command *command = &commands[current_command];
        if (command->argc >= MAX_ARGUMENTS) {
            fprintf(stderr, "Too many arguments\n");
            return -1;
        }
        command->argv[command->argc++] = argument;

        if (found_separator) {
            if (current_command + 1 >= MAX_COMMANDS) {
                fprintf(stderr, "Too many commands\n");
                return -1;
            }
            current_command++;
        }
    }

    if (current_command > 0 && commands[current_command].argc == 0) {
        fprintf(stderr, "Invalid null command\n");
        return -1;
    }

    *command_count = current_command + 1;
    return commands[0].argc == 0 ? 0 : 1;
}

static int run_builtin(const Command *command)
{
    if (strcmp(command->argv[0], "cd") != 0) {
        return 0;
    }

    if (command->argc != 2) {
        fprintf(stderr, "cd: expected exactly one directory\n");
        return 1;
    }
    if (chdir(command->argv[1]) == -1) {
        perror("cd");
    }
    return 1;
}

static void wait_for_children(pid_t children[], size_t child_count)
{
    for (size_t i = 0; i < child_count; i++) {
        int status;
        while (waitpid(children[i], &status, 0) == -1) {
            if (errno == EINTR) {
                continue;
            }
            if (errno != ECHILD) {
                perror("waitpid");
            }
            break;
        }
    }
}

static int run_pipeline(Command commands[], size_t command_count)
{
    pid_t children[MAX_COMMANDS];
    size_t child_count = 0;
    int previous_read = STDIN_FILENO;

    for (size_t stage = 0; stage < command_count; stage++) {
        size_t command_index = command_count - stage - 1;
        int next_pipe[2] = {-1, -1};

        if (stage + 1 < command_count && pipe(next_pipe) == -1) {
            perror("pipe");
            if (previous_read != STDIN_FILENO) {
                close(previous_read);
            }
            wait_for_children(children, child_count);
            return -1;
        }

        pid_t child = fork();
        if (child == -1) {
            perror("fork");
            if (previous_read != STDIN_FILENO) {
                close(previous_read);
            }
            if (next_pipe[0] != -1) {
                close(next_pipe[0]);
                close(next_pipe[1]);
            }
            wait_for_children(children, child_count);
            return -1;
        }

        if (child == 0) {
            struct sigaction default_action;
            memset(&default_action, 0, sizeof(default_action));
            default_action.sa_handler = SIG_DFL;
            sigemptyset(&default_action.sa_mask);
            sigaction(SIGINT, &default_action, NULL);

            if (previous_read != STDIN_FILENO) {
                if (dup2(previous_read, STDIN_FILENO) == -1) {
                    perror("dup2");
                    _exit(1);
                }
            }
            if (next_pipe[1] != -1
                && dup2(next_pipe[1], STDOUT_FILENO) == -1) {
                perror("dup2");
                _exit(1);
            }

            if (previous_read != STDIN_FILENO) {
                close(previous_read);
            }
            if (next_pipe[0] != -1) {
                close(next_pipe[0]);
                close(next_pipe[1]);
            }

            execvp(commands[command_index].argv[0],
                   commands[command_index].argv);
            fprintf(stderr, "%s: Not found\n",
                    commands[command_index].argv[0]);
            _exit(127);
        }

        children[child_count++] = child;
        if (previous_read != STDIN_FILENO) {
            close(previous_read);
        }
        if (next_pipe[1] != -1) {
            close(next_pipe[1]);
            previous_read = next_pipe[0];
        }
    }

    if (previous_read != STDIN_FILENO) {
        close(previous_read);
    }
    wait_for_children(children, child_count);
    return 0;
}

static void discard_remaining_line(void)
{
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {
    }
}

int main(void)
{
    struct sigaction interrupt_action;
    memset(&interrupt_action, 0, sizeof(interrupt_action));
    interrupt_action.sa_handler = handle_interrupt;
    sigemptyset(&interrupt_action.sa_mask);
    if (sigaction(SIGINT, &interrupt_action, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    for (;;) {
        Command commands[MAX_COMMANDS];
        size_t command_count = 0;

        interrupt_received = 0;
        fputs("slush> ", stdout);
        fflush(stdout);

        errno = 0;
        if (fgets(line, sizeof(line), stdin) == NULL) {
            if (interrupt_received || errno == EINTR) {
                interrupt_received = 0;
                clearerr(stdin);
                fputc('\n', stdout);
                continue;
            }
            if (feof(stdin)) {
                return EXIT_SUCCESS;
            }
            perror("fgets");
            return EXIT_FAILURE;
        }

        if (interrupt_received) {
            interrupt_received = 0;
            fputc('\n', stdout);
            continue;
        }

        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            discard_remaining_line();
            fprintf(stderr, "Input line too long\n");
            continue;
        }

        int parse_result = parse_line(line, commands, &command_count);
        if (parse_result <= 0) {
            continue;
        }

        if (command_count == 1 && run_builtin(&commands[0])) {
            continue;
        }
        if (command_count > 1) {
            for (size_t i = 0; i < command_count; i++) {
                if (strcmp(commands[i].argv[0], "cd") == 0) {
                    fprintf(stderr, "cd: built-in commands must stand alone\n");
                    command_count = 0;
                    break;
                }
            }
            if (command_count == 0) {
                continue;
            }
        }

        run_pipeline(commands, command_count);
        interrupt_received = 0;
    }
}
