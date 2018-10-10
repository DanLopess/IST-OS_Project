/* =============================================================================
CircuitRouter-SimpleShell

Developed by Daniel Lopes & Nuno Ramos, IST
 =============================================================================*/
#include <string.h>

void runSeqSolver(){
	// TODO this function when called, runs seqsolver with a given
	// input file
}

void parseCommand(int maxChildren){
	int pid;
	int currentChildren = 0;
	char command[256]; /*large size to acomodate large file names*/

	while() {
		scanf("%s", command);

		if (!strcmp(command, "exit")) {
			exit(1);
		}
		else if (!strcmp(command, "run")) {
			scanf("%s", command);

			if (maxChildren == -1 || currentChildren < maxChildren) {
				pid = fork();
				if (pid != 0) {
					scanf("%s", command);
					int execl("CircuitRouterseq-solver", command);
					/*TODO part that runs code*/
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

	/* Main functionality */
	if (argc == 0 || argc == 1) { /*No more than 1 argument allowed*/
		int maxChildren;
		if (argc == 0)
			maxChildren = -1; /* unlimited children */
		else
			maxChildren = argv[0];
		parseCommand(maxChildren);
	}
	else{
		printf("Error! Too many arguments.");
		exit(1);
	}
	return 0;
}
