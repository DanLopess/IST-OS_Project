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
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFSIZE 255

char *pipeName;
char *shellPipeName;

void exitRoutine() {
	close(shellPipeName); /* need to close file */
	close(pipeName);
	unlink(pipeName);
	free(pipeName);
	exit(0);
}

/* 
 * reads from stdin and writes to advshell pipe 
 */
void writeLoop() {
	char* command[BUFSIZE*2];
	char* aux[BUFSIZE];
	int fshell;

	if ((fshell = open(shellPipeName, O_WRONLY)) < 0) {
		exit(-1); /* error opening pipe */
	}

	while (1) /* use readLineArguments and compare ... as on advshell*/
	{
		scanf("%s", aux);
		if (!strcmp(aux, "run "))
		{
			printf("Command not supported.\n");
			continue;
		}
		scanf("%s", command);
		strcat(command, '|'); /*Separates shellPipeName from command*/
		strcat(command, shellPipeName);
		printf("%s", shellPipeName); /*DEBUG*/
	}
}

/* 
 * reads from own pipe and writes to stdout 
 */
void readLoop () {
	char message[BUFSIZE];
	int fclient;

	if ((fclient = open(pipeName, O_RDONLY)) < 0) {
		exit(-1); /* error opening pipe */
	}
	while (1) {
		if (read(fclient, message, BUFSIZE) > 0) /* if anything was read */
			printf("%s\n", message);
	} 
}


int main(int argc, char const *argv[]) {

	signal(SIGINT, exitRoutine); /* TODO how to send those variables to this function? */
	char command[BUFSIZE];
	int pid;

	pipeName = (char*) malloc(sizeof(char)*BUFSIZE); /* creates a string with BUFSIZE characters */

	if (pipeName == NULL) exit(-1); /* failed to allocate memory */

	if (argv[1] != NULL) { /* if argument exits then it must be advshell pipeName */
		strcpy(shellPipeName, argv[1]);
	} else {
		exit(-1); /* no argument found */
	}

	if(strcpy(pipeName, mkdtemp("../temp/ClientXXXXXX") < 0)) /* create own pipe file */
        exit(-1);

    strcat(pipeName, "/client.pipe");

    if (mkfifo(pipeName, 0777) < 0) {
		printf("Error creating pipe.\n");
        exit(-1);
	}

	if (unlink(pipeName) < 0)
		exit(-1); /* errno if unlink failed */

	pid = fork();
	if (pid == 0) {
		readLoop();
	}
	if (pid > 0) {
		if (argv[1] != NULL) { /* if argument exits then it must be advshell pipeName */
			writeLoop();
		}
		else {
			exit(-1); /* advshell pipe name unknown, exits program */
		}
	}
	else {
		exit(1); 
	}

    return 0;
}
