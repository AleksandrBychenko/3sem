#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
/*



 * два процесса (неродственных) логгируют некоторую информацию в один файл
 * с помощью семафона обеспечить отсутствие гонки
 */
 
 
 
 //gcc -pthread -Wall -o h race.c //  для запуска 


#define SEMAPHORE_NAME "/test_sem"
#define FILE_NAME "test.log"

static volatile int alive = 1; // программа жива 



void signal_handler(int dummy) {
    alive = 0;
}



int echo(const int idx) {
    char buff[100];
    int pid = getpid();//процес айди // айди среды 
    int f = open(FILE_NAME, O_CREAT + O_APPEND + O_WRONLY, 0777); //открыть файл (имя , создать + точка записи в конец + писать , все могут писать )   
    if (f < 0) {
        perror("open");
        return 1;
    }

    sprintf(buff, "[%d] msg %d\n", pid, idx); //буфер записывает в буфер  сообщение  
    printf("%s", buff);

    if (write(f, buff, strlen(buff)) < 0) { // что процесс пишет проверка
        perror("write");
        return 1;
    }
     // close (f);

    sleep(1);
    return 0;
}




int main(int argc, char **argv) {
    sem_t *sem;
    int idx = 0;
    // sem_unlink(SEMAPHORE_NAME) // удаляет семафору 
     
    if (argc == 2 && !strcmp(argv[1], "delete")) {
        sem_unlink(SEMAPHORE_NAME);
        return 0;
    }

    signal(SIGINT, signal_handler); // если контр с то выполняется signal_handler и эхо завершается выходит в вайл  а там уже олл дрцгого значенгия следовательно выходим ищз цикла 
    
    
    
    
    sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 1); // открыть семафор (имя, создать , доступ для всех , начальное значение 1 )
    if (sem == SEM_FAILED) {
        perror("sem_open"); // распечатает читабельные значение ошибки 
        return 1;
    }
    
    


    while (alive) {
        if (sem_wait(sem) < 0) {   //пытается захватитт семафор ждет 
            perror("sem_wait");
            return 1;
        }
        if(echo(idx++)) {
            alive = 0; //то что программа жива 
            // let to release sem
        }
        if (sem_post(sem) < 0) { // разблокировать семафор посылат
        
            perror("sem_post");
        }
        usleep(100);//чтобы другой успер задержать семафор 
    }
    //здесь может быть только 1 поток 

    return 0;
}






