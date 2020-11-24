#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
  с помощью opendir, readdir, closedir, stat
  выписать список файлов, расположенных
  внутри текущей директории с указанием их
  типов (regular file, directory) и размера
*/

// opendir  - открывает директорию по имени 

int main() {
    DIR * d = opendir(".");// указатель структура типа опен (.  -  тикущая директория )
    if(!d) {//если не верно то указатель наловый 
        perror("opendir");
        return 1;
    }
    while(1) {// обход директории 
        struct dirent *e = readdir(d);//читает ( заносит в структкуру в которой есть несколько параметров  ) то что мы  открыли )
        if(!e) {// нулл если достиг конца директории или ошибка 
            if(errno) {//ошибка 
                perror("readdir");
                return 1;
            }
            break;// дошли до конца 
        }
        char * t;// переменная для типа 
        
        struct stat statbuf ; // структура для функфии стат 
        //statbuf.st_size = //
        if (stat(e->d_name, &statbuf)) {// стат принемяет имя файла и структуру в которую записывет данные --- так мы уже его сделали осталось только открыть 
           perror("stat");
           return 1;
        }
        
        switch (e->d_type) {// тоже самое что цепочка ифоф \ после свич идет то что мы сравниваем 
            case DT_REG:// если это имя то идем сюда 
                t = "reg";//
                break;//

            case DT_DIR://
                t = "dir";//
                break;//
            default:// если никуда не попали 
                t = "unk";//
        }
        printf("%s\t%ld\t%s\n", t, statbuf.st_size,   e->d_name);// тип , размер(в байтах)  и имя 
    }
    closedir(d);// закрываем директорию обхода 

    return 0;
}
