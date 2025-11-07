#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("fork failed");
    } else if (pid == 0)    // this block executes only in the child process
    { 
        printf("Child: My PID = %d, my parent's PID = %d\n", getpid(), getppid());
    } else                  // this block executes only in the parent process
    { 
        printf("Parent: My PID = %d, my child's PID = %d\n", getpid(), pid);
    }

    return 0;
}

