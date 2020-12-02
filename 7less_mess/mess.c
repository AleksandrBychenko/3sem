#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>

struct msgbuf { // заполняем структуру ии ее же отсылаем 
    long mtype;       /* message type, must be > 0 */
    int val[2];
};

static int add(int qid, int a, int b) //  добавление 2 чисел 
{
    struct msgbuf msg;
    msg.mtype = 1; // 1 вопрпросы, 2 ответы
    msg.val[0] = a;
    msg.val[1] = b;

    if (msgsnd(qid, (void *) &msg, sizeof(msg.val), 0) == -1) {// берет структуру и запихивает ее в посылку (посылает структкрку )
        perror("add msgsnd");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(qid, (void *) &msg, sizeof(msg.val), 2, 0) == -1) {// получили сообщкение (ждет такую же структурку ( .. Б  ...  ошибки ,  никакого ожидания ) 
        perror("add msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("DEBUG received: %d\n", msg.val[0]); //  пишет результат 
    return msg.val[0];
}

static void server(int qid)
{
    struct msgbuf msg;

    while(1) {
        if (msgrcv(qid, (void *) &msg, sizeof(msg.val), 1, 0) == -1) { // получаем структурку 
            perror("srv msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("DEBUG received: %d + %d\n", msg.val[0], msg.val[1]);
        msg.mtype = 2;
        msg.val[0] += msg.val[1]; // переделывем и =ее 

        if (msgsnd(qid, (void *) &msg, sizeof(msg.val), 0) == -1) {//  отсылаеи 
            perror("srv msgsnd");
            exit(EXIT_FAILURE);
        }
    }
}

int
main(int argc, char *argv[]) {
    int qid;
    int msgkey = 1234;

    qid = msgget(msgkey, IPC_CREAT | 0666); //создаем сообщение 
    if (qid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (!strcmp(argv[1], "server")) { // если сервер то работаем как сервер 
        server(qid);
    }
    while(1) {
        int a, b;
        scanf("%d %d", &a, &b);
        printf("result = %d\n", add(qid, a, b));
    }

    exit(EXIT_SUCCESS);
}
