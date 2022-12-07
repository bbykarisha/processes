#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

enum { SEM_COUNT = 1 };

void
process_son(char *str, int number, int param)
{
    key_t key = ftok(str, 1);
    // получаем доступ к созданному массиву семафоров
    int semid = semget(key, SEM_COUNT, 0);

    struct sembuf ops;
    ops.sem_num = 0; // номер семафора в массиве
    ops.sem_flg = 0; // флаги
    
    int index = param;
    while (index < number) {        
        if (param == 0) {
            ops.sem_op = -1; // уменьшаем на 1
        } else {
            ops.sem_op = 0; // ставим 0 для блокировки
        }
        semop(semid, &ops, 1); // выполняем операцию над семафором

        printf("%d\n", index);
        fflush(stdout);

        index += 2;

        if (param == 0) {
            ops.sem_op = -1; // уменьшаем на 1
        } else {
            ops.sem_op = 2; // увеличиваем на 2
        }
        semop(semid, &ops, 1); // выполняем операцию
    }
}

int semid = 0;

void
sigint_handler(int sig_num)
{
    // удаление массива семафоров
    semctl(semid, 0, IPC_RMID); 
    exit(0);
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    key_t key = ftok(argv[0], 1);
    // создаем массив из 1 семафора для синхронизации
    // работы 2-х процессов
    semid = semget(key, SEM_COUNT, IPC_CREAT | 0666);
    // устанавливаем начальное значение семафора в 2
    semctl(semid, 0, SETVAL, (unsigned short) 2);

    int number = 0;
    scanf("%d", &number);

    if (fork() == 0) {
        process_son(argv[0], number, 0);  
        exit(0);
    }

    if (fork() == 0) {
        process_son(argv[0], number, 1);
        exit(0);
    }

    while (wait(NULL) > 0) {}

    // удаление массива семафоров
    semctl(semid, 0, IPC_RMID);

    return 0;
}