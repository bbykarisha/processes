#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char *first_prog = argv[1];
    char *second_prog = argv[2];
    char *third_prog = argv[3];

    int fd[2];
    pipe(fd);
    
    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);

        close(fd[0]);
        close(fd[1]);

        if (fork() == 0) {
            execlp(first_prog, first_prog, NULL);
        }

        int status = 0;
        wait(&status);

        if (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0))) {
            if (fork() == 0) {
                execlp(second_prog, second_prog, NULL);
            }
            wait(&status);
        }

        exit(0);
    }

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(third_prog, argv + 3);
    }

    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) > 0) {}

    return 0;
}