#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

enum { BUF_SIZE = 20 };

int seconds = 0;
unsigned long long num = 0;

void
alarm_handler(int sig_num)
{
    char new_line = '\n';
    ++seconds;
    
    if (seconds == 5) {
        int index = 0;
        char output[BUF_SIZE];
        unsigned long long num_copy = num;

        while (num_copy != 0) {
            output[index] = num_copy % 10 + '0';
            num_copy /= 10;
            ++index;
        }

        for (int i = index - 1; i >= 0; --i) {
            write(STDOUT_FILENO, &output[i], sizeof(char));
        }
        write(STDOUT_FILENO, &new_line, sizeof new_line);

        seconds = 0;
    }

    alarm(1);
}

void
sigint_handler(int sig_num)
{
    char rest = 5 - seconds + '0';
    char new_line = '\n';

    write(STDOUT_FILENO, &rest, sizeof rest);
    write(STDOUT_FILENO, &new_line, sizeof new_line);
}

int
main(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, alarm_handler);

    scanf("%llu", &num);
    alarm(1);

    while (num > 0) {
        --num;
    }

    return 0;
}
