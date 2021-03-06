#include <stdio.h>
#include <string.h>

void GenerateString(int n, char* string) {
	int rise = 0; // for a new now_size
	char letter = 'a';
	int now_size = 0;
	for (int i = 0; i <= n - 2; i++) {
		string[now_size + 1] = letter + 1;
		now_size += 1;
		// можно было бы использовать готовую ф-ю strncpy
		for (int i = 0; i < now_size; i++) {
			string[now_size + i + 1] = string[i];
			rise = now_size + i + 1;
		}

		now_size = rise;
		string[now_size + 1] = 0;
		letter += 1;
	}	
}


int main() {
	// не используйте транслит
	int col; // how many letter you will have 
	scanf("%d", &col);
	char string[1000];
	string[0] = 'a';
	GenerateString(col, string);
	if (col == 1) {
		printf("a");
	}
	else {
		printf("%s", string);
	}

	return 0;
}
