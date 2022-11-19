#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void
processing(int *fd_1, int *fd_2)
{
    int buffer[2];
    int result[2];
    
    while (read(fd_1[0], buffer, 2 * sizeof(int)) == 2 * sizeof(int)) {     
        result[0] = buffer[0] + buffer[1];
        result[1] = buffer[0] - buffer[1];

        write(fd_2[1], result, 2 * sizeof(int));
    } 

    close(fd_1[0]);
    close(fd_2[1]);
}

void
process_son(int *fd_1, int *fd_2, int *fd_3, int num_1, int num_2)
{
    int num = 0;
    read(fd_3[0], &num, sizeof(int));
     
    int buffer[2];
    buffer[0] = num_1;
    buffer[1] = num_2;

    write(fd_1[1], buffer, 2 * sizeof(int));
     
    int result[2];
    read(fd_2[0], result, 2 * sizeof(int));
    
    printf("%d %d %d %d\n", num_1, num_2, result[0], result[1]);

    num = 10;
    write(fd_3[1], &num, sizeof(int));

    close(fd_1[1]);
    close(fd_2[0]);
    close(fd_3[0]);
    close(fd_3[1]);
}

int
main(void)
{
    setbuf(stdin, 0);

    int fst_fd[2];
    pipe(fst_fd);

    int snd_fd[2];
    pipe(snd_fd);

    if (fork() == 0) {
        close(fst_fd[1]);
        close(snd_fd[0]);
        processing(fst_fd, snd_fd);
        exit(0);
    }

    int trd_fd[2];
    pipe(trd_fd);

    int fst_num = 0;
    int snd_num = 0;

    int num = 10;
    write(trd_fd[1], &num, sizeof(int));

    while (scanf("%d%d", &fst_num, &snd_num) == 2) {
        if (fork() == 0) {
            close(fst_fd[0]);
            close(snd_fd[1]);
            process_son(fst_fd, snd_fd, trd_fd, fst_num, snd_num);
            exit(0);    
        } 
        waitpid(-1, NULL, WNOHANG);  
    }

    close(fst_fd[0]);
    close(snd_fd[1]);
    close(fst_fd[1]);
    close(snd_fd[0]);
    close(trd_fd[0]);
    close(trd_fd[1]);
    
    while (wait(NULL) > 0) {}

    return 0;
}
