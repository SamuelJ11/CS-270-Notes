#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define HELLO printf("Hello!\n")

int main() 
{   
    if (fork() == 0)            // P₀ forks → creates P₁. Only P₁ enters this block.
    {
        fork();                 // P₁ forks → creates P₂. Now P₁ and P₂ continue from here.
        
        if (fork() != 0)        // Both P₁ and P₂ fork here:
                                //   P₁ → creates P₃
                                //   P₂ → creates P₄
                                // Only P₁ and P₂ (the parents in each pair) enter this 'if' body.
        {
            fork();             // P₁ and P₂ each fork again:
                                //   P₁ → creates P₅
                                //   P₂ → creates P₆
        }
    }  
    HELLO;                      // All 7 processes (P₀–P₆) reach here and print once.
    return 0;
}

