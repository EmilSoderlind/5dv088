#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

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

int browseDirectory(char *name){
    DIR *curr_dir;
    struct dirent *dir_pointer;

    if ((curr_dir = opendir(name)) == NULL)
    {
        perror("testDir");
        fprintf(stderr,
                "Error: Unable to open dir '%s' on line %d\n",
                name, __LINE__);
        return -1;
    }
    else
    {
        fprintf(stdout, "Directory '%s' is open\n", name);
    }

    /* Repeat for all files in the open dir */
    while ((dir_pointer = readdir(curr_dir)) != NULL){
        fprintf(stdout, "%s\n", dir_pointer->d_name);


        struct stat buffer;
        if(lstat(name, &buffer) < 0){
            perror(name);
        }
        printf("%d\n",buffer.st_mode);

        if (S_ISDIR(buffer.st_mode)){
            puts("|| directory");
        }

        printf("\n");
    }

    return 0;
}

int main(void)
{
    arr[0] = 0;
    printf("SET arr[0] <- %d\n", 0);
    
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
    
    
    browseDirectory("testDir");

    putchar('\n');

    return 0;
}
