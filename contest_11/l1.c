#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int pos = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--") == 0) {
            pos = i + 1;
            argv[i] = NULL;
            break;
        }
    }

    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[1], argv + 1);
    }

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[pos], argv + pos);
    }

    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) > 0) {}

    return 0;
}
