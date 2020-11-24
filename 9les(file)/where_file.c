#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

/*
  необходимо реализовать поиск файла на
  диске. Аргументом командной строки задаётся
  исходная директория, максимальная глубина
  поиска и имя файла, который ищем, например
  ./a.out . 2 my.file

  p.s при открытии директории (opendir) в
  качестве аругмента нужно передать либо
  абсолютный путь до неё, либо путь
  относительно текущей директории, откуда
  была запущена программа
*/

//делаем с помощю рекурсии 

int find(char * dirname, int level, char * filename) {//дирнейм директория ( папка в которой мы сейчас ищем )
    if(!level)//если уровень 0  то выйти обратно 
        return 0;

    DIR * d = opendir(dirname);//
    if(!d) {//
        perror("opendir");
        return 1;
    }
    while(1) {//
        struct dirent *e = readdir(d);
        if(!e) {//
            if(errno) {//
                perror("readdir");
                return 1;
            }
            break;//
        }

        if(!strcmp(".", e->d_name)) {//если точечка то не заходим а бежим дальше 
            continue;//
        }
        if(!strcmp("..", e->d_name)) {//то же что и . 
            continue;//
        }

        if(!strcmp(filename, e->d_name)) {//сравниваем наше имя  и с текущим 
            printf("%s/%s\n", dirname, filename);// пишем директорию и имя 
        }
        if(e->d_type == DT_DIR) {//если это директория ( нужно перейти на следующую 
            char * subdir = malloc(strlen(dirname)+1+strlen(e->d_name)+1);// создаем для новой папки место ( имя текущей директории + 1(разделительный слеш ) + текущей (наткнулись0+ зключительной единички (зкличительный нулик )
            sprintf(subdir, "%s/%s", dirname, e->d_name);//печатаем в субдир новый путь (в буфер )
            find(subdir, level-1, filename);//вызываем  файн рекурсивно 
        }
    }

    closedir(d);//
    return 0;//

}
int main(int argc, char ** argv) {//мейн принемает то что мы нам=писали 
    find(argv[1], atoi(argv[2]), argv[3]);//3 параметра принемаются как ( где мы ищем , глубина ( перевод в число ) ,  что мы  ищем  
    return 0;
}
