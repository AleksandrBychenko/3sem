#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>

volatile int val = 0;// волотает помогает менять значение между процессами 
volatile int len = 0;//

void u1_handler(int x) {//обработка сигналов 
    val *= 2;// val <<= 1  побитывае смещение в право 
    val += 0;// прибавели 0  
    len ++;//
    if(len == 8) {//как только 8 бит накопим мы его печатаем 
        val &= 0x0ff;// 8 бит обрезаем 
        printf("%c", val);//за счет д мы новое двоичное число записываем как символ а не как 10 число 
        len = 0;//
    }//
    fflush(stdout);//
}

void u2_handler(int x) {//
    val *= 2;//
    val += 1;//
    len ++;//
    if(len == 8) {//
        val &= 0x0ff;//
        printf("%c", val);//за счет д мы новое двоичное число записываем как символ а не как 10 число 
        len = 0;//
    }//
    fflush(stdout);//
}

int main(int argc, char ** argv) {//из  числа ( заходя и исправляя в 2 его представлении получаем совершенно новое значение 
    signal(SIGUSR1, u1_handler);//если падается этот сигнал то делай это
    signal(SIGUSR2, u2_handler);//
    printf("pid: %d\n", getpid());// пишим процесс айди  
    while(1) {//
        sleep(1);//
    }
    return 0;
}
