#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

char* gets1(char *s) 
{ 
    int c; 
    char *dest = s;

    printf("Please type a string: ");  // <-- added prompt
    fflush(stdout);                    // ensure the prompt appears immediately

    while((c = getchar()) != '\n' && c != EOF) 
        *dest++ = c; 

    if (c == EOF && dest == s) 
        /* No characters read */ 
        return NULL; 

    *dest++ = '\0';  /* Terminates string */ 
    return s; 
}

char* get_line()
{
    char buf[4];
    char*result;
    gets1(buf);
    result = malloc(strlen(buf) + 1);
    strcpy(result,buf);
    return result;
}

int main() 
{
    char* line;

    printf("Type a string: ");
    line = get_line();

    printf("You entered: %s\n", line);
    return 0;
}