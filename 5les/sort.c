#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int comp(int a[], int x, int y) {
	
	return a[y] - a[x];
}

void sort(int a[], int n, int (*comp)(int*, int, int)) {
    for (int i = 0; i < n - 1; i++) {
        // сравниваем два соседних элемента.
        for (int j = 0; j < n - i - 1; j++) {
            if (comp (a , j , j + 1) > 0)  {
                // если они идут в неправильном порядке, то  
                //  меняем их местами. 
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

int main() {
	int arr[5] = { 5, 2, 9, 7, 66 };
	sort(arr, 5, comp);

	for (int i = 0; i < 5; i++)
		printf("%d\n", arr[i]);
	return 0;
}
