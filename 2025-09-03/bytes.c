#include <stdio.h>

/*
  byte_pointer is a pointer to an unsigned char, which represents a single byte.
  x is a float, so &x gives the address of the entire float variable in memory
  (not just one byte).

  By writing (byte_pointer)&x, we are:
    1. Taking the address of x with &x
    2. Casting that address to type unsigned char* (byte_pointer)

  This cast allows show_bytes to treat the memory occupied by x as a sequence
  of individual bytes, without changing the actual address. Each byte of the 
  float can then be accessed one at a time using array indexing (start[0], 
  start[1], etc.), allowing us to print the byte representation of the float.
*/


typedef unsigned char* byte_pointer;                // typedef followed by an existing type and a new name means the new name can 
                                                      //now be used as a type to declare variables of the original type
void show_bytes(byte_pointer start, size_t len)     // size_t is a data type (an unsigned integer type) that safely stores the size (in bytes) of some object or memory block.
{
    int i;
    for (i = 0; i < len; i++)
        printf("%.2x ", start[i]);
    printf("\n");
}

void show_int(int x) 
{
    show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x) 
{
    show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void* x) 
{
    show_bytes((byte_pointer)&x, sizeof(void*));    // size_of returns the size of a data a type in bytes.
}

void test_show_bytes(int val) 
{
    int ival = val;
    float fval = (float)ival;
    int *pval = &ival;
    printf("1: ");
    show_int(ival);
    printf("2: ");
    show_float(fval);
    printf("3: ");
    show_pointer(pval);
}

int main()
{
    printf("Now showing: (1) byte representation of int x, (2) byte representation of float x, (3) byte representation of int* x \n");

    test_show_bytes(12345);

    printf("\nNow we can try to print the bytes of a string using the show_bytes routine: \n");
    char *str = "12345";
    show_bytes((byte_pointer)str, 6);

    return 0;
}
