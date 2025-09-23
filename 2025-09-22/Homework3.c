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


//Problem 0:3
int foo(char *rdi) 
{
    char retval = 0;        //movl $0, %eax
        //leaq 1(%rdi), %rsi
    char* ecx;      
    char* rsi;

    while (retval <= 19 && (ecx))
    {
        retval++;
        rdi = rsi;
        *rsi = rdi + 1;
        ecx = *rdi;        
        
    }
    return retval;
}

/*
foo:
movl $0, %eax           // set return register %eax to 0
jmp .L2                 // unconditionally jump to L2

.L3:
addl $1, %eax           // increment %eax by 1
movq %rsi, %rdi         // copy the value of %rsi into %rdi (first argument)

.L2:
leaq 1(%rdi), %rsi      // compute %rdi + 1 (address increment) and store in %rsi
movsbl (%rdi), %ecx     // load signed byte from memory at %rdi into %ecx (zero-extended to 32 bits)
cmpl $19, %eax          // compare %eax with 19
setle %dl               // set %dl = 1 if %eax <= 19, else %dl = 0
movzbl %dl, %edx        // zero-extend %dl into %edx (convert to 32-bit 0 or 1)
testl %edx, %ecx        // bitwise AND %edx and %ecx, sets flags; used to check if both are nonzero
jne .L3                 // jump to L3 if ZF = 0 (i.e., test result was nonzero)
ret                     // return, value in %eax
*/

/*
Procedure:

1. look for instructions that read or write from %rdi, %rsi, %rdx, %rcx, %r8, %r9 before any call to another function:
   Those registers are likely parameters.
2. %rdi is being dereferenced ((%rdi)) → probably a pointer parameter.
   %rsi is storing the “next pointer” address → probably a pointer parameter.
3. identify the control structure:  Loop: there’s a jump that goes backwards to a label already executed.
                                    If/else: conditional jump that skips a block.
                                    Switch: often uses jump tables (jmp *table(%rax))
4. determine which label corresponds to the loop body:
        The label that is jumped to by the conditional jump is the body (L3).
        The label that is at the top of the code or just before the check is usually the condition/loop check (L2).

5. (if it's a loop) determine loop type (while vs do-while vs for):
        jmp L2 before executing L3 → check happens before first execution → while or for.
        No jump before first execution → body executes at least once → do-while.
6. (if it's a loop) check the exit conditions and counters:
        If there’s an add to a counter inside the loop → typical while(counter < limit) style
        Conditional jump (jne) decides whether to repeat → combine with your counter/flag.






*/