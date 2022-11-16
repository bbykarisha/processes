#include <unistd.h>
#include <sys/wait.h>

enum { BUF_SIZE = 1024 };

//grep -v '^[[:blank:]]' file.txt | grep '(' | cut -f 1 -d '(' | sort

int
main(void)
{
    char *fst_command[] = {"grep", "-v", "^[[:blank:]//]", NULL};
    char *snd_command[] = {"grep", "(", NULL};
    char *trd_command[] = {"cut", "-f", "1", "-d", "(", NULL};
    char *fth_command[] = {"sort", NULL};

    char buffer[BUF_SIZE];
    int num = 0;

    int fst_fd[2];
    pipe(fst_fd);

    while ((num = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0) {
        write(fst_fd[1], buffer, num);
    }
    close(fst_fd[1]);

    int snd_fd[2];
    pipe(snd_fd);

    if (fork() == 0) {
        close(snd_fd[0]);
        dup2(fst_fd[0], STDIN_FILENO);
        close(fst_fd[0]);
        dup2(snd_fd[1], STDOUT_FILENO);
        close(snd_fd[1]);
    
        execvp(fst_command[0], fst_command);
    }

    close(fst_fd[0]);  //закрыла первый канал
    close(snd_fd[1]);
   
    int trd_fd[2];
    pipe(trd_fd);

    if (fork() == 0) {
        close(trd_fd[0]);
        dup2(snd_fd[0], STDIN_FILENO);
        close(snd_fd[0]);
        dup2(trd_fd[1], STDOUT_FILENO);
        close(trd_fd[1]);

        execvp(snd_command[0], snd_command);
    }

    close(snd_fd[0]);  //закрыла второй канал
    close(trd_fd[1]);

    int fth_fd[2];
    pipe(fth_fd);

    if (fork() == 0) {
        close(fth_fd[0]);
        dup2(trd_fd[0], STDIN_FILENO);
        close(trd_fd[0]);
        dup2(fth_fd[1], STDOUT_FILENO);
        close(fth_fd[1]);

        execvp(trd_command[0], trd_command);
    }

    close(trd_fd[0]);  //закрыла третий канал
    close(fth_fd[1]);

    if (fork() == 0) {
        dup2(fth_fd[0], STDIN_FILENO);
        close(fth_fd[0]);

        execvp(fth_command[0], fth_command);
    }

    close(fth_fd[0]);  //закрыла четвертый канал

    while (wait(NULL) > 0) {}
    
    return 0;
}