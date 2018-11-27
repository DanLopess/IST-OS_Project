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
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFSIZE 255

/* global pipe names */
char *pipeName; 
char *shellPipeName;
int fshell;
int fclient;

void exitRoutine() {
	close(fshell); /* need to close files */
	close(fclient);
	unlink(pipeName);
	free(pipeName); /* frees memory allocated initally */
	free(shellPipeName);
	exit(0);
}

/* 
 * reads from stdin and writes to advshell pipe 
 */
void writeLoop() {
	char command[BUFFSIZE];

	if ((fshell = open(shellPipeName, O_WRONLY)) < 0) {
		fprintf(stderr, "Failed to open shell pipe");
		exit(EXIT_FAILURE); /* error opening pipe */
	}

	while (1) /* use readLineArguments and compare ... as on advshell*/
	{
		if (fgets(command, BUFFSIZE, stdin) != NULL) {
			strcat (command, "|");
			strcat (command, pipeName);
			write(fshell, command, BUFFSIZE);
		}
	}
}

/* 
 * reads from own pipe and writes to stdout 
 */
void readLoop () {
	char message[BUFFSIZE];

	if ((fclient = open(pipeName, O_RDONLY)) < 0) {
		fprintf(stderr, "Failed to open own pipe");
		exit(EXIT_FAILURE); /* error opening pipe */
	}
	while (1) {
		if (read(fclient, message, BUFFSIZE) > 0) /* if anything was read */
			printf("%s\n", message);
	} 
}


int main(int argc, char const *argv[]) {
	int pid;
	char template[] = "../tmp/ClientXXXXXX";

	signal(SIGINT, exitRoutine);

	shellPipeName = (char *)malloc(sizeof(char) * BUFFSIZE);

	if (shellPipeName == NULL) exit(EXIT_FAILURE); /* failed to allocate memory */	
	if(argv[1] == NULL) exit(EXIT_FAILURE); /* argument must exist because it contains advshell pipename*/

	strcpy(shellPipeName, argv[1]); /* passed verification so we can assign it to shellPipeName */

	if ((pipeName = mkdtemp(template)) == NULL)
		exit(EXIT_FAILURE); /* trying to create own pipe directory*/

	strcat(pipeName, "/client.pipe");

	if (unlink(pipeName) < 0)
		exit(EXIT_FAILURE); /* errno if unlink failed */

	if (mkfifo(pipeName, 0777) < 0) { /* create own namedpipe */
		printf("Error creating pipe.\n");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid == 0) {
		readLoop();
	}
	if (pid > 0) {
		writeLoop();
	}
	else {
		exit(EXIT_FAILURE); /* failed to create child */
	}

    return 0;
}
