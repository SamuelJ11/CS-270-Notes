#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//Phase 4
int func4(int edi, int esi, int edx)
{
    int ebx, eax;
    eax = edx - esi;

    ebx = ((unsigned int)eax >> 31) + eax; 
    ebx = ebx >> 1;                        
    ebx = ebx + esi;

    if (ebx > edi)
    {
        edx = ebx - 1;
        eax = func4(edi, esi, edx); 
        ebx = ebx + eax; 
    }
    else if (ebx < edi)
    {
        esi = ebx + 1;
        eax = func4(edi, esi, edx);  
        ebx = ebx + eax;
    }

    eax = ebx;
    return eax;
}

//Phase 5
char* arrayfunc(char* origstring, int size)
{    
    int index = 0;
    char mysterystring[] = "adguerniotvbyhmcSo you think you can stop the bomb with ctrl-c, do you?";
    char* retval = malloc(size + 1); // allocate space for result

    do
    {      
        char origchar = origstring[index];
        origchar = origchar & 0xf;
        retval[index] = mysterystring[origchar];  
        index++;
    } while (index != size);    

    retval[size] = '\0';
    return retval;
}

int main() 
{
    printf("the value of eax is %d\n", func4(3, 0, 14));  
    printf("the value of your string is %s\n", arrayfunc("orange", 6));
}


