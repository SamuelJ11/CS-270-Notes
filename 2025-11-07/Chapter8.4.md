# CHAPTER 8:  EXCEPTIONAL CONTROL FLOW

## Process Control

### Obtaining Process ID's

    • Each process has a unique positive (nonzero) process ID (PID), and the getpid function returns the PID of the calling process.

    • The getppid function returns the PID of its parent

        -  both getpid and getppid routines return an integer value of type pid_t (defined in types.h as 'int')

### Creating and Terminating Processes

    • A process is always in either 1 of 3 states:

        (1) running: process executes instructions or is scheduled by the kernel
        
        (2) stopped: process paused, waiting for signal

        (3) terminated: process is done and removed from memory

    • A parent process creates a new running child process by calling the fork function:

        #include <sys/types.h>
        #include <unistd.h>

        pid_t fork(void);

        - returns 0 to the child

        - returns PID of child to parent

        - returns -1 on error

    • See process.c for an example on how forl() and get(p)pid work:

        - the child sees PID = 0 (no matter how many children there are)

        - the parent always sees the PID of that child, and must use getpid() to see its own PID

        - By checking the return value, your program can unambiguously know which code path to execute for the parent vs the child

    • The parent and the child are separate processes that run concurrently

        - depending on the system, the parent or child process may terminate first

        - each process has the same user stack, local variable values, heap, global variable values and code, but they have private address spaces

            -- any subsequent changes after the fork that a parent or child makes to a variable are private and are not reflected in the memory of the other process

    • It is often helpful to sketch the process graph, which is a simple kind of precedence graph that captures the partial ordering of program statements. 
    
        - each vertex corresponds to the execution of a program statement
        
        - a directed edge a --> b denotes that statement 'a' happens before statement 'b'

        - each graph begins with a vertex that corresponds to the parent process calling main

        - the sequence of vertices for each process ends with a vertex corresponding to a call to exit

    • If there are multiple topological sorts possible for a given sequence of vertices, then the printf statemetns in the parent and child can occur in either order

        - see figure 8.17 on page 768 for an example of a program with nested forks

            -- this program runs four processes, each of which makes a call to `printf` and which can execute in any order

