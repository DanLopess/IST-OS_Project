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
#include <sys/stat.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define COMMAND_RUN "run"
#define COMMAND_EXIT "exit"
#define PIPENAME "/tmp/AdvShell.pipe"

#define MAXARGS 3
#define BUFFER_SIZE 256

int fshell;

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

void waitForInput(fd_set *fdset) {
    int selected;
    int max; /* stores highest file descriptor*/
    int stdin = STDIN_FILENO;
	FD_ZERO(fdset);

    if (fshell > stdin) /* determines which files descriptor is the highest */
        max = fshell + 1;
    else
        max = stdin + 1;

    FD_SET(fshell, fdset); /* sets pipe for listening */
    FD_SET(stdin, fdset);  /* sets stdin for listening */

    selected = select(max, fdset, NULL, NULL, NULL); /* waits for either the pipe or stdin */

    if (selected == -1 || selected == 0)
    {
        perror("Error reading instructions.");
        exit(EXIT_FAILURE);
    }
}

void initiateShellPipe() {
    unlink(PIPENAME);

    if (mkfifo(PIPENAME, 0777) < 0)
    {
        perror("Failed to create pipe");
        exit(EXIT_FAILURE); /* tries to make a new pipe */
    }

    if ((fshell = open(PIPENAME, O_RDWR)) < 0)
    {
        perror("Failed to open pipe");
        exit(EXIT_FAILURE);
    }
}

void finishUp(vector_t *children){
    for (int i = 0; i < vector_getSize(children); i++)
    {
        free(vector_at(children, i));
    }
    vector_free(children);

    close(fshell);
    unlink(PIPENAME);
}

int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE+PATH_MAX];
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

    initiateShellPipe();

    while (1) {
        int numArgs;
        int stdin = STDIN_FILENO;
        bool_t isClient = FALSE;

        waitForInput(&fdset);

        if (FD_ISSET(stdin, &fdset)) {
            numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
        }
        if (FD_ISSET(fshell, &fdset)) {
            read(fshell, buffer, BUFFER_SIZE);
            numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
            if (numArgs == 3) /* make sure it has client's pipeName */
                isClient = TRUE;
            else {
                perror("No enough arguments.");
                continue; /* if no return parameter is found, then ignores this client command */
            }
        }

        if (numArgs == -1) {
            perror("Error reading instructions.");
            exit(EXIT_FAILURE);
        }

        if (!isClient && numArgs > 0 && (strcmp(args[0], COMMAND_EXIT) == 0))
        {
            printf("CircuitRouter-AdvShell will exit.\n--\n");

            /* Espera pela terminacao de cada filho */
            while (runningChildren > 0)
            {
                waitForChild(children);
                runningChildren--;
            }

            printChildren(children);
            printf("--\nCircuitRouter-AdvShell ended.\n");
            break;
        }

        if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0) {
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
                printf("%s: background child started with PID %d.\n", COMMAND_RUN, pid);
                continue;
            } else {

                char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
                if (isClient) {
                    char *newArgs[3] = {seqsolver, args[1], args[2]}; /* if read from pipe */
                    execv(seqsolver, newArgs);
                } else {
                    char *newArgs[3] = {seqsolver, args[1], NULL}; /* if read from stdin */
                    execv(seqsolver, newArgs);
                }
                perror("Error while executing child process"); /* Not supposed to get here */
                exit(EXIT_FAILURE);
            }
        }

        else if (numArgs == 0)
            continue;  /* No argument, ignores and asks again */

		else { /* command is not supported */
            if (isClient) { /* if input is from client, then send through pipe */
                char* pipeName = args[2];
                int fclient;
                char message[BUFFER_SIZE];

                if ((fclient = open(pipeName, O_WRONLY)) < 0) {
                    perror ("Failed to open pipe");
                }
                strcpy(message, "Command not supported.");
                if (write(fclient, message, BUFFER_SIZE < 0))
                {
                    perror("Failed to write to pipe");
                    exit(EXIT_FAILURE);
                }
                close(fclient);
            } else /*  just prints to stdout */
                printf("Command not supported.\n");
        }

    }

    /* finish up */
    finishUp(children);

    return EXIT_SUCCESS;
}
