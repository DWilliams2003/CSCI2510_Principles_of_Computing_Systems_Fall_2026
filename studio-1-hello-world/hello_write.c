// Name: Dawson Williams
// Date: 2026-09-24
// Print a greeting using the write() system call.

#include <unistd.h>

int main(int argc, char* argv[]){
	write(STDOUT_FILENO, "Hello, world!\n", 14);
	return 0;
}
