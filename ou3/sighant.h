
// Prevent double inport
#ifndef sighant
#define sighant

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "mish.h"
#include "parser.h"

void catchSignal(int theSignal);

#endif