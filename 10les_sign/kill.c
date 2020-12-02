#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char ** argv) {
    signal(SIGQUIT, SIG_IGN); // еслди из консоли вбить 1 значение то мы дадаим сигнал SIGQUIT что значит игнорирование процесса (завершение ) но сигнал меняет их  свойство поэтому программа продолжит далише работать ( SIGQUIT игнорируется ) 
    //signal(SIGINT, SIG_IGN); //выход из программы ( контр с не работал ) 
    while(1) {   // УСПЕЛ ОБРАБОТАТЬ (просто работал )
        sleep(1);
    }
    return 0;
}


