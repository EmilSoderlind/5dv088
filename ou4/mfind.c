
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

void checkStartFoldersIfGoal(char* parentDirectoryName){
  // -vvv--- Testing if startFolder is goal ---vvv-

  struct stat bufferStartFolder;
  if (lstat(parentDirectoryName, &bufferStartFolder) < 0){
      fprintf(stderr, "./mfind: ");
      fflush(stderr);
      perror(parentDirectoryName);
  }

  char *copy = alloca((strlen(parentDirectoryName) + 1));
  strcpy(copy,parentDirectoryName);

  char parentDirectoryName_onlyName[1024]; //result here
  char *ch; //define this
  ch = strtok(copy, "/"); //first split
  while (ch != NULL) {
    strcpy(parentDirectoryName_onlyName, ch);//copy result
    ch = strtok(NULL, "/");//next split
  }

  if ((((bufferStartFolder.st_mode & S_IFMT) == S_IFDIR) == 1)){
    if((strcmp(parentDirectoryName_onlyName, filenameGoal) == 0)){
      printf("%s\n", parentDirectoryName);
    }
  }
  // -^^^--- Testing if startFolder is goal ---^^^-
}

int browseDirectory(void){


    char *parentDirectoryName = dequeueFromQueue();

    if (parentDirectoryName == NULL){
        return -1;
    }

    if (strlen(parentDirectoryName) == 0){ // Empty string from dequeue
        return -1;
    }

    DIR *currDirStream;
    struct dirent *dirEntry;

    if ((currDirStream = opendir(parentDirectoryName)) == NULL)
    {
        perror(parentDirectoryName);
        return -1;
    }


    //checkStartFoldersIfGoal(parentDirectoryName);


        while ((dirEntry = readdir(currDirStream)) != NULL)
        {

            // Ignore . and ..
            if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
            {

                // halfPath is only parentDirectory + "/"
                char *halfPath = buildFullFilePathconcat(parentDirectoryName, "/");

                // fullPath is full directory + filename
                char *fullPath = buildFullFilePathconcat(halfPath, dirEntry->d_name);


                struct stat buffer;
                if (lstat(fullPath, &buffer) < 0)
                {
                    fprintf(stderr, "./mfind: ");
                    fflush(stderr);
                    perror(dirEntry->d_name);
                }
                switch (buffer.st_mode & S_IFMT)
                {
                case S_IFREG:

                    // directories or links NOT specified
                    if (((strcmp(tvalue, "d")) != 0) && ((strcmp(tvalue, "l")) != 0))
                    {
                        if ((strcmp(dirEntry->d_name, filenameGoal) == 0))
                        {
                            printf("%s\n", fullPath);
                        }
                    }
                    break;
                case S_IFDIR:

                    if (((strcmp(tvalue, "d")) == 0) && (strstr(dirEntry->d_name, filenameGoal) != NULL))
                    {
                        printf("%s\n", fullPath);
                    }
                    else if (((strcmp(tvalue, "")) == 0 && (strstr(dirEntry->d_name, filenameGoal) != NULL)))
                    { // Nothing specified
                        printf("%s\n", fullPath);
                    }
                    addDirectoryToQueue(fullPath);
                break;
            case S_IFLNK:
                if (((strcmp(tvalue, "l")) == 0) && (strstr(dirEntry->d_name, filenameGoal) != NULL)){
                    printf("%s\n", fullPath);
                }
                else if (((strcmp(tvalue, "")) == 0 && (strstr(dirEntry->d_name, filenameGoal) != NULL)))
                { // Nothing specified
                    printf("%s\n", fullPath);
                }
                break;
            default:
                printf("Found \"Default\"\n");
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

    printf("\n");

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
                return -1;
                }
            default:
                return -1;
        }
    }

    if(argc == 1){
        fprintf(stderr,"Empty argv!\n");
        return -1;
    }else if(argc < 3){
        fprintf(stderr,"Invalid call. Example call: ./mfind . godis.txt\n");
        return -1;
    }

    // Check for valid type flag/value
    if(tflag == 1){
      // Not directory
      if((strcmp(tvalue, "d") != 0)){
        // Not ordinary file
        if((strcmp(tvalue, "f") != 0)){
          // if not link
          if((strcmp(tvalue, "l") != 0)){
            fprintf(stderr,"Invalid filetype\n");
            return -1;
          }
        }
      }
    }

    // Check length of pvalue
    if(pflag == 1){
      if(atoi(pvalue) == 0){
        fprintf(stderr,"Invalid -p value. Positive number is required.\n");
        return -1;
      }
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
        checkStartFoldersIfGoal(argv[i]);
    }

    numberOfThreads = atoi(pvalue);

    // If not given -> mainthread #1
    if(numberOfThreads == 0){
        numberOfThreads++;
    }

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&condition, NULL);

    // Creating threads
    //pthread_t threadArray[numberOfThreads];

    threadArray[0] = pthread_self();
    for (int i = 1; i < numberOfThreads; i++){
        pthread_t newThread = pthread_self();
        threadArray[i] = newThread;
    }

    for(int i = 1; i<numberOfThreads;i++){
        if (pthread_create(&threadArray[i], NULL, &goThreadGo, NULL)){
            fprintf(stderr, "Couldn't create thread\n");
            exit(1);
        }
    }

    goThreadGo(NULL);

    // Joining threads
    lastThreadFinished();

    return 0;
}

void lastThreadFinished(void){

    for (int i = 0; i < numberOfThreads; i++){
        if (pthread_equal(threadArray[i], pthread_self()) == 0){
            pthread_join(threadArray[i], NULL);
        }
    }

    Queue_free(toBeVisitedQueue); // CALL ONLY ONCE

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
        pthread_mutex_unlock(&mtx);
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

void printThreadWork(int callsToOpenDir){
  printf("Thread: %08x Reads: %d\n", (int)pthread_self(), callsToOpenDir);
}

// Function to be called with threads -> Runs search algorithm
void *goThreadGo(void *arg){

    arg++;

    int callsToOpenDir = 0;

    while(true){
        pthread_mutex_lock(&mtx);
        while (lengthOfQueue == 0 && lastThreadDone == 0){

            // Only main-thread
            if (numberOfThreads == 1){

                printThreadWork(callsToOpenDir);
                return NULL;

            } else if(threadsWaiting == numberOfThreads-1){
                lastThreadDone = 1;

                printf("\n");

                while(threadsWaiting != 0){
                    pthread_mutex_unlock(&mtx);
                    pthread_cond_broadcast(&condition);
                }

                printThreadWork(callsToOpenDir);

                return NULL;
            }else{
                threadsWaiting++;
                pthread_cond_wait(&condition, &mtx);
                threadsWaiting--;

                // Condwaita

                if(lastThreadDone == 1){
                    printThreadWork(callsToOpenDir);
                    return NULL;
                }

            }

        }

        pthread_mutex_unlock(&mtx);

        browseDirectory();
        callsToOpenDir++;

        // Wake up threads to check if work is to be done
        pthread_cond_broadcast(&condition);

    }
    return 0;
}
