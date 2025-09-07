#include <stdio.h>

typedef unsigned char* byte_pointer;                // typedef followed by an existing type and a new name means the new name can 
                                                      //now be used as a type to declare variables of the original type
void show_bytes(byte_pointer start, size_t len)     // size_t is a data type (an unsigned integer type) that safely stores the size (in bytes) of some object or memory block.
{
    int i;
    for (i = 0; i < len; i++)
        printf("%.2x ", start[i]);
    printf("\n");
}

int main()
{
    short x = 12345;
    short mx = -x;

    show_bytes((byte_pointer)&x, sizeof(short));
    show_bytes((byte_pointer)&mx, sizeof(short));

    return 0;
}

