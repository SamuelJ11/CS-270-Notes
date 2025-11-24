#include <unistd.h>  // contains system calls like read(), write(), and close()
#include <fcntl.h>   // defines constants for opening files, e.g., O_RDONLY, O_WRONLY, O_CREAT
#include <stdlib.h>  // general utilities like exit()
#include <stdio.h>   // standard I/O like printf(), scanf(), fgets()
#include <sys/types.h>  // needed for file permissions and modes
#include <sys/stat.h>   // needed for file permissions and modes
#include <string.h>     // for functions like strlen()

#define DEF_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
#define DEF_UMASK S_IWGRP | S_IWOTH

int main() 
{
    umask(DEF_UMASK);

    int operation_bit;
    printf("Do you wish to read or write to this file?\n");
    printf("Enter 0 for read or 1 for write: ");
    scanf("%d", &operation_bit);
    getchar(); // consume leftover newline after pressing Enter

    switch (operation_bit)
    {
        case 0:
        {        
            int fd = open("DONOTREAD.txt", O_RDONLY);
            if (fd < 0) 
            { 
                perror("open"); exit(1); 
            }

            char buffer[1024];
            ssize_t bytes_read = read(fd, buffer, sizeof(buffer) -1);   // -1 to make room for null-terminator
            if (bytes_read < 0) 
            { 
                perror("read"); close(fd); exit(1); 
            }
            buffer[bytes_read] = '\0';  // add a null terminator \0 at the end
                                        // this makes the data in buffer a C string so we can printf it.
            printf("File contents:\n%s\n", buffer);
            close(fd);
            break;
        }
        
        case 1: 
        {        
            int fd = open("DONOTREAD.txt", O_WRONLY | O_CREAT | O_APPEND, DEF_MODE);
            if (fd < 0) 
            { 
                perror("open"); exit(1); 
            }

            char buffer[1024];
            printf("Enter text to write: ");
            fgets(buffer, sizeof(buffer), stdin);

            ssize_t n = write(fd, buffer, strlen(buffer));
            if (n < 0) 
            { 
                perror("write"); close(fd); exit(1); 
            }

            close(fd);
            break;
        }
        default: 
        {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}