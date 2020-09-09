#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
void transfer(int x) {
	int r = 1;//the size of massif
	int z = x;//10 number 

	while (z / 2 != 0) {
		r = r + 1;
		z /= 2;
	}
	
	int a[1000];
	for (int i = r - 1; i >= 0; i--) {
		a[i] = x % 2;
		x = x / 2;

	}
	for (int i = 0; i < r; i++) {
		printf("%d", a[i]);
	}
}

int main()
{
	int x;
	scanf("%d", &x);
	transfer(x);
}
