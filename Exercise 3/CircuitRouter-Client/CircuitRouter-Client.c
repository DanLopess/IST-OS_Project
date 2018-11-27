/*
// Projeto SO - exercise 3, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
// AdvShell works as a client in a namedpipe
*/

#include "../lib/commandlinereader.h"
#include "../lib/vector.h"
#include "CircuitRouter-Client.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFSIZE 255

/* global variables */
int fshell, fclient;
char* shellPipeName;
char* pipeName;

void finishUp() {
	close(fclient);
	close(fshell);
	unlink(pipeName);
	free(pipeName);
	free(shellPipeName);
}

int main(int argc, char const *argv[]) {
	int fshell, fclient;
	char template[] = "ClientXXXXXX";
	char absolutePath[PATH_MAX + 1]; /* absolute path that is sent to advshell */

	shellPipeName = (char *)malloc(sizeof(char) * BUFFSIZE);
	pipeName = (char *)malloc(sizeof(char) * BUFFSIZE);

	if (shellPipeName == NULL) exit(EXIT_FAILURE); /* failed to allocate memory */
	if (pipeName == NULL) exit(EXIT_FAILURE); /* failed to allocate memory */
	if(argv[1] == NULL) exit(EXIT_FAILURE); /* argument must exist because it contains advshell pipename*/

	strcpy(shellPipeName, argv[1]); /* passed verification so we can assign it to shellPipeName */

	strcpy(pipeName, mktemp(template));

	if(strlen(pipeName) == 0) exit(EXIT_FAILURE); /* failed to generate pipeName*/

	unlink(pipeName);

	if (mkfifo(pipeName, 0777) < 0) { /* create own namedpipe */
		perror("Error creating own pipe");
		exit(EXIT_FAILURE);
	}

	if ((fshell = open(shellPipeName, O_WRONLY)) < 0) {
		perror("Failed to open shell pipe");
		exit(EXIT_FAILURE); /* error opening pipe */
	}

	if ((fclient = open(pipeName, O_RDONLY)) < 0) {
		perror("Failed to open own pipe");
		exit(EXIT_FAILURE); /* error opening pipe */
	}

	if(realpath(pipeName, absolutePath) == NULL) {
		perror("Failed to obtain absolute path");
		exit(EXIT_FAILURE);
	}

	while(1) {
		char command[BUFFSIZE]; /* stores command being sent to advshell */
		char message[BUFFSIZE]; /* stores response from advshell */
		int sentCommand = 0;

		if (fgets(command, BUFFSIZE, stdin) != NULL) /* read command and send to advshell*/
		{
			strcat(command, " ");  /* Makes it so that the first argument in the command is the return pipe*/
			strcat(command, absolutePath);
			if (write(fshell, command, BUFFSIZE < 0)) {
				perror("Failed to write to pipe");
				exit(EXIT_FAILURE);
			}
			sentCommand = 1;
		}
		if (sentCommand) {
			if (read(fclient, message, BUFFSIZE) < 0){
				perror("Failed to read from pipe");
				exit(EXIT_FAILURE);
			}
			printf("%s\n", message);
		}
	}

	finishUp();

    return 0;
}
