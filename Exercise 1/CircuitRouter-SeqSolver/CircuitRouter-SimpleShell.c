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
	execl("./CircuitRouterseq-solver", fileName);
	if (1)  /*Needs to check for success*/
		printf("CHILD EXITED (PID=%d; return OK\n)", getpid());
	else
		printf("CHILD EXITED (PID=%d; return NOK\n)", getpid());
	return;
}

void parseCommand(int maxChildren){
	int *childrenPIDs = (int*) malloc(sizeof(int)*maxChildren);
	int currentChildren = 0; /* Index of last PID inserted */
	char command[256]; /*large size to acomodate large file names*/

	while (scanf("%s", command)) {
		if (!strcmp(command, "exit")) {
			wait(-1);
			for(int i = 0; i <= currentChildren; i++){
				//if child i returned successfully
				printf("CHILD EXITED (PID=%d; return OK)\n", childrenPIDs[i]);
				//if child i returned unsuccessfully
				printf("CHILD EXITED (PID=%d; return NOK)\n", childrenPIDs[i]);
			}
			//TODO wait for all children and print all children PIDs + status
			printf("END.\n");
			exit(1);
		}
		else if (!strcmp(command, "run")) {
			if (maxChildren == -1 || currentChildren < maxChildren) {
				childrenPIDs[currentChildren] = fork();
				if(childrenPIDs[currentChildren] == -1){
					printf("Error! Can't fork.\n");
					continue; /* Tries again */
				}
				if (childrenPIDs[currentChildren] == 0) { /* Child Process */
					runSeqSolver();
				} else { /* Dad Process */
					currentChildren++;
					continue; /* Go back to begginning of while and reads another command*/
				}
			}
			else { /*too many children to create another */
				wait(-1); /* Wait for any child to terminate (at least one)*/
				currentChildren--;
			}
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
	if (argc == 0 || argc == 1) { /*No more than 1 argument allowed*/
		if (argc == 0)
			maxChildren = -1; /* unlimited children */
		else
			maxChildren = atoi(argv[0]);
		parseCommand(maxChildren);
	}

	else{
		printf("Error! Too many arguments.");
		exit(1);
	}
	return 0;
}
