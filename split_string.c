#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




size_t Split(char* input_string, char* share , char** new_strings)
{
    char* replacement = strtok(input_string, share);
    size_t i = 0;
    for (i = 0; replacement != NULL; i++) {
        new_strings[i] = replacement;
        replacement = strtok(NULL, share);
    }
    return i;
}

int main()
{
	char input_string[100];
	fgets(input_string, 100, stdin);
    int size = 1000;
    char** new_strings = (char**)calloc(size, sizeof(char*));
    char share[] = " \n\t.,";
    size_t str_num = Split(input_string, share , new_strings);
    for (size_t i = 0; i < str_num; i++) {
        printf("%uword: %s\n", i + 1, new_strings[i]);
    }
    return 0;
}
