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

/* Concatinate String 1 and String 2 and return the result
 *  Arguments:
 *  s1        String 1
 *  s2        String 2
 *  Returns:  String 1 and String 2 concatinated
 */
char *buildFullFilePathconcat(const char *s1, const char *s2);

/* Dequeue directorie from queue and search through entires for searched file
 * return 0 if success
 */
int searchDirectory(void);

/*  Function called by threads to start the search algorithm
 *  Arguments:
 *  arg      Null-argument
 */
void *threadLoop(void *arg);

/*  Enqueue-ing directorie to queue
 *  Underlying wrapper-function around queue-datatype to make it threadsafe
 *  Arguments:
 *  name    String to be added to queue
 */
void enqueueCharToQueue(char *name);

/*  Enqueue-ing directorie to queue
 *  Wrapper-function around queue-datatype to make it threadsafe
 *  Arguments:
 *  newDir  String to be added to queue
 */
void addDirectoryToQueue(char *newDir);

/*  Dequeue-ing directorie from queue
 *  Wrapper-function around queue-datatype to make it threadsafe
 *  Return: Dequeue:d string
 */
char *dequeueFromQueue(void);

/*  Called by threads to print their thread-id and numberOfDirectories they've opened.
 *  Arguments:
 *  callsToOpenDir  Number of opened directories
 */
void printThreadWork(int callsToOpenDir);

/* Check if StartDirectoryName is the searched goal
 *  Arguments:
 *  parentDirectoryName directoryName to check
 */
void checkStartFoldersIfGoal(char* parentDirectoryName);

/*  Called by Main-thread to wait for threads to exit before returning in main
*/
void joinThreads(void);

/*  Search after filename (name) of type (type) in filepath:s (start1...) in number of threads (nrthr)
 *  Example call: mfind [-t type] [-p nrthr] start1 [start2 ...] name
 *  Arguments:
 *  argc      Number of arguments
 *  argv      String-array of arguments
 *  Returns:  0 if sucess
 */
int main(int argc, char **argv);
