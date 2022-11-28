#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int count = 0;
 
void
sigint_handler(int sig_num)
{
    ++count;

    char number = count + '0';
    char new_line = '\n';

    write(STDOUT_FILENO, &number, sizeof number);
    write(STDOUT_FILENO, &new_line, sizeof new_line);
    
    if (count == 5) {
        signal(SIGINT, SIG_DFL);
        exit(0);
    }
}

int
main(void)
{
    signal(SIGINT, sigint_handler);
    pid_t pid;

    if ((pid = fork()) == 0) {
        while (1) {
            pause();
        }
    }

    for (int i = 0; i < 5; ++i) {
        kill(pid, SIGINT);
        usleep(50);
    }
    wait(NULL);
    
    return 0;
}
