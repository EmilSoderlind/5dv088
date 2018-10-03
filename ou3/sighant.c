
#include "sighant.h"

void catchSignal(int theSignal)
{
    if (theSignal == SIGINT)
    {

        fprintf(stderr, "Recieved signal %d!\n", theSignal);

        // Kill children
        for (int i = 0; i < (int)NR_OF_CHILDREN; i++){
            fprintf(stderr, "Killing child %d (%d)\n", i, PID_CHILDREN_ARRAY[i]);

            if (kill(PID_CHILDREN_ARRAY[i], SIGINT) < 0){
                fprintf(stderr, "Error killing child!\n");
            }
        }

        fflush(stderr);

        NR_OF_CHILDREN = 0;

        if (signal(SIGINT, catchSignal) == SIG_ERR){
            fprintf(stderr, "Couldn't register signal handler\n");
            exit(1);
        }

        loopRunShell();
    
    }else{
        fprintf(stdout, "Recieved %d-sig\n",theSignal);
        fprintf(stdout, "[Mish-process completed]\n");

         exit(0);
    }
}