#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int count = 0;
int finish = 0;
int do_command = 0;

void
sigint_handler(int sig_num)
{
    ++count;
    if (count % 3 == 0) {
        ++do_command;
    }
}

void
sigterm_handler(int sig_num) {
    finish = 1;
}

void
run_command(char **first, char **second)
{
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(first[0], first);
        exit(1);
    }

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(second[0], second);
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) > 0) {}   
}

int
main(int argc, char *argv[])
{
    setbuf(stdin, 0);
    setbuf(stdout, 0);

    int pos = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--") == 0) {
            pos = i + 1;
            argv[i] = NULL;
            break;
        }
    }

    pid_t pid;
    if ((pid = fork()) == 0) {
        signal(SIGINT, sigint_handler);
        signal(SIGTERM, sigterm_handler);
        while (1) {
            if (do_command > 0) {
                run_command(argv + 1, argv + pos);
                --do_command;
            } else if (finish == 1) {
                exit(0);
            } else {
                usleep(50);
            }  
        }
    }

    int num = 0;
    while (scanf("%d", &num) == 1) {
        if (num == 0) {
            kill(pid, SIGINT);
        } else if (num > 0) {
            usleep(num);
        }
    }

    kill(pid, SIGTERM);
    wait(NULL);

    return 0;
}
