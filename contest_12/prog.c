#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int
main(void)
{
    pid_t pid;
    if ((pid = fork()) == 0) {
        write(1, "A", 1);
        write(1, "B", 1);
    } else {
        kill(pid, SIGINT);
        write(1, "C", 1);
        wait(0);
    }
}
