#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
        int fd[2], result;
       
        
        if(pipe(fd) < 0){
            printf("Can\'t create pipe\n");
            exit(-1);
        }
        result = fork();
        
        
        if(result <0){
            printf("Can\'t fork child\n");
            exit(-1);
        
        } else if (result > 0) {
            int in[2];
            printf("Enter a and b:\n");
            scanf("%d %d", in, in + 1);
            if (write(fd[1], in, sizeof(int) * 2) != sizeof(int) * 2) {
		        printf("Can\'t read string\n");
		        exit(-1);
	      }
	      int from_another;
	      int status;

		    wait(&status);

		    if (read(fd[0], &from_another, sizeof(int)) != sizeof(int)) {
			     printf("Incorrect pipe read\n");
			     exit(-1);
		    }
		   close(fd[0]);
		   close(fd[1]);
	   	printf("a * b = %d\n", from_another);
	}
	
	else{
	     printf("Work. Waiting for data\n");
	   	 int input[2];

		  if (read(fd[0], input, sizeof(int) * 2) != sizeof(int) * 2) {
			   printf("Incorrect pipe read\n");
			   exit(-1);
		}
		  printf("Multiplicator gets enought data\n");
		  int go_away = input[0] * input[1];

		  if (write(fd[1], &go_away, sizeof(int)) != sizeof(int)) {
			  printf("Incorrect pipe write\n");
			  exit(-1);
		 }
		  printf("Multiplicator exiting\n");
		  close(fd[0]);
		 close(fd[1]);
		 exit(0);
	 } 
	
	
	
	return 0;
}
