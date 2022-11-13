#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char *prog_path = argv[1];
    char *file_path = argv[2];

    if (fork() == 0) {
        int fd = open(file_path, O_RDONLY);  
        if (fd == -1) {
            return 0;
        }  

        dup2(fd, STDIN_FILENO);  
        close(fd);

        execlp(prog_path, prog_path, NULL); 
    }
    wait(NULL);

    return 0;
}
