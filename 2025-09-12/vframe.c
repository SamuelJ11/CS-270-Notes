#include <stdio.h>
#include <stdint.h>

long vframe(long n, long idx, long *q) 
{
    long i;
    long *p[n];
    p[0] = &i;
    for (i = 1; i < n; i++)
        p[i] = q;
    return *p[idx];
}

int main() 
{
    long n_size = 5;
    long index = 1;
    long q_value = 100;
    long *q_ptr = &q_value;

    long result = vframe(n_size, index, q_ptr);

    printf("The returned value is: %ld\n", result);

    return 0;
}