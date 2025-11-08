#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {

    int x = 1;
    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("fork failed");
    } else if (pid == 0)    // this block executes only in the child process
    { 
        x--;
        printf("Child: My PID = %d, my parent's PID = %d, x=%d\n", getpid(), getppid(), x);
    } else                  // this block executes only in the parent process
    { 
        x++;
        printf("Parent: My PID = %d, my child's PID = %d, x=%d\n", getpid(), pid, x);
    }

    return 0;
}

