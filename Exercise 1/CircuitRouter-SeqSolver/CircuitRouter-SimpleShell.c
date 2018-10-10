/* =============================================================================
CircuitRouter-SimpleShell

Developed by Daniel Lopes & Nuno Ramos, IST
 =============================================================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void runSeqSolver() {
	char command[256];
	scanf("%s", command);
	execl("CircuitRouterseq-solver", command);

	return;
}

void parseCommand(int maxChildren){
	int pid;
	int currentChildren = 0;
	char command[256]; /*large size to acomodate large file names*/

	while (1) {
		scanf("%s", command);

		if (!strcmp(command, "exit")) {
			exit(0);
		}
		else if (!strcmp(command, "run")) {
			scanf("%s", command);

			if (maxChildren == -1 || currentChildren < maxChildren) {
				pid = fork();
				if (pid == 0) {
					runSeqSolver();
				}
			}
			else { /*too many children to create another */
				/*wait() TODO nao faco ideia como se usa o wait mas acho q precisa*/
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
