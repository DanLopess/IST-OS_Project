/* =============================================================================
CircuitRouter-SimpleShell

Developed by Daniel Lopes & Nuno Ramos, IST
 =============================================================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../lib/commandlinereader.h"

void create_child(int* childrenPIDs, int* index, int *currentChildren, char **argVector) {
	childrenPIDs[*index] = fork();
	if (childrenPIDs[*index] == -1){
		printf("Error! Can't fork.\n");
	}
	else if (childrenPIDs[*index] != 0) { /* Father Process */
		*index = *index + 1;
		*currentChildren = *currentChildren + 1;
	} else { /* Child Process */
		execl("CircuitRouter-seqsolver", "CircuitRouter-seqsolver", argVector[1],(char*) NULL);
	}
}

void parseCommand(int maxChildren){
	int *childrenPIDs, *childrenSuccess;
	int index = 0; /* Index of last childrenPID inserted into array */
	int maxIndex = maxChildren;
	int currentChildren = 0; /* Control the number of children executing */
	int status, result, control = 0, i;
	char *argVector[3], line[256];
	int argVectorSize = 3, bufferSize = 256;

/*Initialization*/
	if(maxChildren >= 0){
		childrenPIDs = (int*) malloc(sizeof(int)*maxChildren);
		childrenSuccess = (int*) malloc(sizeof(int)*maxChildren);
	} else{
		childrenPIDs = (int*) malloc(sizeof(int)*10);
		childrenSuccess = (int*) malloc(sizeof(int)*10);
	}

	while (1) {
		result = readLineArguments(argVector, argVectorSize, line, bufferSize);
		if (result == 1 && !strcmp(argVector[0], "exit")) {
			for (; control < index; control++) /*Waits for all unfinished children*/
				childrenSuccess[control] = waitpid(childrenPIDs[control], &status, WIFEXITED(status));

			for (i = 0; i < index; i++) {
				if (childrenSuccess[i] != -1) {
					printf("CHILD EXITED (PID=%d; return OK)\n", childrenPIDs[i]);
				} else{
					printf("CHILD EXITED (PID=%d; return NOK)\n", childrenPIDs[i]);
				}
			}
			printf("END.\n");
			free(childrenPIDs);
			free(childrenSuccess);
			exit(0);
		}

		else if (result == 2 && !strcmp(argVector[0], "run")) {
			if (maxChildren == -1 || currentChildren < maxChildren) {
				if (index == maxIndex) {
					childrenPIDs = realloc(childrenPIDs, (sizeof(int)*maxIndex + 10));/* Increases size of array*/
					childrenSuccess = realloc(childrenPIDs, (sizeof(int)*maxIndex + 10));
					maxIndex = maxIndex + 10;
				}
			}

			else { /*too many children to create another */
				childrenSuccess[control] = waitpid(-1, &status, 0); /* Wait for any child to terminate (at least one)*/
				printf("childrenSuccess[control] = %d", childrenSuccess[control]);
				currentChildren--;
				control++;
			}
			create_child(childrenPIDs, &index, &currentChildren, argVector);
		}
		else{
			printf("Invalid command!\n");
		}
	}
}


/* =============================================================================
 * main
 * =============================================================================
 */
int main(int argc, char** argv){
	/*
	 * Initialization
	 */
	 int maxChildren;

	/* Main functionality */
	if (argc == 1 || argc == 2) { /*No more than 1 argument allowed*/
		if (argc == 1)
			maxChildren = -1; /* unlimited children */
		else
			maxChildren = atoi(argv[1]); /* convert string from argument to integer */
		parseCommand(maxChildren);
	}

	else{
		printf("Error! Too many arguments.\n");
		exit(1);
	}
	return 0;
}
