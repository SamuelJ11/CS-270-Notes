#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;

    pid = fork();

    if (pid > 0) {          // parent only
        fork();
        printf("Hello\n");
    }

    printf("Hello\n");
    exit(0);
}
