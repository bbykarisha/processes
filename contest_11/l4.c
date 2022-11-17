#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int
main(void)
{
    int fd[2];
    pipe(fd);

    int character = 0;
    int count = 0;

    if (fork() == 0) {
        close(fd[0]);
        while (read(STDIN_FILENO, &character, 1) == 1) {
            ++count;
            if (count % 2 == 0) {
                write(fd[1], &character, 1);
            }
        }
        close(fd[1]);

        exit(0);
    }  

    close(fd[1]);
     
    if (fork() == 0) {
        if (fork() == 0) {
            while(read(fd[0], &character, 1) == 1) {
                ++count;
                if (count % 2 == 0) {
                    write(STDOUT_FILENO, &character, 1);
                }
            }
            close(fd[0]);
            exit(0);
        }
        close(fd[0]);
        wait(NULL);
        
        exit(0);
    }

    close(fd[0]);
     
    while (wait(NULL) > 0) {}

    return 0;
}
