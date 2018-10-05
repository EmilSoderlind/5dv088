
#include "sighant.h"

void catchSignal(int theSignal)
{
    if (theSignal == SIGINT)
    {

        //fprintf(stderr, "Recieved signal %d!\n", theSignal);

        //killChildren(theSignal);

        fflush(stderr);

        if (signalHand(SIGINT, catchSignal) == SIG_ERR){
            //fprintf(stderr, "Couldn't register signal handler\n");
            exit(1);
        }

        //fprintf(stderr, "Running loopRunShell\n");
        //loopRunShell();

    }else{
        //fprintf(stdout, "Recieved %d-sig\n",theSignal);
        //fprintf(stdout, "[Mish-process completed]\n");

        exit(0);
    }
}

Sigfunc *signalHand(int signo, Sigfunc *func)
{
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return (SIG_ERR);
    return (oact.sa_handler);
}