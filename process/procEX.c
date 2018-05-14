
/*
   문제 1 ~ 10000까지 더하는 프로그램을 작성하시오(fork()함수 이용)

   fork()함수를 사용하여 부모 프로세스에서는 1 ~ 5000
   자식 프로세스에서는 5001 ~ 10000을 더하는 프로그램을 작성하시오.
   부모 : 1 ~ 5000까지 더한 값 출력, 자식 : 5001 ~ 10000까지 더한 값 출력.

   (단, 자식프로세스가 종료되었을때 자식 프로세스가 정상적으로 종료되었는지
   시그널에 의해 종료되었는지 확인하는 구문을 넣으시오.)

*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	int sum, i;
	pid_t pid;

	pid = fork();
	switch(pid)
	{
			case -1:
					perror("fork faild");
					exit(1);
			case 0:
					sum = 0;
					for(i=5001;i<10001;i++)
							sum += i;
					printf("자식의 더한 값 : %d, getpid = %d \n",sum, getpid());
					break;
			default:
					sum = 0;
					for(i=1;i<5001;i++)
							sum += i;

					printf("부모의 더한 값 : %d, getpid = %d \n", sum, getpid());
					break;
	}

	if(pid!=0)
	{
			int stat;
			pid_t child_pid;

			child_pid = wait(&stat);
			
			if(WIFEXITED(stat))
					printf("정상 종료, exit staus = %d \n", WEXITSTATUS(stat));
			else if(WIFSIGNALED(stat))
					printf("비정상 종료, signal number = %d%s \n",
									WTERMSIG(stat),
									WCOREDUMP(stat) ? "(core file generated)":"");
			else if(WIFSTOPPED(stat))
					printf("child stopped, signal number = %d \n", WSTOPSIG(stat));
	}

	exit(0);
}
