#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

size_t Split(char* input_string, char* share, char** new_strings)
{
    char* replacement = strtok(input_string, share);
    size_t i = 0;
    for (i = 0; replacement != NULL; i++) {
        new_strings[i] = replacement;
        replacement = strtok(NULL, share);
    }
    return i;
}


int main(){
        char input_string[100];
        int size = 1000;
        char** new_strings = (char**)calloc(size, sizeof(char*));
        char share[] = " \n\t.,";

        FILE *file;
	file = fopen("plan.txt", "r");
 
	
	int N =  10;
	//char arr[N];
	
 
	/*while (fgets (input_string, N, file) != NULL){
	        size_t str_num = Split(input_string, share, new_strings);
               for (size_t i = 0; i < str_num; i++) {
                    system(new_strings[i]);
		     printf("%s\n", new_strings[i]);
               }  
	       
	}*/
	
	
	while (fgets (input_string, N, file) != NULL){
	        Split(input_string, share, new_strings);
                int x  = atoi(new_strings[0]);
                system(new_strings[1]);
		 sleep( x ); 
		 kill(1, 1); 
		 
		 
		  
                 
	       
	}
	
 
	printf("\n");
	fclose(file);

   
   return 0; 
}
