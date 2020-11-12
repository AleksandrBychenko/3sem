#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>



#define TABLE_SEMAPHORE "/table"
#define TABLE_PIPE "/tmp/table"

#define MAX_LINE_LENGTH 100 
#define SEPARATOR ':'
#define ROLE_WASH "wash"
#define ROLE_DRY "dry"
#define TIME_SUFFIx ".time"
#define INPUT_SUFFIx ".input"

sem_t * table_sem;
int table_fifo;

static volatile int alive = 1;

void signal_handler(int x) {
    alive = 0;
}



typedef struct entry_t {
    const char * name;
    int value;
    struct entry_t * next; // указатель на структуру 
} entry;


//функция загрузки из  файла (используя структуру )
entry* load(const char * filename) {
    char buff[MAX_LINE_LENGTH];
    entry* head = NULL;// голова цепочки 
    
    entry* tail = NULL; //ссылка на последний элемент 

    FILE* f= fopen(filename, "r");// открытие файла 
    if(!f) {
        perror(filename);
        exit(1);
    }
    
    
    
    

    while(!feof(f)) {// считывапем до конца файла 
        if(1!=fscanf(f, "%s", buff)) { //проверка что она считала хотя бы 1 строчку (fscanf -  возращает сколько переменных  она считала 
            continue;
        }
        
        
        char* sep = strchr(buff, SEPARATOR); //  находит место где нахдится в строке сепаратор или можно заменить ':'
        /*
        т оже самое но через  ваил 
        char * sep 
        for (int i;i < MAX_LINE_LENGTH ;i++){
           if (buff[i] == ':'){
              sep = &buff[i];
              break;
              }
        }
        */
        
        
        if(!sep) { //если не нашли сепаратор 
            printf("%s: ingore line, no separator `%c`\n", filename, SEPARATOR);
            continue;
        }
        
        *sep = '\0';// там где сепаратор ставим \0 и таким образом разделяем 

        entry* e = malloc(sizeof(entry)); //выделяем сылочку под 1 имя и заполняем его 
        e->name = strdup(buff); //дублирует строчку до \0
        e->value = atoi(sep+1); // преобразует строку в целое число с координатоми от сеп + 1 до конца  либо туда вписывается массив ( а ) который мы преобразуем 

        if(head) { //если голова заполнена то мы вешеемся в конец 
            tail->next = e;
            tail = e; //новй хвост 
        } else {
            head = e;
            tail = e;
        }

    }
    fclose(f); //закрывем файл 

    if(!head) { //ругаемся если мы запеись не нашли 
        printf("no records in %s\n", filename);
        exit(1);
    }

    return head;
}





// ищем по цепочке то что нам нужно 
int find(entry * e, const char * name) {
    while(e && strcmp(name, e->name)>0) { //сравнение строк покак они не совпадут идем дальше ( е может стать нулем в конце )
        e = e->next;   // пеерход на следующую структуру 
    }
    if(!e) {
        printf("no entry for key `%s`", name);
        exit(1);
    }
    return e->value;
}



void wash() {
    entry* times = load(ROLE_WASH TIME_SUFFIx); //  файл с таблицами временами записываем в семафор 
    entry* input = load(ROLE_WASH INPUT_SUFFIx); // файл с таблицами каое количество отробатывет записываем в структуры 

    while (input) { // проходим 1 файл  - структуру до нулл 
        int t = find(times, input->name); // встроенная функция находим в табличке таймз имя совподающее с нашим текущим 
        for(int i=0; i<input->value; i++) {//
            printf("wash %s (%d)\n", input->name, t);
            sleep(t);
            printf("  cleaned\n");
            int table_slots;

            if(sem_getvalue(table_sem, &table_slots) < 0) { // Функция sem_getvalue возвращает текущее значение семафора, помещая его в целочисленную переменную, на которую указывает valp  int sem_post(sem_t *sem); аналогично 
                perror("sem_getvalue");
                exit(1);
            }
            printf("  slots: %d\n", table_slots);
            

            if(sem_wait(table_sem)<0) {
                perror("sem_wait");
                exit(1);
            }

            write(table_fifo, input->name, strlen(input->name)+1);//запишет в фифо ( это  канал ) , что , какого размера 

            printf("  done!\n");

        }
        input = input->next; //продвигаемся по нему 
    }
}

void dry() {
    entry * times = load(ROLE_DRY TIME_SUFFIx);
    char name[MAX_LINE_LENGTH];

    while(1) {
        int offset = 0;
        while(1) {
            int len = read(table_fifo, name+offset, 1);// связываемся через  файл фифо мкежду вытиранием и мойкой ... в буфер нейм запишем со смещение офсет 1 байт из какнала 
            if(len != 1) {
                return;
            }
            if(name[offset] == '\0') {
                break;
            }
            offset ++;
        }
        //прочли из  потока слово
        
        if(sem_post(table_sem) < 0) { // освобождаем стол 
            perror("sem_post");
            return;
        }
        
        int t = find(times, name); //сколько времени ждать 
        printf("dry %s (%d)\n", name, t);
        sleep(t);
        printf("  done!\n");
    }
}

int get_table_limit() { //берет тейбел лимит 
    const char *s = getenv("TABLE_LIMIT"); // Функция getenv() возвращает указатель на информацию об окружении, ассоциированную со строкой  в таблице информации окружения. 
    if(!s) {
        printf("no TABLE_LIMIT env\n");
        exit(1);
    }
    return atoi(s); //размер
}

void table_create() {
    sem_unlink(TABLE_SEMAPHORE);// удаляем если была 
    table_sem = sem_open(TABLE_SEMAPHORE, O_CREAT + O_EXCL, 0777, get_table_limit());//создаем семафору 
    if(table_sem == SEM_FAILED) {
        perror("create `" TABLE_SEMAPHORE "` failed");
        exit(1);
    }

    mkfifo(TABLE_PIPE, 0777); // создаем фифо "файл "
    printf("waiting for partner...\n");
    table_fifo = open(TABLE_PIPE, O_WRONLY); //открываем для записи 
    if(table_fifo == -1) {
        perror("creeate `" TABLE_PIPE "` failed");
        exit(1) ;
    }
}





void table_open() {
    table_sem = sem_open(TABLE_SEMAPHORE, 0);
    if(table_sem == SEM_FAILED) {
        perror("open `" TABLE_SEMAPHORE "` failed");
        exit(1);
    }

    printf("waiting for partner...\n");
    table_fifo = open(TABLE_PIPE, O_RDONLY);
    if(table_fifo == -1) {
        perror("open `" TABLE_PIPE "` failed");
        exit(1);
    }
}



int main(int argc, char **argv) { // параметры системы 

    //signal(SIGINT, signal_handler);
    if(argc != 2) { //если нет таки х файлов  на 2 файла 
        printf("no role given, expected `" ROLE_WASH "` or `" ROLE_DRY "`)");
        return 1;
    }

    if(!strcmp(argv[1], ROLE_WASH)) { //0 аргумент ссылка на имя программы  1 - 1 с1 аргумент 
        table_create();
        wash();
    } else {
        table_open();
        dry();
    }


    return 0;
}
