#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{	
	int stauts;
	pid_t pid;

	//자식 프로세스 생성
	pid = fork();
	switch(pid)
	{
		case -1:
				perror("Failed fork()");
				exit(-1);
		case  0:
				execlp("./hello","hello","",(char*)NULL);
				exit(0);
		default:
				puts("parents process");
				printf("child PID = %ld \n", (long)pid);
	}

	pid = wait(&stauts);
	printf("exit process PID = %ld \n", (long)pid);

	return 0;
}
