#include <stdio.h>
#include <unistd.h>

int global = 10;

int main(void)
{
	int i=20;
	pid_t pid;
	int status;

	switch(pid = fork())
	{
			case -1:
					puts("fork error!!");
					return -1;
			case 0:
					global = global + 10;
					i = i+10;
					break;
			default:
				global = global+100;
				i=i+100;
				break;
	}

	printf("pid = %d, global = %d, i = %d \n",getpid(),global,i);

	return 0;
}
