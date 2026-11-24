#define _XOPEN_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <crypt.h>

int main( int argc, char* argv[] ){

	if( argc != 3 ){
		printf("Usage: <2 char salt> <8 char key> \n");
		return -1;
	}

	struct crypt_data data;

	char* salt = argv[1];
	char* key = argv[2];
	char* result;

	result = crypt_r(key, salt, &data);
	if( result == NULL ){
		perror("Crypt() failed");
		return -1;
	}


	printf("Salt: %s\nPassword: %s\nResult: %s\n", salt, key, result);
		
	return 0;
}
