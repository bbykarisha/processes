#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

unsigned long long num = 0;
int seconds = 0;

void
alarm_handler(int sig_num)
{
    ++seconds;
    if (seconds == 5) {
        printf("%llu\n", num);
        seconds = 0;
    }

    alarm(1);
}

void
sigint_handler(int sig_num)
{
    printf("%d\n", 5 - seconds);
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