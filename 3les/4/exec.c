#include <unistd.h>
#include <sys/wait.h>
int main()
{	
	if (fork() == 0) {
		execlp("gcc", "gcc", "Hellow.c", "-o", "hello", NULL);
	}
	int status = 0;
	wait(&status);
	execlp("./hello", "./hello", NULL);
}
