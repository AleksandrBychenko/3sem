#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main()
{
	int N = 0;
	scanf("%d", &N);
	printf("Global process-  PID: %d\n", getpid());
	for(int i = 0; i < N; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			printf("Was_error\n");
			exit(1);
		}
		else if (pid == 0) {
			printf("New process %d. PID:[%d], PPID:[%d]\n", i, getpid(), getppid());
		}	
		else {
			int additionally_process = 0;
			wait(&additionally_process );
			printf("Child process %d (PID - %d) work ;  Exit from now  process (PID - %d)\n", i, pid, getpid());
			exit(0);
		}
	}
}
