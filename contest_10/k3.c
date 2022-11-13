#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
 
int
main(int argc, char *argv[])
{
    char *first_prog = argv[1];
    char *second_prog = argv[2];
    char *third_prog = argv[4];

    char *file_path = argv[3];

    int status = 0;

    if (fork() == 0) {
        int fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd == -1) {
            return 0;
        }

        dup2(fd, STDOUT_FILENO);
             
        if (fork() == 0) {
            execlp(first_prog, first_prog, NULL);
        }  

        wait(&status);

        if (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0))) {
            if (fork() == 0) {
                execlp(second_prog, second_prog, NULL);
            }

            wait(&status);
        }
 
        close(fd);

        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {         
            exit(0);    
        } else {
            exit(1);
        }
    }

    wait(&status);

    if (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0))) {
        return 0;
    }

    if (fork() == 0) {
        execlp(third_prog, third_prog, NULL);
    }

    while (wait(NULL) > 0) {}

    return 0;
}