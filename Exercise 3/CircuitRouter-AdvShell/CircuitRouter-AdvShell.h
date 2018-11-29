#ifndef CIRCUITROUTER_ADVSHELL_H
#define CIRCUITROUTER_ADVSHELL_H

#include "../lib/vector.h"
#include "../lib/timer.h"
#include <sys/types.h>

typedef struct {
    pid_t pid;
    int status;
    TIMER_T time1, time2;
} child_t;

void waitForChild(vector_t *children);
void printChildren(vector_t *children);
int waitForInput(fd_set* fdset);
void initiateShellPipe();
void sendNotSupported(char* pipeName);
void finishUp(vector_t *children);
int exec_command(char **args, int control, int numArgs, vector_t *children);

#endif /* CIRCUITROUTER_SHELL_H */
