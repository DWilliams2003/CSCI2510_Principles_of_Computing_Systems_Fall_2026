// Dawson Williams
// Date: 2026-09-24
// Print a greeting using the write() system call.

#ifdef _WIN32
#include <io.h>
#define write _write
#define STDOUT_FILENO 1
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[]){
	static const char message[] = "Hello, world!\n";
	write(STDOUT_FILENO, message, sizeof(message) - 1);
	return 0;
}
