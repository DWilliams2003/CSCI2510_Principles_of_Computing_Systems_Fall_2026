//Not the same as Studio-2 due to initial code not configured for Linux.
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    int input = open(argv[1], O_RDONLY);
    if (input == -1) {
        perror("Error opening file");
        return -1;
    }

    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(input, buffer, sizeof(buffer))) > 0) {
        ssize_t written = 0;
        while (written < bytes_read) {
            ssize_t result = write(STDOUT_FILENO, buffer + written,
                                   (size_t)(bytes_read - written));
            if (result == -1) {
                perror("Error writing to standard output");
                close(input);
                return 1;
            }
            if (result == 0) {
                fprintf(stderr, "Error writing to standard output: no bytes written\n");
                close(input);
                return 1;
            }
            written += result;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading file");
        close(input);
        return 1;
    }

    if (close(input) == -1) {
        perror("Error closing file");
        return 1;
    }

    return 0;
}