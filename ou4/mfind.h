#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h> //For PATH_MAX

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>

#include <stdio.h>
#include <string.h>

#include "queue.h"

char *buildFullFilePathconcat(const char *s1, const char *s2);
void enqueueCharToQueue(char *name);
int browseDirectory(void);
int main(int argc, char **argv);