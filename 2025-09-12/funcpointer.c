#include <stdio.h>
#include <stdint.h>

#include <stdio.h>

int fun(int x, int *p)
{
    *p = x; // store x into the memory pointed by p
    return *p;
}

int main()
{
    int (*fp)(int, int *); // pointer to function returning void
    fp = fun;

    int x = 12345;
    int y_value = 54321;
    int *y = &y_value;

    int result = fp(x, y); // call function through pointer
    printf("The value of y after function call is %d\n", result);

    return 0;
}
