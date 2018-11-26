#ifndef CIRCUITROUTER_CLIENT_H
#define CIRCUITROUTER_CLIENT_H

#include "../lib/vector.h"
#include <sys/types.h>

void exitRoutine(char *pipeName, char *shellPipeName);
void writeLoop(char* shellPipeName);
void readLoop(char *pipeName);

#endif /* CIRCUITROUTER_CLIENT_H */
