#include <stdio.h>

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

int main() 
{
    printf("%d\n", func4(3, 0, 14));
}