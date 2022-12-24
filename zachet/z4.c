#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#if 0
union semun 
{
    int     val;             
    struct  semid_ds *buf;   
    unsigned short *array;          
};
#endif

void 
process_son(char *str, int fd, int n, int ind)
{
    key_t key = ftok(str, 1);
    int semid = semget(key, n, 0);

    int num = 0;
    while (1) {
        semop(semid, &(struct sembuf) {ind, -1, 0}, 1);
        lseek(fd, -1 * sizeof(int), SEEK_CUR);
        read(fd, &num, sizeof num);
        if (num > 0) {
            --num;
            write(fd, &num, sizeof num);
            semop(semid, &(struct sembuf) {(ind + 1) % n, 1, 0}, 1);
        } else if (num == 0) {
            semop(semid, &(struct sembuf) {(ind + 1) % n, 1, 0}, 1);
            break;
        }
    }
}

int
main(int argc, char *argv[])
{
    union semun 
    {
        int     val;             
        struct  semid_ds *buf;   
        unsigned short *array;          
    };

    int n = 0;
    int k = 0;
    scanf("%d%d", &n, &k);

    if ((n == 0) || (k == 0)) {
        return 1;
    } 

    char tmp_file[7] = "XXXXXX";
    int tmp_fd = mkstemp(tmp_file);
    unlink(tmp_file);

    write(tmp_fd, &k, sizeof k);

    key_t key = ftok(argv[0], 1);
    int semid = semget(key, n, IPC_CREAT | 0666);

    unsigned short *arr_sem = calloc(n, sizeof *arr_sem);

    arr_sem[0] = 1;
    for (int i = 1; i < n; ++i) {
        arr_sem[i] = 0;
    }

    union semun arg;
    arg.array = arr_sem;

    semctl(semid, 0, SETALL, &arg);

    pid_t pid;
    for (int i = 0; i < n; ++i) {
        if ((pid = fork()) == 0) {
            process_son(argv[0], tmp_fd, n, i);
            exit(0);
        }
    }

    while (wait(NULL) > 0) {}

    lseek(tmp_fd, 0, SEEK_SET);

    int num = 0;
    while (read(tmp_fd, &num, sizeof num) == sizeof num) {
        printf("%d\n", num);
    }

    free(arr_sem);
    close(tmp_fd);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
