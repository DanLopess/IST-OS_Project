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

void runSeqSolver() {
	char fileName[256];
	scanf("%s", fileName);
	execl("", "CircuitRouterseq-solver", fileName, NULL);
}

void create_child(int* childrenPIDs, int* index, int *currentChildren) {
	childrenPIDs[*index] = fork();
	if (childrenPIDs[*index] == -1){
		printf("Error! Can't fork.\n");
	}
	if (childrenPIDs[*index] == 0) { /* Child Process */
		runSeqSolver();
	} else { /* Dad Process */
		index++;
		currentChildren++;
	}
}

void parseCommand(int maxChildren){
	int *childrenPIDs ;
	int index = 0; /* Index of last childrenPID inserted into array */
	int maxIndex = maxChildren;
	int currentChildren = 0; /* Control the number of children executing */
	int status, i;
	char command[256]; /*large size to acomodate large file names*/

/*Initialization*/
	if(maxChildren >= 0){
		childrenPIDs = (int*) malloc(sizeof(int)*maxChildren);
	} else{
		childrenPIDs = (int*) malloc(sizeof(int)*10);
	}

	while (scanf("%s", command)) {
		if (strcmp(command, "exit")) {
			for (i = 0; i <= index; i++){
				if (waitpid(childrenPIDs[i], &status, WIFEXITED(status))){
					printf("CHILD EXITED (PID=%d; return OK)\n", childrenPIDs[i]);
				} else{
					printf("CHILD EXITED (PID=%d; return NOK)\n", childrenPIDs[i]);
				}
			}
			printf("END.\n");
			exit(0);
		}
		else if (strcmp(command, "run")) {
			if (maxChildren == -1 || currentChildren < maxChildren) {
				if (index == maxIndex) {
					childrenPIDs = realloc(childrenPIDs, (sizeof(int)*index + 10));/* Doubles size of array*/
					maxIndex = maxIndex + 10;
				}
				create_child(childrenPIDs, &index, &currentChildren);
			}

			else { /*too many children to create another */
				waitpid(-1, &status, 0); /* Wait for any child to terminate (at least one)*/
				currentChildren--;
				create_child(childrenPIDs, &index, &currentChildren);
			}
		}

		else{
			printf("Invalid command!\n");
		}
	}
	free(childrenPIDs);
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
		if (argc == 0)
			maxChildren = -1; /* unlimited children */
		else
			maxChildren = atoi(argv[1]);
		parseCommand(maxChildren);
	}

	else{
		printf("Error! Too many arguments.\n");
		exit(1);
	}
	return 0;
}
