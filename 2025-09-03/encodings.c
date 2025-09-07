#include <stdio.h>
#include <limits.h>

typedef unsigned char* byte_pointer;                // typedef followed by an existing type and a new name means the new name can 
                                                      //now be used as a type to declare variables of the original type
void show_bytes(byte_pointer start, size_t len)     // size_t is a data type (an unsigned integer type) that safely stores the size (in bytes) of some object or memory block.
{
    int i;
    for (i = 0; i < len; i++)
        printf("%x ", start[i]); 
    printf("\n");
}

int main()
{
                // NEGATION

    short x = 12345;
    short mx = -x;

    show_bytes((byte_pointer)&x, sizeof(short));
    show_bytes((byte_pointer)&mx, sizeof(short));

                // CASTING

    printf("\n");

    short int v = -12345;
    unsigned short uv = (unsigned short)v;

    printf("v: %d\nuv: %u\n", v, uv);

    int w = -1;
    unsigned int uw = (unsigned int)w;

    printf("\n");

                // SIGNED VS UNSIGNED INTERPRETATION.

    printf("w: %d\nuw: %u\n", w, uw);

    int x2 = -1; // 11111111 11111111 11111111 11111111 
    unsigned u2 = 2147483661; // 10000000 00000000 00000000 00001101

    printf("\n");

    printf("x2 = unsigned: %u = signed: %d\n", x2, x2);
    printf("u2 = unsigned: %u = signed: %d\n\n", u2, u2);

                // SIGN EXTENSION

    short sX = -12345;             // -12345 
    unsigned short usX = sX;       // 53191 
    int X = sX;                    // -12345 
    unsigned uX = usX;             // 53191 
          
    printf("sX = %d:\t", sX);
    show_bytes((byte_pointer) &sX, sizeof(short));

    printf("usX = %u:\t", usX);
    show_bytes((byte_pointer) &usX, sizeof(unsigned short));

    printf("X = %d:\t", X);
    show_bytes((byte_pointer) &X, sizeof(int));

    printf("\n");

                // TRUNCATING NUMBERS

    int x3 = 53191;
    short sx3 = (short)x3; 
    int y2 = sx3; 

    printf("x3 = %d\n", x3); 
    printf("sx3 = %d\n", sx3);
    printf("y2 = %d\n", y2);

    return 0;
}

