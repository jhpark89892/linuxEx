#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static void usr1Handler(int sig)
{
	int fd, byteCount;
	char buffer[100] = "SIGUSR1";
	printf("usr1Handler : signal : %s\n", strsignal(sig));
	fd=open("./sigtest.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
	if(fd == -1)
	{
			perror("open");
			exit(1);
	}

	byteCount = write(fd, buffer, strlen(buffer));
	if(byteCount != strlen(buffer))
	{
			perror("usr1 : write");
			exit(1);
	}
	write(fd,"\n",1);
	close(fd);
	puts("SIGUSR1 쓰기");

}

static void usr2Handler(int sig)
{
	int fd, byteCount;
	char buffer[100] = "SIGUSR2";

	printf("usr2Handler: signal: %s\n",strsignal(sig));
	fd=open("./sigtest.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
	if(fd==-1)
	{
			perror("open");
			exit(1);
	}

	byteCount = write(fd, buffer, strlen(buffer));
	if(byteCount != strlen(buffer))
	{
			perror("usr2 : write");
	}
	write(fd,"\n",1);
	close(fd);
	puts("SIGUSR2 쓰기");

}

static void intHandler(int sig)
{
	puts("SIGINT 호출");
	exit(0);
}

int main(void)
{
		int i;
		if(signal(SIGINT, intHandler)==SIG_ERR)
		{
					perror("sigINT:signal");
					return -1;
		}

		if(signal(SIGUSR1, usr1Handler)==SIG_ERR)
		{
				perror("usr1:signal");
				return -1;
		}
		if(signal(SIGUSR2, usr2Handler)==SIG_ERR)
		{
				perror("usr2:signal");
				return -1;
		}

				pause();
				pause();
				pause();
				pause();
	
		//pause();
		
}
