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

int arr[300];

void *rCode(void *dummy)
{
    arr[0] = 1;
    printf("rCode sleep\n");
    sleep(5);
    printf("rCode sleep - DONE\n");
    return NULL;
}

void *wCode(void *dummy){

    arr[0] = 2;
    return NULL;
}

char *buildFullFilePathconcat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 3); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int browseDirectory(char *parentDirectoryName){
    DIR *currDirStream;
    struct dirent *dirEntry;

    if ((currDirStream = opendir(parentDirectoryName)) == NULL)
    {
        perror("testDir");
        fprintf(stderr,
                "Error: Unable to open dir '%s' on line %d\n",
                parentDirectoryName, __LINE__);
        return -1;
    }else{
        fprintf(stdout, "Opening directory '%s'\n\n", parentDirectoryName);
    }

    while ((dirEntry = readdir(currDirStream)) != NULL){

        // Ignore . and ..
        if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
        {
           
            printf("dirEntry->d_name = %s\n", dirEntry->d_name);

            char *halfPath = buildFullFilePathconcat(parentDirectoryName, "/");
            printf("halfPath: %s\n",halfPath);
            char *fullPath = buildFullFilePathconcat(halfPath, dirEntry->d_name);
            free(halfPath);
            printf("fullPath: %s\n", fullPath);

            
            struct stat buffer;
            if (lstat(fullPath, &buffer) < 0){
                perror(dirEntry->d_name);
                fprintf(stderr, "Error: Can't stat file '%s' on line %d\n", dirEntry->d_name, __LINE__);
            }
            free(fullPath);

            printf("%d\n",buffer.st_mode);
            printf("S_ISREG(buffer.st_mode) -> %d\n", S_ISREG(buffer.st_mode));

            if (!S_ISREG(buffer.st_mode)){
                fprintf(stderr, "'%s' is NOT a regular file\n",parentDirectoryName);
            }

            printf("\n");
        }
    }

    return 0;
}

int main(void)
{
    arr[0] = 0;
    
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

    browseDirectory("/Users/Emil/Grov-Filer/5dv088/ou4/testDir");

    putchar('\n');

    return 0;
}

