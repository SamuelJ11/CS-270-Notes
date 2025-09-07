#include <stdio.h>


void inplace_swap(int *x, int *y)
{
    // initially a(*x) = 0101, b(*y) = 1010
    *y = *x ^ *y;  // Step 1:  b = 0101 ^ 1010 = 1111
    *x = *x ^ *y;  // Step 2:  a = 0101 ^ 1111 = 1010
    *y = *x ^ *y;  // Step 3:  b = 1010 ^ 1111 = 0101
}

int main()
{
    int a = 5;
    int b = 10;

    printf("Before the swap: a = %u, b = %u \n", a, b);

    inplace_swap(&a, &b);

    printf("After the swap: a = %u, b = %u \n", a, b);

    return 0;
}