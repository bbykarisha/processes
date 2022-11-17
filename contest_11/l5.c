#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void
processing(int fd_1, int fd_2)
{
    int first = 0;
    int second = 0;

    while ((read(fd_1, &first,  sizeof(int)) == sizeof(int)) && 
            (read(fd_1, &second, sizeof(int)) == sizeof(int)))
    {     
        int sum = first + second;
        int dif = first - second;

        write(fd_2, &sum, sizeof(int));
        write(fd_2, &dif, sizeof(int));
    } 

    close(fd_1);
    close(fd_2);
}

void
process_son(int fd_1, int fd_2, int num_1, int num_2)
{
    write(fd_1, &num_1, sizeof(int));
    write(fd_1, &num_2, sizeof(int));
    close(fd_1);

    int sum = 0;
    int dif = 0;

    read(fd_2, &sum, sizeof(int));
    read(fd_2, &dif, sizeof(int));
    close(fd_2);

    printf("%d %d %d %d\n", num_1, num_2, sum, dif);
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
        processing(fst_fd[0], snd_fd[1]);
        exit(0);
    }

    close(fst_fd[0]);
    close(snd_fd[1]);

    int fst_num = 0;
    int snd_num = 0;

    while (scanf("%d%d", &fst_num, &snd_num) == 2) {
        if (fork() == 0) {
            process_son(fst_fd[1], snd_fd[0], fst_num, snd_num);
            exit(0);    
        }   
    }

    close(fst_fd[1]);
    close(snd_fd[0]);
    
    while (wait(NULL) > 0) {}

    return 0;
}