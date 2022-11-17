#include <unistd.h>
#include <sys/wait.h>

//grep "^[[:alnum:]_]*(" | cut -f 1 -d "(" | sort

int
main(void)
{
    char *fst_command[] = {"grep", "^[[:alnum:]_]*(", NULL};
    char *snd_command[] = {"cut", "-f", "1", "-d", "(", NULL};
    char *trd_command[] = {"sort", NULL};

    int fst_fd[2];
    pipe(fst_fd);

    if (fork() == 0) {
        dup2(fst_fd[1], STDOUT_FILENO);
        close(fst_fd[0]);
        close(fst_fd[1]);

        execvp(fst_command[0], fst_command);
    }

    close(fst_fd[1]);

    int snd_fd[2];
    pipe(snd_fd);

    if (fork() == 0) {
        close(snd_fd[0]);
        dup2(fst_fd[0], STDIN_FILENO);
        dup2(snd_fd[1], STDOUT_FILENO);
        close(fst_fd[0]);
        close(snd_fd[1]);

        execvp(snd_command[0], snd_command);
    }

    close(fst_fd[0]);
    close(snd_fd[1]);

    if (fork() == 0) {
        dup2(snd_fd[0], STDIN_FILENO);
        close(snd_fd[0]);

        execvp(trd_command[0], trd_command);
    }

    close(snd_fd[0]);

    while (wait(NULL) > 0) {}

    return 0;
}
