#include "ringofprocs.h"
#include <string.h>

volatile sig_atomic_t IAMLEADER = 0;
volatile sig_atomic_t sig_received = 0;
int numcycles = -1;

// implement a function to report fatal system-level errors and exit
void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

// create the actual signal handler
void mysighandler(int signal)
{
    sig_received = 1;
}

// define a new type called 'sighandler_t': this new type is an alias for a pointer to a function that takes an int and returns void
typedef void (*sighandler_t)(int);

// create a wrapper function called Signal() that calls sigaction() for us
void Signal(int signum, sighandler_t handler)
{
    struct sigaction new_action;
    new_action.sa_handler = handler;        // assigns the function pointer passed to the wrapper to the handler field
    sigfillset(&new_action.sa_mask);        // initializes the set to include every possible signal (all bits are 1)
    new_action.sa_flags = SA_RESTART;       // ensures interrupted slow syscalls like open(), read(), write(), etc restart automatically

    if (sigaction(signum, &new_action, NULL) < 0)
    {
        unix_error("Signal error");
    }
}

int main(int argc, char *argv[]) 
{
    // extract the file descriptors from *pargv
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
        // install the handler
        Signal(SIGUSR1, mysighandler);
    }       
}