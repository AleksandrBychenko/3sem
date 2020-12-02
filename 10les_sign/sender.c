#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//#include <zconf.h>
#include <unistd.h>


int main(int argc, char **argv) {//программа посылающая сигналы 
    int pid = atoi(argv[1]);// при первом запуске пеердаем айдишник 

    while (1) {//
        int v = getchar();//взяли с клавиатуры символ (чтобы можно было писать с клавиатуры во время выполнения программы 
        if (v <= 0) {// если конец ввода то выходим 
            break;//
        }
        for (int b = 7; b >= 0; b--) {//
            if ((v >> b) & 1) {// 010101 & 00001 = 1
                kill(pid, SIGUSR2);//(процесс айди, сигнал ) послает как в обычном случае 
            } else {//
                kill(pid, SIGUSR1);//
            }//
            usleep(100);//
        }
    }
    return 0;
}
