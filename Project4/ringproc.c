#include "ringofprocs.h"

volatile IAMLEADER = 0;
volatile sig_atomic_t sig_received = 0;
int numcycles = -1;

void mysighandler(int sig)
{
    sig_received = 1;
}

int main(int argc, char *argv[]) 
{

    // extract the filedescriptors from *pargv
    int input_fd = atoi(argv[READFDARG]);
    int output_fd = atoi(argv[WRITEFDARG]);
    int ready_fd = atoi(argv[READYFDARG]);

    // extract the ring index from the environment variable (PROCNUMKEY defined as "childindx")
    int myIndex = atoi(getenv(PROCNUMKEY));

    // determine if I am the leader
    char *numcycles_env = getenv(NUMCYCSKEY);
    if (numcycles_env != NULL) 
    {
        IAMLEADER = 1;             
        numcycles = atoi(numcycles_env); 
    } 
    else 
    {
        IAMLEADER = 0;             
    }    

    // install signal handler if I am the leader
    if (IAMLEADER == 1)
    {
        struct sigaction act;
        act.sa_handler = mysighandler;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask); // Don't block other signals while in the handler

        if (sigaction(SIGUSR1, &act, NULL) == -1) 
        {
            perror("sigaction");
            exit(1);
        }
    }       
}