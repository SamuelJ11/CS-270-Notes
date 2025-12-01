#include "ringofprocs.h"
#include <string.h>

volatile sig_atomic_t IAMLEADER = 0;
volatile sig_atomic_t sig_received = 0;
int numcycles = -1;
long cycle_value = 0;
char buf[5];

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

void mysighandler(int signal)
{
    sig_received = 1;
}

typedef void (*sighandler_t)(int);

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
    // STEP 1: EXTRACT THE FILE DESCRIPTORS FROM *PARGV

    int input_fd = atoi(argv[READFDARG]);
    int output_fd = atoi(argv[WRITEFDARG]);
    int ready_fd = atoi(argv[READYFDARG]);

    // STEP 2: GET MY INDEX FROM THE ENVIRONMENT

    int myIndex = atoi(getenv(PROCNUMKEY));

    // STEP 3: DETERMINE IF LEADER AND INSTALL HANDLER
  
    char *numcycles_env = getenv(NUMCYCSKEY);
    if (numcycles_env != NULL) 
    {
        IAMLEADER = 1;             
        numcycles = atoi(numcycles_env); 

        Signal(SIGUSR1, mysighandler);
    } 
    else 
    {
        IAMLEADER = 0;             
    }    
    
    // STEP 4: INDICATE TO THE RING DRIVER THAT THIS PROCESS IS READY

    write(ready_fd, &myIndex, 1);
    close(ready_fd);

    // STEP 5: WAIT FOR SIGURS1 TO BE DELIVERED, PROMPT USER FOR CYCLE VALUE

    char* value = NULL;
    if (IAMLEADER)
    {
        sigset_t mymask, mtmask, oldmask;
        sigfillset(&mymask); sigemptyset(&mtmask);
        if (sigprocmask(SIG_BLOCK, &mymask, &oldmask) < 0)
        {
            unix_error("sigprocmask error: ");
        }

        while (sig_received == 0)
        {
            // unblock signals and pause this thread
            sigsuspend(&mtmask); 
            /* signal arrived - restore previous signal mask */
            if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
            {
                unix_error("sigprocmask error: ");
            }
        }

        printf("Please enter an integer in [1,9999] to circulate: ");
        value = fgets(buf, sizeof(buf), stdin);
        if (value == NULL)
        {
            printf("no input recieved, exiting.\n");
            exit(0);
        }
        cycle_value = atoi(value);
    }

    // STEP 6: WHILE LOOPING, READ A LONG FROM THE INPUT PIPE FILE DESCRIPTOR

    long cycle_iterator = 0;
    long check;
    if (IAMLEADER) 
    {
        write(output_fd, &cycle_value, sizeof(long));  // start the ring
    }

    while(1)
    {
        // increment the value read from the predecessor 
        check = read(input_fd, &cycle_value, sizeof(long));    
        if (check == 0)
        {
            close(output_fd);
            exit(0);
        }
        // report my value and that it was received
        report(myIndex, cycle_value);

        if (IAMLEADER)
        {
            cycle_iterator++;
            if (cycle_iterator > numcycles)
            {
                close(output_fd);
                exit(0);
            }            
        }

        cycle_value++;
        write(output_fd, &cycle_value, sizeof(long));
    }

    return 0;
}