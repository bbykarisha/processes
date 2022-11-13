#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char *prog_path = argv[1];
    char *file_path = argv[2];

    if (fork() == 0) {
        int fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) {
            return 0;
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);

        execlp(prog_path, prog_path, NULL);
    }
    wait(NULL);

    return 0;
}