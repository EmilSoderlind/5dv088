#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h> //For PATH_MAX

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <stdio.h>
#include <string.h>

#include "queue.c"


int lengthOfQueue = 0;

char *buildFullFilePathconcat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 3); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int browseDirectory(){

    char *parentDirectoryName = "testDir";
    lengthOfQueue--;

    printf("parentDirectoryName: %s\n", parentDirectoryName);

    if (parentDirectoryName == NULL){
        printf("NULL parentDirectoryName\n");
        return -1;
    }

    if (strlen(parentDirectoryName) == 0){ // Empty string from dequeue
        printf("empty parentDirectoryName\n");
        return -1;
    }

    printf("------- Searching: %s-------\n", parentDirectoryName);

    DIR *currDirStream;
    struct dirent *dirEntry;

    if ((currDirStream = opendir(parentDirectoryName)) == NULL)
    {
        perror("testDir");
        fprintf(stderr,
                "Error: Unable to open dir '%s' on line %d\n",
                parentDirectoryName, __LINE__);
        return -1;
    }

    while ((dirEntry = readdir(currDirStream)) != NULL){

        // Ignore . and ..
        if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0){
           
            char *halfPath = buildFullFilePathconcat(parentDirectoryName, "/");
            char *fullPath = buildFullFilePathconcat(halfPath, dirEntry->d_name);
            free(halfPath);
            
            struct stat buffer;
            if (lstat(fullPath, &buffer) < 0){
                perror(dirEntry->d_name);
                fprintf(stderr, "Error: Can't stat file '%s' on line %d\n", dirEntry->d_name, __LINE__);
            }

            switch (buffer.st_mode & S_IFMT){
            case S_IFREG:
                printf("Ignore: %s\n", fullPath);
                break;
            case S_IFDIR:
                printf("Append: %s --> Queue\n",fullPath);
                //enqueue(fullPath);
                lengthOfQueue++;;
                break;
            case S_IFLNK:
                printf("Ignore: %s\n", fullPath);
                break;
            default:
                printf("Ignore: %s\n", fullPath);
                printf("\n");
            }
        }
    }
    closedir(currDirStream);
    return 0;
}


int main(int argc, char** argv){

    printf("main()\n");

 
        for (int i = 1; i < argc; i++)
    {
        enqueue(argv[i]);
        lengthOfQueue++;
    }

    //list_append(&list,"testDir");
    //list_append(&list,"testDir/deepFold");

    /*
    pthread_t rthread;
    pthread_t wthread;

    if (pthread_create(&rthread, NULL, &rCode, NULL))
    {
        fprintf(stderr, "Couldn't create r thread\n");
        exit(1);
    }

    if (pthread_create(&wthread, NULL, &wCode, NULL))
    {
        fprintf(stderr, "Couldn't create w thread\n");
        exit(1);
    }

    printf("main thread waiting\n");
    pthread_join(rthread, NULL);
    pthread_join(wthread, NULL);
    printf("Threads done!\n");
    printf("arr[0] -> %d\n", arr[0]);
    */

    while(lengthOfQueue > 0){
        printf("lengthOfQueue: %d\n",lengthOfQueue);
        
        browseDirectory();

        printf("---\n");
    }

    return 0;
}

