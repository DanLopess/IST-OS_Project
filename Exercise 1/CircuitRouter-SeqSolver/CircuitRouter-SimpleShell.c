/* =============================================================================
CircuitRouter-SimpleShell

Developed by Daniel Lopes & Nuno Ramos, IST
 =============================================================================*/
#include <string.h>


int main(int argc, char** argv){
	/*
	 * Initialization
	 */

	 int maxChildren, pid;
	 int currentChildren = 0;
	 int exit = 0;
	 char command[201]; /*large size to acomodate large file names*/

	/* Main functionality */
	if (argc == 0 || argc == 1) { /*No more than 1 argument allowed*/
		if (argc == 0)
			maxChildren = -1; /*unlimited children*/
		else
			maxChildren = argv[0];
	}
	else{
		printf("Error! Too many arguments.");
		exit(1);
	}

	while (!exit) {
		scanf("%s", command);

		if (!strcmp(command, "exit")) {
			exit = 1;
		}

		else if (!strcmp(command, "run")) {
			scanf("%s", command);

			if (maxChildren == -1 || currentChildren < maxChildren) {
				pid = fork();
				if (pid != 0) {
					/*TODO part that runs code*/
				}
			}

			else { /*too many children to create another */
				/*wait() TODO nao faco ideia como se usa o wait mas acho q precisa*/
			}
		}
	}


	return 0;
}
