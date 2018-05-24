#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static void sigHandler(int sig)
{
		static int count=0;
	
		printf("sigGenRec : sig = %d, count = %d \n", sig, count);
		count++;
}

int main(void)
{	
		pid_t pid;
		int i;
		int fd, byteCount;
		char buffer[10];

		if(signal(SIGINT, sigHandler)==SIG_ERR)
		{
				perror("signal");
				return -1;
		}

		pause();
		fd = open("./test.txt", O_RDONLY);
		byteCount = read(fd,buffer,10);

		if(byteCount == 0)
			puts("Can't read test.txt");
		close(fd);

		pid = atoi(buffer);
		printf("pid = %d \n", pid);
	
		kill(pid,SIGINT); //pid값을 가지는 프로세서에 sigint시그날을 보냄
		for(i=0;i<4;i++)
		{
			pause();	//pause()에서 SIGINT시그날을 받으면 sigHandler()가 수행되고 다음 행이 수행;
			sleep(2);
			kill(pid,SIGINT);
		}
		return 0;
}
