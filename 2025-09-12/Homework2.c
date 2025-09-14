#include <stdio.h>
#include <stdint.h>


#define TYPEOF(x) _Generic((x), \
    int: "int", \
    unsigned: "unsigned", \
    char: "char", \
    unsigned char: "unsigned char", \
    short: "short", \
    long: "long", \
    default: "other")

int main()
{
    // define variables

    int m;
    unsigned n;
    unsigned char a, b;
    char z;
    short x;
    long q;
    
    // print output

    printf("The type of z+2 is %s\n", TYPEOF(z+2));
    printf("The type of q + a - b is %s\n", TYPEOF(q + a - b));
    printf("The type of (x ^ 0xff) + 3 is %s\n", TYPEOF((x ^ 0xff) + 3));
    printf("The type of n + 3000000000U is %s\n", TYPEOF(n + 3000000000U));
    printf("The type of m + n is %s\n", TYPEOF(m + n));
    printf("The type of a + b is %s\n", TYPEOF(a + b));
    printf("The type of x + 32762 is %s\n", TYPEOF(x + 32762));

    return 0;
}