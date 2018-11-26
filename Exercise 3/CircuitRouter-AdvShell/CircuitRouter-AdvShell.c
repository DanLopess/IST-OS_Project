
/*
// Projeto SO - exercise 3, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
// AdvShell works as a server in a namedpipe
*/

#include "../lib/commandlinereader.h"
#include "../lib/vector.h"
#include "CircuitRouter-AdvShell.h"
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
#include<fcntl.h>

#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"

#define MAXARGS 3
#define BUFFER_SIZE 100

void waitForChild(vector_t *children) {
    while (1) {
        child_t *child = malloc(sizeof(child_t));
        if (child == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
        child->pid = wait(&(child->status));
        if (child->pid < 0) {
            if (errno == EINTR) {
                /* Este codigo de erro significa que chegou signal que interrompeu a espera
                   pela terminacao de filho; logo voltamos a esperar */
                free(child);
                continue;
            } else {
                perror("Unexpected error while waiting for child.");
                exit (EXIT_FAILURE);
            }
        }
        vector_pushBack(children, child);
        return;
    }
}

void printChildren(vector_t *children) {
    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        int status = child->status;
        pid_t pid = child->pid;
        if (pid != -1) {
            const char* ret = "NOK";
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ret = "OK";
            }
            printf("CHILD EXITED: (PID=%d; return %s)\n", pid, ret);
        }
    }
    puts("END.");
}

void stdinParse() {

}

void pipeParse() {

}

int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int MAXCHILDREN = -1;
    vector_t *children;
    int runningChildren = 0;
    fd_set fdset;

    FD_ZERO(&fdset); /* fills fdset with all zero's */

    if(argv[1] != NULL){
        MAXCHILDREN = atoi(argv[1]);
    }

    children = vector_alloc(MAXCHILDREN);

    printf("Welcome to CircuitRouter-AdvShell\n\n");

    if (unlink("../tmp/AdvShell.pipe") < 0) 
        exit(-1); /* errno if unlink failed */
    
    if (mkfifo("../tmp/AdvShell.pipe", 0777) < 0) 
        exit(-1); /* tries to make a new pipe */

    while (1) {
        int numArgs;
        int fshell;
        int max; /* stores highest file descriptor*/

        if ((fshell = open("../tmp/AdvShell.pipe", O_RDONLY)) < 0)
            exit(-1);


        /* ver sugestao do professor, utilizar o select para decidir se leio do stdin ou do pipe */
        numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);

        if (fshell > numArgs) /* determines which files descriptor is the highest */
            max = fshell + 1;
        else 
            max = numArgs + 1;

        FD_SET(fshell, &fdset);  /* sets pipe for listening */
        FD_SET(numArgs, &fdset); /* sets stdin for listening, maybe not numArgs */
        
        select(max, &fdset, NULL, NULL, NULL); /* waits for either the pipe or stdin */

        if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0){
            int pid;
            if (numArgs < 2) {
                printf("%s: invalid syntax. Try again.\n", COMMAND_RUN);
                continue;
            }
            if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN) {
                waitForChild(children);
                runningChildren--;
            }

            pid = fork();
            if (pid < 0) {
                perror("Failed to create new process.");
                exit(EXIT_FAILURE);
            }

            if (pid > 0) {
                runningChildren++;
                printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
                continue;
            } else {
                char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
                char *newArgs[3] = {seqsolver, args[1], NULL};

                execv(seqsolver, newArgs); /* if main returned 0, then all ok */
                perror("Error while executing child process"); /* Not supposed to get here */
                exit(EXIT_FAILURE);
            }
        }

        else if (numArgs == 0){
            /* No argument, ignores and asks again */
            continue;
        }
        else
            printf("Command not supported.\n");

    }

    for (int i = 0; i < vector_getSize(children); i++) {
        free(vector_at(children, i));
    }
    vector_free(children);

    if (unlink("../tmp/AdvShell.pipe") < 0)
        exit(-1); /* errno if unlink failed */

    return EXIT_SUCCESS;
}
