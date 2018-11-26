/*
// Projeto SO - exercise 3, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
// AdvShell works as a client in a namedpipe

TODO: 
- 1 namedpipe to send command to advshell (which is waiting for commands)
- 1 namedpipe to receive info from advshell
*/

#include "lib/commandlinereader.h"
#include "lib/vector.h"
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
#include <fcntl.h>

#define NAMESIZE 100

int main(int argc, char const *argv[])
{
    int fshell, fclient;
    char* pipeName = (char*) malloc(sizeof(char)*NAMESIZE); /* creates a string with NAMESIZE characters */

    strcpy(pipeName, "../temp/ClientXXXXXX"); /* fills pipeName with generic expression */

    if(mkdtemp(pipeName) < 0) /* create own pipe file */
        exit(-1);
    
    strcat(pipeName, "/client.pipe");
    
    unlink(pipeName);

    if (mkfifo(pipeName, 0777) < 0)
        exit(-1);

    // ... read from stdin... send to advshell pipe, read from client pipe



    unlink(pipeName);
    
    return 0;
}


