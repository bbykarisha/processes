#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int count = 0;
 
void
sigint_handler(int sig_num)
{
    ++count;
    printf("%d\n", count);
    if (count == 5) {
        signal(SIGINT, SIG_DFL);
        exit(0);
    } else {
        signal(SIGINT, sigint_handler);
    }
}

int
main(void)
{
    signal(SIGINT, sigint_handler);
    pid_t pid;

    if ((pid = fork()) == 0) {
        while (1) {}
    }

    for (int i = 0; i < 5; ++i) {
        usleep(50);
        kill(pid, SIGINT);
    }
    wait(NULL);
    
    return 0;
}