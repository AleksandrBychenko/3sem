#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    int pfd;
    int fd[2];
    char x[100];
    int size=0;
    int a,b;
    
    if((pfd=pipe(fd))<0)
    {
        printf("folse");
        exit(1);
    }
    else
    {
        do
        {
            a=write(fd[1],"w",2);
            b=read(fd[0],x,1);
            size=(size+2);//write 2 bits 
            printf("%d\n",size);
        } 
        while(a != b);
    }
        
    return 0;
}
