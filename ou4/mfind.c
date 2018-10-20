
#include "mfind.h"

int lengthOfQueue = 0;
Queue *toBeVisitedQueue;
char *filenameGoal = "";

int tflag = 0;
char *tvalue = "";

pthread_cond_t condition;
pthread_mutex_t mtx;

pthread_t threadArray[1024];
int numberOfThreads = 1;
int threadsWaiting = 0;
int lastThreadDone = 0;

char *buildFullFilePathconcat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 3); // +1 for the null-terminator
    // in real code you would check for errors in maloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


int browseDirectory(void){

    
    char *parentDirectoryName = dequeueFromQueue();

    if (parentDirectoryName == NULL){
        fprintf(stderr,"NULL parentDirectoryName\n");

        return -1;
    }

    if (strlen(parentDirectoryName) == 0){ // Empty string from dequeue
        fprintf(stderr, "empty parentDirectoryName\n");
        return -1;
    }

    DIR *currDirStream;
    struct dirent *dirEntry;

    if ((currDirStream = opendir(parentDirectoryName)) == NULL)
    {
        perror(parentDirectoryName);
        return -1;
    }

    while ((dirEntry = readdir(currDirStream)) != NULL){

        // Ignore . and ..
        if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0){
           
            // halfPath is only parentDirectory + "/"
            char *halfPath = buildFullFilePathconcat(parentDirectoryName, "/");

            // fullPath is full directory + filename
            char *fullPath = buildFullFilePathconcat(halfPath, dirEntry->d_name);
            
            struct stat buffer;
            if (lstat(fullPath, &buffer) < 0){
                perror(dirEntry->d_name);
            }

            switch (buffer.st_mode & S_IFMT){
            case S_IFREG:

                // Not intrested in files
                if (((strcmp(tvalue, "d")) != 0) && ((strcmp(tvalue, "l")) != 0)){
                    if ((strcmp(dirEntry->d_name, filenameGoal) == 0)){
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
                if (((strcmp(tvalue, "l")) == 0) && (strstr(dirEntry->d_name, filenameGoal) != NULL))
                {
                    printf("%s\n", fullPath);
                }
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

    numberOfThreads = atoi(pvalue);

    // If not given -> mainthread #1
    if(numberOfThreads == 0){
        numberOfThreads++;
    }

    printf("Threads: %d\n", numberOfThreads);

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&condition, NULL);

    // Creating threads
    //pthread_t threadArray[numberOfThreads];

    threadArray[0] = pthread_self(); 
    for (int i = 1; i < numberOfThreads; i++){
        printf("Creating thread\n");
        pthread_t newThread = pthread_self();
        threadArray[i] = newThread;
    }

    for(int i = 1; i<numberOfThreads;i++){
        if (pthread_create(&threadArray[i], NULL, &goThreadGo, NULL)){
            fprintf(stderr, "Couldn't create thread\n");
            exit(1);
        }
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

    printf("Main-thread: %08x\n", (int)pthread_self());

    goThreadGo(NULL);

    printf("Main-thread returned -> return 0\n");

    lastThreadFinished();

    return 0;
}

void lastThreadFinished(void){
    printf("%08x calls lastThreadFinished()\n", (int)pthread_self());
    printf("ThreadArray:\n");
    for (int i = 0; i < numberOfThreads; i++)
    {
        printf("%d|%08x\n", i, (int)threadArray[i]);
    }

    for (int i = 0; i < numberOfThreads; i++){
        printf("Waiting for %d|%08x\n", i, (int)threadArray[i]);

        if (pthread_equal(threadArray[i], pthread_self()) != 0){
            printf("don't wait for myself (%08x)\n",(int)pthread_self());
        }else{
            printf("%d returned: %08x\n", i, pthread_join(threadArray[i], NULL));
        }
    }

    printf("All done\n");

    Queue_free(toBeVisitedQueue); // CALL ONLY ONCE

    printf("Queue freed\n");

}

void enqueueCharToQueue(char *name){
        
    //Node *start_dir = Node_init((sizeof(name)*2));
    Node *start_dir = Node_create();

    char *stringToBeEnqueued = (char *)malloc((strlen(name) + 1) * sizeof(char));

    strcpy(stringToBeEnqueued, name);
    start_dir->name = stringToBeEnqueued;
    Enqueue(toBeVisitedQueue, start_dir);
}


// WRAPER Increment lengthOfQueue with 1 threadsafely
void addDirectoryToQueue(char *newDir){
    pthread_mutex_lock(&mtx);

    enqueueCharToQueue(newDir);
    lengthOfQueue++;
    pthread_mutex_unlock(&mtx);
}

// WRAPER
char* dequeueFromQueue(void){
    pthread_mutex_lock(&mtx);

    if(lengthOfQueue == 0){
        fprintf(stderr,"Queue is already empty\n");
        return NULL;
    }

    // Make threadsafe
    Node *tempNode = Dequeue(toBeVisitedQueue);
    char *tempName = tempNode->name;

    free(tempNode);

    lengthOfQueue--;
    pthread_mutex_unlock(&mtx);
    return tempName;
}


// Function to be called with threads -> Runs search algorithm
void *goThreadGo(void *arg){

    printf("Thread: %08x calls goThreadGo!\n", (int)pthread_self());
    arg++;

    int callsToOpenDir = 0;

    callsToOpenDir++;

    while(true){
        pthread_mutex_lock(&mtx);
        while (lengthOfQueue == 0 && lastThreadDone == 0){

            printf("\nEmpty queue\n");
            
            // Only main-thread
            if (numberOfThreads == 1){

                printf("Thread: %08x Reads: %d -1--------------\n", (int)pthread_self(), callsToOpenDir);
                return NULL;

            } else if(threadsWaiting == numberOfThreads-1){
                lastThreadDone = 1;

                printf("Thread %08x reahced end\n",(int)pthread_self());

                printf("threadsWaiting: %d\n", threadsWaiting);
                printf("numberOfThreads: %d\n", numberOfThreads);
                printf("DONE!\n");

                while(threadsWaiting != 0){
                    printf("Broadcasting til no thread (%d) w8s\n",threadsWaiting);
                    pthread_mutex_unlock(&mtx);
                    pthread_cond_broadcast(&condition);
                }

                printf("Thread: %08x Reads: %d -2--------------\n", (int)pthread_self(), callsToOpenDir);

                return NULL;
            }else{
                threadsWaiting++;
                printf("Thread: %08x sleeps\n", (int)pthread_self());
                pthread_cond_wait(&condition, &mtx);
                threadsWaiting--;

                // Condwaita

                if(lastThreadDone == 1){
                    printf("Thread: %08x Reads: %d -3--------------\n", (int)pthread_self(), callsToOpenDir);
                    return NULL;
                }else{
                    printf("Thread: %08x wakes up!\n", (int)pthread_self());
                }

            }

        }
        
        pthread_mutex_unlock(&mtx);


        browseDirectory();
        callsToOpenDir++;
        //

        


        // lock 
        // kö tom? | Lås upp
        // |        Ja -->> 
        // |        Antalet väntande-trådar = trådar
        // |        Ja ->   Broadcasta och return
        // |        Nej ->  Cond-waita
        // |
        // Nej --> Kör goThreadGo


    }


    

    //pthread_t id = pthread_self();
    //printf("Thread: %08x Reads: %d\n", (int)id, callsToOpenDir);

    return 0;
}
/*
// Function to be called with threads -> Runs search algorithm
void *goThreadGo(void *arg){
    printf("goThreadGo!\n");
    arg++;
    int callsToOpenDir = 0;

    while (!isQueueEmpty()){


        browseDirectory();        
        callsToOpenDir++;



    }

    pthread_t id = pthread_self();

    
    return 0;
}*/
