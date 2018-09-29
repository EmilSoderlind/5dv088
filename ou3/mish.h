#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>

#include "execute.h"
#include "parser.h"

#define STRCMP(a, R, b) strcmp(a, b) R 0

static const unsigned int MAX_COMMANDS = 20;

int internal_echo(int argc, char *argv[]);

int internal_cd(char *argv[]);

int prompt(command commandArr[], int *NrOfCommands);

int runCommand(command com, int pipeIndex, int nrOfCommands, int pipeArray[][2]);

int runShell(void);

int main(void);

void print_command(command com);