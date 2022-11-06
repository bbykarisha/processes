#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        execvp(argv[1], argv + 1);
    }

    wait(&status);

    if (WIFEXITED(status) == 0) {
        int sig_num = WTERMSIG(status);
        printf("%d\n", sig_num);
    } else {
        printf("%d\n", WEXITSTATUS(status));
    }

    return 0;
}
