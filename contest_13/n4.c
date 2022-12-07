#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

typedef struct Message_queue
{
    long type;
    int val;
} Message_queue;

void
client(char *str, int number)
{
    key_t key_1 = ftok(str, 1);
    key_t key_2 = ftok(str, 2);

    // получили доступ к ресурсам
    int msqid_cs = msgget(key_1, 0);
    int msqid_sc = msgget(key_2, 0);

    Message_queue arg;
    size_t msgsize = sizeof(int);

    // number раз запрашиваем увеличение числа
    arg.type = 1;
    for (int i = 0; i < number; ++i) {
        msgsnd(msqid_cs, &arg, msgsize, 0);
    }

    // запрашиваем операцию возврата числа
    arg.type = 2;
    msgsnd(msqid_cs, &arg, msgsize, 0);

    Message_queue num;

    // получаем итоговое число
    msgrcv(msqid_sc, &num, msgsize, 1, 0);
    printf("%d\n", num.val);

    // просим сервер завершиться
    arg.type = 3;
    msgsnd(msqid_cs, &arg, msgsize, 0);
}

void
server(char *str)
{
    key_t key_1 = ftok(str, 1);
    key_t key_2 = ftok(str, 2);

    // получили доступ к ресурсам
    int msqid_cs = msgget(key_1, 0);
    int msqid_sc = msgget(key_2, 0);

    Message_queue arg, final_num;
    size_t msgsize = sizeof(int);

    int number = 0;

    while (1) {
        // получаем сообщение
        msgrcv(msqid_cs, &arg, msgsize, 0, 0);
        
        if (arg.type == 1) {
            // получаем сообщение о том, что нужно увеличить число
            ++number;
        } else if (arg.type == 2) {
            // получаем сообщение о возврате числа
            final_num.type = 1;
            final_num.val = number;
            // отправляем итоговое число клиенту
            msgsnd(msqid_sc, &final_num, msgsize, 0);
        } else if (arg.type == 3) {
            // получаем сообщение о завершении
            break;    
        }
    }
}

int msqid_cs = 0;
int msqid_sc = 0;

void
sigint_handler(int sig_num)
{
    // удаление ресурсов
    msgctl(msqid_cs, IPC_RMID, NULL);
    msgctl(msqid_sc, IPC_RMID, NULL);

    exit(0);
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    key_t key_1 = ftok(argv[0], 1);
    key_t key_2 = ftok(argv[0], 2);

    // создали очередь сообщений для передачи данных 
    // от клиента к серверу
    msqid_cs = msgget(key_1, IPC_CREAT | 0666);
    // создали очередь сообщений для передачи данных 
    // от сервера к клиенту
    msqid_sc = msgget(key_2, IPC_CREAT | 0666);

    int number = 0;
    scanf("%d", &number);

    if (fork() == 0) {
        client(argv[0], number);
        exit(0);
    }

    if (fork() == 0) {
        server(argv[0]);
        exit(0);
    }

    while(wait(NULL) > 0) {}

    // удаление ресурсов
    msgctl(msqid_cs, IPC_RMID, NULL);
    msgctl(msqid_sc, IPC_RMID, NULL);

    return 0;
}
