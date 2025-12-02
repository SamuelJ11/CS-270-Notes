#include "ringofprocs.h"
#include <string.h>

volatile sig_atomic_t IAMLEADER = 0;
volatile sig_atomic_t sig_received = 0;

// create a type alias named sighandler_t to simplify call to Signal()
typedef void (*sighandler_t)(int);

// wrapper function for reporting system errors
void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

// simple atomic signal handler
void mysighandler(int signal)
{
    sig_received = 1;
}

// implement a wrapper function called Signal() that handles the unwiedly sigaction() function call
void Signal(int signum, sighandler_t handler)
{
    struct sigaction new_action;
    new_action.sa_handler = handler;        // assigns the function pointer passed to the wrapper to the handler field
    sigfillset(&new_action.sa_mask);        // initializes the set to include every possible signal (all bits are 1)
    new_action.sa_flags = SA_RESTART;       // ensures interrupted slow syscalls like open(), read(), write(), etc restart automatically

    if (sigaction(signum, &new_action, NULL) < 0)
    {
        unix_error("sigaction error");
    }
}

int main(int argc, char *argv[]) 
{
    long numcycles = 0, cycle_value = 0;

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
        // installing the handler is simple now - Signal() handles error checking
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

    if (IAMLEADER)
    {
        char buf[5];    // 4-character buffer for cycle_value (plus 1 for null-terminator)        
        sigset_t mymask, mtmask, oldmask;
        sigfillset(&mymask); sigemptyset(&mtmask);

        // block all signals in mymask, and save the previously blocked signals in oldmask
        if (sigprocmask(SIG_BLOCK, &mymask, &oldmask) < 0)
        {
            unix_error("sigprocmask error");
        }

        while (sig_received == 0)
        {
            /*  sigsuspend() does 3 things here:
                (1) temporarily replaces the current blocked set with the provided "mt" set of blocked signals 
                (2) suspends the process until a signal arrives whose action is either to run a handler or terminate the process
                (3) returns AFTER THE HANDLER RETURNS and restores the old blocked set (mymask) automatically
            */
            sigsuspend(&mtmask); 

            // restore previously blocked signals stored in oldmask
            if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
            {
                unix_error("sigprocmask error");
            }
        }

        printf("Please enter an integer in [1,9999] to circulate: ");
        fgets(buf, sizeof(buf), stdin);
        if (buf[0] == '\n')
        {
            printf("Warning: no input recieved for cycle value, exiting.\n");
            exit(0);
        }
        cycle_value = atoi(buf);
    }

    // STEP 6: WHILE LOOPING, READ A LONG FROM THE INPUT PIPE FILE DESCRIPTOR

    long cycle_iterator = 0, inputpipe_open = 0;

    // this code starts the ring
    if (IAMLEADER) 
    {
        write(output_fd, &cycle_value, sizeof(long));
    }

    while(1)
    {
        inputpipe_open = read(input_fd, &cycle_value, sizeof(long)); 

        // if predecessor has closed its write end of the pipe, close output pipe and exit   
        if (inputpipe_open == 0)
        {
            close(output_fd);
            exit(0);
        }

        // report current process's index and received value
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

        // write the value of the incremented cycle_value to successor in the ring
        write(output_fd, &cycle_value, sizeof(long));
    }
}