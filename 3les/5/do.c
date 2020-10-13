#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main(){

        FILE *file;
	file = fopen("plan.txt", "r");
 
	
	int N =  10;
	char arr[N];
 
	while (fgets (arr, N, file) != NULL){
	        
	  system(arr);
		
	}
 
	printf("\n");
	fclose(file);

   
   return 0; 
}
