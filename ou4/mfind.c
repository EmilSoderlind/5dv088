
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



/* Concatinate String 1 and String 2 and return the result
 *  Arguments:
 *  s1        String 1
 *  s2        String 2
 *  Returns:  String 1 and String 2 concatinated
 */
char *buildFullFilePathconcat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 3); // +1 for the null-terminator
    // Check malloc for NULL
    if(result == NULL){
      fprintf(stderr, "Could not allocate memory.\n");
      exit(-1);
    }

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/* Check if StartDirectoryName is the searched goal
 *  Arguments:
 *  parentDirectoryName directoryName to check
 */
void checkStartFoldersIfGoal(char* parentDirectoryName){

  struct stat bufferStartFolder;
  if (lstat(parentDirectoryName, &bufferStartFolder) < 0){
      fprintf(stderr, "./mfind: ");
      fflush(stderr);
      perror(parentDirectoryName);
  }

  char *copy = alloca((strlen(parentDirectoryName) + 1));
  // Check alloca for Null
  if(copy == NULL){
    fprintf(stderr, "Could not allocate memory on stack.\n");
    exit(-1);
  }

  strcpy(copy,parentDirectoryName);

  char parentDirectoryName_onlyName[1024]; //result here
  char *ch; //define this
  ch = strtok(copy, "/"); //first split
  while (ch != NULL) {
    strcpy(parentDirectoryName_onlyName, ch);//copy result
    ch = strtok(NULL, "/");//next split
  }

  if ((((bufferStartFolder.st_mode & S_IFMT) == S_IFDIR) == 1)){ // Start is direcotry
    if(((strcmp(tvalue, "f")) != 0)){ // Searching for directory
      if((strcmp(parentDirectoryName_onlyName, filenameGoal) == 0)){ // Names match
        printf("%s\n", parentDirectoryName);
      }
    }
  }
}

/* Dequeue directorie from queue and search through entires for searched file
 * return 0 if success
 */
