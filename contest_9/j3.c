#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char *path_2 = argv[argc - 1];
    argv[argc - 1] = NULL;

    if (fork() == 0) {
        execvp(argv[1], argv + 1);
    }

    int status = 0;
    wait(&status);

    if ((WIFEXITED(status) == 0) || (WEXITSTATUS(status) != 0)) {
        return 0;
    }
    
    if (fork() == 0) {
        execlp(path_2, path_2, NULL);    
    }
    wait(&status);

    return 0;
}