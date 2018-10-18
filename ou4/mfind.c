

#include "mfind.h"

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


int browseDirectory(void){

    char *parentDirectoryName = dequeueFromQueue();

    if (parentDirectoryName == NULL){
        return -1;
    }

    lengthOfQueue--;


    if (parentDirectoryName == NULL){
        fprintf(stderr,"NULL parentDirectoryName\n");
        return -1;
    }

    if (strlen(parentDirectoryName) == 0){ // Empty string from dequeue
        fprintf(stderr, "empty parentDirectoryName\n");
        free(parentDirectoryName); 
        return -1;
    }

    DIR *currDirStream;
    struct dirent *dirEntry;

    if ((currDirStream = opendir(parentDirectoryName)) == NULL){
        perror(parentDirectoryName);
        return -1;
    }

    while ((dirEntry = readdir(currDirStream)) != NULL){

        // Ignore . and ..
        if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0){
           
            // halfPath is only parentDirectory + "/"
            char *halfPath = buildFullFilePathconcat(parentDirectoryName, "/");
            char *fullPath = buildFullFilePathconcat(halfPath, dirEntry->d_name);
            // fullPath is full directory + filename

            struct stat buffer;
            if (lstat(fullPath, &buffer) < 0){
                perror(dirEntry->d_name);
            }

            switch (buffer.st_mode & S_IFMT){
            case S_IFREG:

                // Not intrested in files
                if (((strcmp(tvalue, "d")) != 0) || ((strcmp(tvalue, "l")) != 0)){
                    if ((strstr(dirEntry->d_name, filenameGoal) != NULL)){
                        printf("%s\n",fullPath);
                    }
                }

                break;
            case S_IFDIR:
                
                if (((strcmp(tvalue, "d")) == 0) && (strstr(dirEntry->d_name, filenameGoal) != NULL)){
                    printf("%s\n", fullPath);
                }

                addDirectoryToQueue(fullPath);

                break;
            case S_IFLNK:
                if (((strcmp(tvalue, "l")) == 0) && (strstr(dirEntry->d_name, filenameGoal) != NULL)){
                    printf("%s\n", fullPath);
                }
                //printf("Ignore: %s\n", fullPath);
                break;
            default:
                //printf("Ignore: %s\n", fullPath);
                break;
            }
            free(halfPath);
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

    if(argc == 1){
        fprintf(stderr,"Empty argv!\n");
        return -1;
    }

    printf("Threads: %s\n", pvalue);

    filenameGoal = argv[argc - 1];

    // Calculating which argv-indexes startFolders have
    int startFolderIndex = 1;
    if(pflag){
        startFolderIndex += 2;
    }if(tflag){
        startFolderIndex += 2;
    }

    // Creating queue
    toBeVisitedQueue = Queue_create();

    // Enqueueing startfolders argv-arguments
    for(int i = startFolderIndex; i < argc-1; i++){
        addDirectoryToQueue(argv[i]);
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
    
    
    goThreadGo();

    Queue_free(toBeVisitedQueue); // CALL ONLY ONCE

    printf("End of queue!\n");

    return 0;
}

void enqueueCharToQueue(char *name){
    Node *start_dir = Node_init((sizeof name) * 5);
    char *stringToBeEnqueued = (char *)malloc((strlen(name) + 1) * sizeof(char));

    strcpy(stringToBeEnqueued, name);
    start_dir->name = stringToBeEnqueued;
    Enqueue(toBeVisitedQueue, start_dir);
}

// WRAPER Return lengthOfQueeu threadsafely
int readLengthOfQueue(void){

    // TO BUILD MORE

    return lengthOfQueue;
}

// WRAPER Increment lengthOfQueue with 1 threadsafely
void addDirectoryToQueue(char *newDir){

    // TO BUILD MORE
    lengthOfQueue++;
    enqueueCharToQueue(newDir);
}

// WRAPER
char* dequeueFromQueue(void){

    if(lengthOfQueue == 0){
        fprintf(stderr,"Queue is already empty\n");
        return NULL;
    }

    // Make threadsafe


    lengthOfQueue--;
    return Dequeue(toBeVisitedQueue)->name;
}

// WRAPER
bool isQueueEmpty(void){

    // Make threadsafe

    return (lengthOfQueue == 0);
}

// Function to be called with threads -> Runs search algorithm
void goThreadGo(void){
    printf("goThreadGo!\n");

    int callsToOpenDir = 0;

    while (!isQueueEmpty()){
        browseDirectory();
        callsToOpenDir++;
    }

    pthread_t id = pthread_self();

    printf("Thread: %08x Reads: %d\n", (int)id, callsToOpenDir);
}
