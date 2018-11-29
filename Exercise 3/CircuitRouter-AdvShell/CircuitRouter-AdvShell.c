/*
// Projeto SO - exercise 3, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
*/

#include "../lib/commandlinereader.h"
#include "../lib/vector.h"
#include "../lib/timer.h"
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
int MAXCHILDREN = -1;
int runningChildren = 0;
vector_t *children;

void waitForChild(vector_t *children) {
    while (1) {
        int i;
        child_t *child = malloc(sizeof(child_t));
        if (child == NULL)
        {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
        child->pid = wait(&(child->status));
        if (child->pid < 0)
        {
            if (errno == EINTR)
            {
                /* Error code means wait for child was interrupted,
				therefore must be resumed */
                free(child);
                continue;
            }
            if(errno == ECHILD){
                break;
            }
            else
            {
                perror("Unexpected error while waiting for child.");
                exit(EXIT_FAILURE);
            }
        }
        for (i = 0; i < vector_getSize(children); i++) {
            child_t *childTemp = vector_at(children, i);
            if (childTemp->pid == child->pid)
            {
                childTemp->status = child->status;
            }
        }

        return;
    }
}

void printChildren(vector_t *children) {
    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        int status = child->status;
        pid_t pid = child->pid;
        TIMER_T time1,time2;

        time1 = child->time1;
        time2 = child->time2;

        if (pid != -1) {
            const char* ret = "NOK";
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ret = "OK";
            }
            printf("CHILD EXITED: (PID=%d; return %s; %ds)\n", pid, ret, (int)TIMER_DIFF_SECONDS(time1, time2));
        }
    }
    puts("END.");
}

/*returns 1 if all ok, returns 0 if must redo while */
int waitForInput(fd_set *fdset) {
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
        return 0;
    }
    return 1;
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

void sendNotSupported(char* pipeName) {
    int fclient;
	char message[BUFFER_SIZE];

    if ((fclient = open(pipeName, O_RDWR)) < 0) {
        perror ("Failed to open pipe");
    }
    strcpy(message, "Command not supported.");
    if (write(fclient, message, BUFFER_SIZE) < 0) {
        perror("Failed to write to pipe");
        exit(EXIT_FAILURE);
    }
    close(fclient);
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

/* returns 1 if break while, 0 if continue */
int exec_command(char** args, int control, int numArgs, vector_t *children) {
    /* control = 0, exec from stdin, control = 1, exec from pipe */
    if (numArgs == -1)
    {
        perror("Error reading instructions.");
        exit(EXIT_FAILURE);
    }

    if (!control && numArgs > 0 && (strcmp(args[0], COMMAND_EXIT) == 0))
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
        return 1;

    }

    if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0)
    {
        if (numArgs < 2)
        {
            printf("%s: invalid syntax. Try again.\n", COMMAND_RUN);
            return 0;
        }
        if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN)
        {
            waitForChild(children);
            runningChildren--;
        }

        child_t *child = malloc(sizeof(child_t));
        if (child == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
        child->pid = fork();
        if (child->pid < 0)
        {
            perror("Failed to create new process.");
            exit(EXIT_FAILURE);
        }

        if (child->pid > 0)
        {
            if (TIMER_READ(child->time1) < 0)
            {
                perror("Failed to obtain time.");
                exit(EXIT_FAILURE);
            }
            runningChildren++;
            vector_pushBack(children, child);
            printf("%s: background child started with PID %d.\n", COMMAND_RUN, child->pid);
            return 0;
        }
        else
        {
            char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
            if (control)
            {
                char *newArgs[4] = {seqsolver, args[1], args[numArgs - 1], NULL}; /* if read from pipe */
                execv(seqsolver, newArgs);
            }
            else
            {
                char *newArgs[3] = {seqsolver, args[1], NULL}; /* if read from stdin */
                execv(seqsolver, newArgs);
            }
            perror("Error while executing child process"); /* Not supposed to get here */
            exit(EXIT_FAILURE);
        }
    }

    else if (numArgs == 0)
        return 0; /* No argument, ignores and asks again */

    else
    {                 /* command is not supported */
        if (control) /* if input is from client, then send through pipe */
            sendNotSupported(args[numArgs - 1]);
        else /*  just prints to stdout */
            printf("Command not supported.\n");
    }
    return 0;
}

void handler(int sig) {
    pid_t pid;

    child_t *child = malloc(sizeof(child_t));

    if ((pid = waitpid(-1, &(child->status), WNOHANG)) < 0) {
        perror("Failed to wait for child.");
        exit(EXIT_FAILURE);
    }
    child->pid = pid;
    if (TIMER_READ(child->time2) < 0) {
        perror("Failed to obtain time.");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < vector_getSize(children); i++) {
        child_t *childTemp = vector_at(children, i);
        if (childTemp->pid == child->pid) {
            childTemp->status = child->status;
			childTemp->time1 = child->time1;
            childTemp->time2 = child->time2;
            break;
        }
    }

}

int main (int argc, char** argv) {
    signal(SIGCHLD, handler);
    char *args[MAXARGS + 1], *args1[MAXARGS+1];
    char buffer[BUFFER_SIZE], buffer1[BUFFER_SIZE];
    fd_set fdset;

    FD_ZERO(&fdset); /* fills fdset with all zero's */

    if(argv[1] != NULL) {
        MAXCHILDREN = atoi(argv[1]);
    }

    children = vector_alloc(MAXCHILDREN);

    printf("Welcome to CircuitRouter-AdvShell\n\n");

    initiateShellPipe();

    while (1) {
        int stdin = STDIN_FILENO;

        if(!waitForInput(&fdset))
            continue;

        if (FD_ISSET(stdin, &fdset)) {
            int numArgs;
            numArgs = readLineArguments(0,args1, MAXARGS+1, buffer, BUFFER_SIZE); /* function was changed
            to support extra arguments (int control) */
            if(exec_command(args1, 0, numArgs, children))
                break;
            else
                continue;
        }
        if (FD_ISSET(fshell, &fdset)) {
            read(fshell, buffer1, BUFFER_SIZE);
            int numArgs = readLineArguments(1,args, MAXARGS + 1, buffer1, BUFFER_SIZE);
            if (numArgs == 3)  {/* make sure it has client's pipeName */
                if(exec_command(args,1,numArgs, children)) /* returned 1 then must leave while */
                    break;
                else
                    continue; /* returned 0 then must go to the start of while*/
            } else {
				sendNotSupported(args[numArgs-1]);
                continue; /* if no return parameter is found, then ignores this client command */
            }
        }
    }

    /* finish up */
    finishUp(children);

    return EXIT_SUCCESS;
}
