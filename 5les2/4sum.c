#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int a[3] = {0,0,0};

void* thread1(void* dummy)
{
	for (int i = 0; i < 10000000; i++) {
		a[0]++;
		a[2]++;
	}
	return NULL;
}

void* thread2(void* dummy)
{
	for (int i = 0; i < 10000000; i++) {
		a[1]++;
		a[2]++;
	}
	return NULL;
}

int main() {
       // printf("a[1] = %d", a[1]);
	pthread_t thread_1, thread_2;
	
	pthread_create(&thread_1, NULL, thread1, NULL);
	pthread_create(&thread_2, NULL, thread2, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	printf("a[0] = %d\na[1] = %d\na[2] = %d\na[0] + a[1] = %d\n", a[0], a[1], a[2], a[0] + a[1]);
	return 0;
}
