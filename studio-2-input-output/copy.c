#ifdef _WIN32
#include <io.h>
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#define read _read
#define write _write
#else
#include <unistd.h>
#endif
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#define bufferSize 200

int main() {
    char buffer[bufferSize];

    while (1) {
        int bytesRead = read(STDIN_FILENO, buffer, bufferSize);

        if (bytesRead == 0) {
            break; // EOF reached
        }

        write(STDOUT_FILENO, buffer, bytesRead);
    }

    return 0;
}