#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)     // char** for array of strings
{
    struct stat statbuf;    // this creates a variable of type 'stat' called "statbuf" that will hold file information.
    char *type, *readok, *writeok;      // these will point to strings like "regular", "directory", "yes", "no"
    int filesize = -1;

    if (stat(argv[1], &statbuf) < 0) // if the file doesn't exist or can’t be read, stat() returns -1
    {
        perror("error when calling stat()");
        exit(1);
    }

    if (S_ISREG(statbuf.st_mode))  // this looks like a function call, but it’s not a function — it’s a macro that checks file type
    {
        type = "regular";
        filesize = statbuf.st_size;
    }  
        
    else if (S_ISDIR(statbuf.st_mode))
        type = "directory";
    else
        type = "other";

    if (statbuf.st_mode & S_IRUSR)
        readok = "yes";
    else
        readok = "no";

    if (statbuf.st_mode & S_IWUSR)
        writeok = "yes";
    else
        writeok = "no";

    printf("type: %s, read permisson?: %s, write permission?: %s, size: %d bytes \n", type, readok, writeok, filesize);
    
    return 0;
}