### Reaping Child Processes

    • When a process terminates for any reason, the kernel does not remove it from the system immediately:

        - the process is kept around in a terminated state until it is reaped by its parent

        - when the parent reaps the terminated child, the kernel passes the child's exit status to the parent and then discards the terminated process, at which point it ceases to exist
        
        - a terminated process that has not yet been reaped is called a 'zombie'

    • When a parent process terminates, the kernel arranges for the 'init' process to become the adopted parent of any orphaned children

        - the 'init' process, which has a PID of 1, is created by the kernel during system start-up, never terminates, and is the ancestor of every process

    • If a parent process terminates without reaping its zombie children, then the kernel arranges for the 'init' process to reap them.

    • It is important for zombie children to be reaped because they still consume system memory resources even though they are no longer running.
    
    • The waitpid() function is the mechanism used to transition a terminated child process from a zombie state to complete removal from the system, ensuring the parent correctly receives and processes the child's final status:

        #include <sys/types.h>
        #include <sys/wait.h>

        pid_t waitpid(pid_t pid, int *statusp, int options);

        - when options = 0, waitpid suspends execution of the calling process until a child process in its wait set terminates

        - if a process in the wait set has already terminated at the time of the call, then waitpid() returns immediately

            -- in either case, waitpid() returns the PID of the terminated child that caused waitpid() to return

    • In the waitpid() function, the members of the wait set are determined by the pid argument:

        - if PID > 0, the wait set is the singleton child process whose process ID is equal to PID

        - if PID = -1, then the wait set consists of all of the parent's child processes

    • In the waitpid() function, the default behavior can be modified by setting options to various combinations of the WNOHANG, WUNTRACED, and WCONTINUED constants:

        - WNOHANG: return immediately (with a return value of 0) if none of the child processes in the wait set has terminated yet. This option is useful in those cases where you want to continue doing useful work while waiting for a child to terminate

        - WUNTRACED: suspend execution of the calling process until a process in the wait set becomes either terminated or stopped. This option is useful wehn you want to check for both terminated and stopped children

        - WCONTINUED: suspend execution of the calling process until a running process in the wait set it terminated or until a stopped process has been resumed by recieving a SIGCONT signal

    • Options can also be combined by ORing them together:

        WNOHANG|WUNTRACED: return immediately, with a return value of 0, if none of the children in the wait set has stopped or terminated, or with a return value equal to the PID of one of the stopped or terminated children

    • If the 'statusp' argument is non-NULL, then waitpid() encodes status information about the child that caused the return in 'status', which is the value pointed to by 'statusp'

        - The 'wait.h' include file defines several macros for interpreting the 'status' argument, see page 771 for a list of those macros

    • When a system call like waitpid() fails, it returns a specific error value (often -1) and sets the global variable errno to a code that indicates the exact cause of the failure

        - if the process calling waitpid() has no existing child processes left to wait for, it returns -1 and errno is set to ECHILD

        - if the waitpid() function was interrupted by a signal, then it returns −1 and sets errno to EINTR

    • The wait() function is a convenience wrapper around the more versatile waitpid() function

        - a parent simply needs to stop and wait until any one of its children terminates

        - wait(&status) = waitpid(-1, &status, 0)

    • See the code snipped on page 773 to understand the following description
    
        - after the for-loop finishes, only the parent reaches the while loop

        - waitpid(-1, &status, 0) means “wait for any child process to change state; block until one does"

        - As each child terminates, the call to waitpid() returns with the non-zero PID of that child

        - the parent can call WIFEXITED(status) to test whether the child terminated normally and WEXITSTATUS(status) to get the value passed to exit() (e.g., 100 + i)

        - When there are no more child processes to wait for, waitpid(-1, &status, 0) returns -1 and sets errno to ECHILD

        - the if (errno != ECHILD) check ensures the -1 was caused by “no more children” (normal) and not some other error

    • See figure 8.19 on page 744 on how to eliminate non-determinism in the output order by reaping the children in the same order that they were created

        - pid_t pid[N], retpid: this creates an array to hold the process IDs of each child that the parent creates

            -- pid[i] will store the PID of the i-th child

            -- retpid will store the PID returned later by waitpid()

        - what waitpid(pid[i++], ...) means:

            -- waitpid() lets the parent wait for a specific child, not just any child

            -- in this case, pid[i] is that particular child in order

            -- thus, the parent reaps children in the same order they were created

### Putting Processes to Sleep

    • The sleep() function suspends a process for a specified period of time.

        - returns zero if the requested amount of time has elapsed, and the number of seconds still left to sleep otherwise

    • another function that we will find useful is the pause() function, which puts the calling process to sleep until a signal is received

        - always returns -1

### Loading and Running Programs

    • the execve() function loads and runs a new program in the context of the current process.

        - "in the context of the new current process" means the process ID (PID) stays the same, but everything inside (the code, stack, heap, data, etc.) is replaced by the new program’s code and data

            int execve(const char *filename, char *const argv[], char *const envp[]);

        - the execve() function loads and runs the executable object file 'filename' with the argument list 'argv' and the environment variable list 'envp'

        - called once and never returns, except if there is an error (-1)

    • In the execve() function, the argv variable points to a null-terminated array of pointers, each of which points to an argument string.

        - by convention, argv[0] is the name of the executable object file, and each entry points to a C string 

                argv → ┌────────────┐
                    │   ─────────┼──► "ls\0"
                    │   ─────────┼──► "-l\0"
                    │   ─────────┼──► "/home/samuel\0"
                    │   NULL     │
                    └────────────┘

        - 'envp' is a pointer to an array of strings. Each string represents one environment variable in your process

            -- each element in the array is a string of the form "name = value"  