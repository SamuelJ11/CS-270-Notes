#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Thread routine
void *thread(void *vargp)
{
    pthread_t self = pthread_self();
    printf("Hello, world! from thread %lu\n", self);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <num threads> \n",argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    pthread_t t_ids[n];

    // Create all threads
    for (int i = 0; i < n; i++) {
        pthread_create(&t_ids[i], NULL, thread, NULL);
    }

    // Join all threads
    for (int i = 0; i < n; i++) {
        pthread_join(t_ids[i], NULL);
    }
}