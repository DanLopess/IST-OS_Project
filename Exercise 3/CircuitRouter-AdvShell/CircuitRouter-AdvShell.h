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

void waitForChild();
void printChildren();
int waitForInput(fd_set* fdset);
void initiateShellPipe();
void sendNotSupported(char* pipeName);
void finishUp();
int exec_command(char **args, int control, int numArgs);

#endif /* CIRCUITROUTER_SHELL_H */
