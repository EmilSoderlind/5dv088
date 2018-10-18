#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include <pthread.h>

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
void goThreadGo(void);

int readLengthOfQueue(void);
void addDirectoryToQueue(char *newDir);
char *dequeueFromQueue(void);
bool isQueueEmpty(void);

int main(int argc, char **argv);