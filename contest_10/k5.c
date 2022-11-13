#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

enum 
{
    FNAME_SIZE = 7,
    BUF_SIZE = 32
};

int
main(void)
{
    char file_name[FNAME_SIZE] = "XXXXXX";
    int fd_write = mkstemp(file_name);
    int fd_read = open(file_name, O_RDONLY);

    unlink(file_name);

    //child - writer
    if (fork() == 0) {
        int character = 0;
        int count = 0;
        int pos_read = 0; 
        int pos_write = 0;

        while (read(STDIN_FILENO, &character, 1) == 1) {
            pos_read = lseek(fd_read, 0, SEEK_CUR);
            pos_write = lseek(fd_write, 0, SEEK_CUR);

            while (pos_read - pos_write == 1) {
                usleep(50);
                pos_read = lseek(fd_read, 0, SEEK_CUR);
            }
            write(fd_write, &character, 1);
            ++count;
            if (count == BUF_SIZE) {
                count = 0;
                lseek(fd_write, 0, SEEK_SET);
            }
        }
        exit(0);
    }

    //parent - reader
    int character = 0;
    int count = 0;
    int pos_read = 0;         
    int pos_write = 0;

    while (1) {
        pos_read = lseek(fd_read, 0, SEEK_CUR);
        pos_write = lseek(fd_write, 0, SEEK_CUR);  
        while (pos_read == pos_write) {
            if (waitpid(-1, NULL, WNOHANG) > 0) {
                close(fd_read);
                close(fd_write);
                return 0;
            }
            usleep(50);
            pos_write = lseek(fd_write, 0, SEEK_CUR);
        }  
        read(fd_read, &character, 1);
        write(STDOUT_FILENO, &character, 1);
        
        ++count;
        if (count == BUF_SIZE) {
            count = 0;
            lseek(fd_read, 0, SEEK_SET);
        }
    }

    return 0;
}