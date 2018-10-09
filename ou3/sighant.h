
// Prevent double inport

#ifndef sighant
#define sighant

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

//#include "mish.h"
#include "parser.h"

typedef void Sigfunc(int);

Sigfunc *signalHand(int signo, Sigfunc *func);

#endif