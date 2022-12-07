#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>

enum
{
    SEM_COUNT = 2,
    MEM_SIZE = 1024
};

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short array[SEM_COUNT];    
};

void
process_son(char *str)
{
    key_t key_1 = ftok(str, 1);
    key_t key_2 = ftok(str, 2);
    key_t key_3 = ftok(str, 3);

    semget(key_1, SEM_COUNT, 0);
    shmget(key_2, MEM_SIZE, 0);
    msgget(key_3, 0);
}

int semid = 0;
int shmid = 0;
int msqid = 0;

void
sigint_handler(int sig_num)
{
    // удаляем ресурсы
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    msgctl(msqid, IPC_RMID, NULL);

    exit(0);
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    key_t key_1 = ftok(argv[0], 1);
    key_t key_2 = ftok(argv[0], 2);
    key_t key_3 = ftok(argv[0], 3);

    // создаем массив семафоров
    semid = semget(key_1, SEM_COUNT, IPC_CREAT | 0666);
    // создаем разделяемую память на 1 КБ
    shmid = shmget(key_2, MEM_SIZE, IPC_CREAT | 0666);
    // создаем очередь сообщений
    msqid = msgget(key_3, IPC_CREAT | 0666);

    union semun arg;
    arg.val = 0;
    
    // устанавливаем значения семафоров в 0
    semctl(semid, 0, SETVAL, &arg);
    semctl(semid, 1, SETVAL, &arg);

    if (fork() == 0) {
        process_son(argv[0]);
        exit(0);
    }

    wait(NULL);

    // удаляем ресурсы
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    msgctl(msqid, IPC_RMID, NULL);

    return 0;
}
