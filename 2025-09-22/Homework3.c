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
/*
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
*/

/*
bazz:
cmpq $2, %rsi        // 3 > 2           // 2 > 2 !!
jg .L2
movq (%rdi), %rax                       // %rax = 0             **%rdi = &y
addq $1, %rax                           // %rax = 0 + 1 = 1
ret                                      
.L2:
subq $8, %rsp        // γ = γ - 8  
subq $1, %rsi        // %rsi = 2
call bazz            // γ = γ - 8
addq $1, %rax
addq $8, %rsp
ret

bar:
pushq %rbp         // β
pushq %rbx         // a = 10
subq $8, %rsp      // γ = γ - 8
movq %rdi, %rbx    // a = &y
movq (%rsi), %rbp  // β = 10             
movl $3, %esi      // %rsi = 3
call bazz          // γ = γ - 8
addq (%rbx), %rax
movq %rax, (%rbx)
addq %rbp, %rax
addq $8, %rsp
popq %rbx
popq %rbp
ret

foo:
pushq %rbx          // a
subq $16, %rsp      //
movq %rdi, %rax     /// %rax = 10
movq %rsi, %rdi     // %rdi = &y
xorl %edx, %edx     // %edx = 0
movq (%rsi), %rbx   // a = 0
addq %rbx, %rax     // a = 0 + 10 = 10
movq %rax, (%rsp)   // *(γ - 16) = 10
movq %rsp, %rsi     // %rsi = γ - 16, where *(γ - 16) = 10
call bar            // *(γ - 16) = address of addq %rbx, %rax
addq %rbx, %rax
jne .L12
addq $16, %rsp
popq %rbx
ret
*/