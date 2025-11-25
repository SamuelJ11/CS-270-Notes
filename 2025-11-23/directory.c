#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    DIR *streamp;
    struct dirent *dep;

    // Check if the directory name was provided
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
        exit(1);
    }

    // 2. Open the directory
    streamp = opendir(argv[1]);

    if (streamp == NULL) 
    {
        // Use perror to report the error from opendir
        perror("opendir error");
        exit(1);
    }

    // 3. Reset errno before the loop (readdir might fail and set errno)
    errno = 0;

    // 4. Loop: Read directory entries one by one
    while ((dep = readdir(streamp)) != NULL) 
    {
        printf("Found file: %s\n", dep->d_name);
    }

    // 5. Check for a readdir error
    if (errno != 0)
        // Use perror to report the error from readdir
        perror("readdir error");

    // 6. Close the directory stream handle
    closedir(streamp);

    // 7. Exit successfully
    exit(0);
}