
// Prevent double inport
#ifndef mish
#define mish

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#include <signal.h>

#include <unistd.h>
#include <sys/wait.h>

#include "execute.h"
#include "sighant.h"

static unsigned int NR_OF_CHILDREN = 0;
static int PID_CHILDREN_ARRAY[MAXCOMMANDS];

#define STRCMP(a, R, b) strcmp(a, b) R 0

static const unsigned int MAX_COMMANDS = 20;

int internal_echo(int argc, char *argv[]);

int internal_cd(char *argv[]);

int prompt(command commandArr[], int *NrOfCommands);

int runCommand(command com, int pipeIndex, int nrOfCommands, int pipeArray[][2]);

int runShell(void);

int loopRunShell(void);

void killChildren(int sig);

int main(void);

void print_command(command com);

#endif