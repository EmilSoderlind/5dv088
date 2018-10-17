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

#include "queue.c"

int lengthOfQueue = 0;
Queue *toBeVisitedQueue;
char *filenameGoal = "";

int tflag = 0;
char *tvalue = "";

char *buildFullFilePathconcat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 3); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void enqueueCharToQueue(char *name){
    Node *start_dir = Node_init((sizeof name)*5);
    char *stringToBeEnqueued = (char *) malloc((strlen(name) + 1) * sizeof(char));

    strcpy(stringToBeEnqueued, name);
    printf("Enqueing: %s\n",name);
    start_dir->name = stringToBeEnqueued;
    Enqueue(toBeVisitedQueue, start_dir);
}

int browseDirectory(){

    char *parentDirectoryName = Dequeue(toBeVisitedQueue)->name;

    if(parentDirectoryName == NULL){
        return -1;
    }

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

    printf("\n\n------- Searching: %s-------\n", parentDirectoryName);

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
            }

            int foundFileFlag = 0;

            switch (buffer.st_mode & S_IFMT){
            case S_IFREG:

                printf("filenameGoal: %s\n",filenameGoal);
                printf("tflag: %d\n",tflag);
                printf("tvalue: %s\n", tvalue);
                printf("fullPath: %s\n", fullPath);

                if(strlen(tvalue) == 0){
                    // Type specified 
                    if (tflag && (strstr(fullPath, filenameGoal) != NULL) && (strstr(fullPath, tvalue) != NULL)) {
                        foundFileFlag = 1;
                    }
                // Type NOT specified 
                }else if (strstr(fullPath, filenameGoal) != NULL){
                    foundFileFlag = 1;
                }

                if(foundFileFlag == 1){
                    printf("%s\n",fullPath);
                }
                break;
            case S_IFDIR:
                printf("Append: %s --> Queue\n",fullPath);
                
                enqueueCharToQueue(fullPath);
                lengthOfQueue++;
                
                break;
            case S_IFLNK:
                printf("Ignore: %s\n", fullPath);
                break;
            default:
                printf("Ignore: %s\n", fullPath);
                printf("\n");
            }
            free(fullPath);
        }
    }
    free(parentDirectoryName);
    closedir(currDirStream);
    return 0;
}



int main(int argc, char** argv){

    int c;
    
    int pflag = 0;
    char *pvalue = "";

    while ((c = getopt(argc, argv, "t:p:")) != -1){
        switch (c){
            case 't':
                tflag = 1;
                tvalue = optarg;
                break;
            case 'p':
                pflag = 1;
                pvalue = optarg;
                break;
            case '?':
                if (optopt == 'c'){
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }else if (isprint(optopt)){
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }else{
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",optopt);
                return 1;
                }
            default:
                abort();
        }
    }

    printf("tflag: %d\n",tflag);
    printf("tvalue: %s\n", tvalue);
    printf("pflag: %d\n", pflag);
    printf("pvalue: %s\n", pvalue);

    filenameGoal = argv[argc-1];


    if(argc == 1){
        printf("Empty argv!\n");
        return -1;
    }

    // Creating queue
    toBeVisitedQueue = Queue_create();
 
    enqueueCharToQueue("testDir");
    lengthOfQueue++;
    
    

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
        printf("-1- lengthOfQueue: %d\n", lengthOfQueue);
        
        browseDirectory();
        
        printf("-2- lengthOfQueue: %d\n", lengthOfQueue);
    }

    Queue_free(toBeVisitedQueue);

    printf("End of queue!\n");

    return 0;
}