int searchDirectory(void){

    char *parentDirectoryName = dequeueFromQueue();

    if (parentDirectoryName == NULL){
        return -1;
    }

    if (strlen(parentDirectoryName) == 0){ // Empty string from dequeue
      free(parentDirectoryName);
      return -1;
    }

    DIR *currDirStream;
    struct dirent *dirEntry;

    if ((currDirStream = opendir(parentDirectoryName)) == NULL){
        free(parentDirectoryName);
        perror(parentDirectoryName);
        return -1;
    }

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
                //printf("Found \"Default\"\n");
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

/*  Called by Main-thread to wait for threads to exit before returning in main
*/
void joinThreads(void){
    for (int i = 0; i < numberOfThreads; i++){
        if (pthread_equal(threadArray[i], pthread_self()) == 0){
            if(pthread_join(threadArray[i], NULL) != 0){
              perror("phtread_join");
              exit(-1);
            }
        }
    }

    Queue_free(toBeVisitedQueue); // CALL ONLY ONCE
}

/*  Enqueue-ing directorie to queue
 *  Underlying wrapper-function around queue-datatype to make it threadsafe
 *  Arguments:
 *  name    String to be added to queue
 */
void enqueueCharToQueue(char *name){
    //Node *start_dir = Node_init((sizeof(name)*2));
    Node *start_dir = Node_create();
    char *stringToBeEnqueued = (char *)malloc((strlen(name) + 1) * sizeof(char));

    // check malloc for NULL
    if(stringToBeEnqueued == NULL){
      fprintf(stderr, "Could not allocate memory.\n");
      exit(-1);
    }

    strcpy(stringToBeEnqueued, name);
    start_dir->name = stringToBeEnqueued;
    Enqueue(toBeVisitedQueue, start_dir);
}


/*  Enqueue-ing directorie to queue
 *  Wrapper-function around queue-datatype to make it threadsafe
 *  Arguments:
 *  newDir  String to be added to queue
 */
void addDirectoryToQueue(char *newDir){

  int error = pthread_mutex_lock(&mtx);
    if(error != 0){
      printf("Got errorcode: %d\n", error);
      perror("pthread_mutex_lock");
      exit(-1);
    }

    enqueueCharToQueue(newDir);
    lengthOfQueue++;

    if(pthread_mutex_unlock(&mtx) != 0){
      perror("pthread_mutex_unlock");
      exit(-1);
    }
}

/*  Dequeue-ing directorie from queue
 *  Wrapper-function around queue-datatype to make it threadsafe
 *  Return: Dequeue:d string
 */
char* dequeueFromQueue(void){

  if(pthread_mutex_lock(&mtx) != 0){
    perror("pthread_mutex_lock");
    exit(-1);
  }

    if(lengthOfQueue == 0){
      if(pthread_mutex_unlock(&mtx) != 0){
        perror("pthread_mutex_unlock");
        exit(-1);
      }
      return NULL;
    }

    // Make threadsafe
    Node *tempNode = Dequeue(toBeVisitedQueue);
    char *tempName = tempNode->name;

    free(tempNode);

    lengthOfQueue--;

    if(pthread_mutex_unlock(&mtx) != 0){
      perror("pthread_mutex_unlock");
      exit(-1);
    }

    return tempName;
}


/*  Called by threads to print their thread-id and numberOfDirectories they've opened.
 *  Arguments:
 *  callsToOpenDir  Number of opened directories
 */
void printThreadWork(int callsToOpenDir){
  printf("Thread: %08x Reads: %d\n", (int)pthread_self(), callsToOpenDir);
}


/*  Function called by threads to start the search algorithm
 *  Arguments:
 *  arg      Null-argument
 */
void *threadLoop(void *arg){

    arg++;

    int callsToOpenDir = 0;

    while(true){

      if(pthread_mutex_lock(&mtx) != 0){
        perror("pthread_mutex_lock");
        exit(-1);
      }

      if (lengthOfQueue == 0 && lastThreadDone == 0){

            // Only main-thread
            if (numberOfThreads == 1){

                printThreadWork(callsToOpenDir);
                return NULL;

            } else if(threadsWaiting == numberOfThreads-1){
                lastThreadDone = 1;

                printf("\n");

                while(threadsWaiting != 0){

                    if(pthread_mutex_unlock(&mtx) != 0){
                      perror("pthread_mutex_unlock");
                      exit(-1);
                    }
                    if(pthread_cond_broadcast(&condition) != 0){
                      perror("pthread_cond_broadcast");
                      exit(-1);
                    }
                }

                printThreadWork(callsToOpenDir);

                return NULL;
            }else{

                threadsWaiting++;

                // Cond-wait
                if(pthread_cond_wait(&condition, &mtx) != 0){
                  perror("pthread_cond_wait");
                  exit(-1);
                }

                threadsWaiting--;

                if(lastThreadDone == 1){
                    printThreadWork(callsToOpenDir);
                    return NULL;
                }
            }
        }

        if(pthread_mutex_unlock(&mtx) != 0){
          perror("pthread_mutex_unlock");
          exit(-1);
        }

        searchDirectory();
        callsToOpenDir++;

        // Wake up threads to check if work is to be done
        if(pthread_cond_broadcast(&condition) != 0){
          perror("pthread_cond_broadcast");
          exit(-1);
        }

    }
    return 0;
}


/*  Search after filename (name) of type (type) in filepath:s (start1...) in number of threads (nrthr)
 *  Example call: mfind [-t type] [-p nrthr] start1 [start2 ...] name
 *  Arguments:
 *  argc      Number of arguments
 *  argv      String-array of arguments
 *  Returns:  0 if sucess
 */
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

    // Check input arguments
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

    // Creating mutex / cond
    if(pthread_mutex_init(&mtx, NULL) != 0){
      perror("pthread_mutex_init");
      exit(-1);
    }

    if(pthread_cond_init(&condition, NULL) != 0){
      perror("pthread_cond_init");
      exit(-1);
    }

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

    // Creating threads
    threadArray[0] = pthread_self();
    for (int i = 1; i < numberOfThreads; i++){
        pthread_t newThread = pthread_self();
        threadArray[i] = newThread;
    }

    for(int i = 1; i<numberOfThreads;i++){
        if (pthread_create(&threadArray[i], NULL, &threadLoop, NULL)){
            fprintf(stderr, "Couldn't create thread\n");
            exit(-1);
        }
    }

    threadLoop(NULL);

    joinThreads();

    return 0;
}
