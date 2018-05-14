#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main()
{
		pid_t pid;

		sleep(5);
		printf("\nprocess ID = %d \n", pid = getpid());
		printf("parent process ID = %d \n", getppid());
		printf("real user Id = %d \n", getuid());
		printf("effective user id = %d \n", geteuid());
		printf("real group id = %d \n", getgid());
		printf("effective group id = %d \n", getegid());
		return 0;
}
