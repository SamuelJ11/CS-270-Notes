#include <stdio.h>
#include <stdint.h>

//Problem 0:1
/*
int foo(int *x, int y, int z) 
{
    int retval = 0;
    int *xptr = x;
    z = 0;

    while (y > retval) 
    {
        z += *xptr;
        retval++;
        xptr++;
    }

    retval = z;
    return retval;
}
*/

//Problem 0:2
/*
int foo(int *x)
{
    int retval = *x;
    if (retval <= 10)
    {
        retval = (3 * retval) + 1; 
        *x = retval;
        return retval;
    }
    retval = retval - 10;
    *x = retval;    
    return retval;
}
*/

/*
//Problem 0:3
int foo(char *rdi) 
{
    int retval = 0;

    for (;*rdi++ !=0 && retval <= 19;)
    {
        retval++;
    }

    return retval;
}
*/

//Problem 0:4
int foo(int edi, int esi)
{
    int retval = 0;
    int ecx = retval;
    while (retval <= 31)
    {
        if (!(1 & (esi ^ edi)))
        {
            ecx = ecx + 1;
        }

        edi = edi >> 1;
        esi = esi >> 1;
        retval = retval + 1;
    }

    retval = ecx;
    return retval;
}
