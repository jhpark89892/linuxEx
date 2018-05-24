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
		static int count =0;

		printf("sigGenTran : sig = %d, count = %d \n", sig, count);
		count++;
}


int main(int argc, char *argv[])
{
		pid_t pid;
		int i;
		int	fd, byteCount;
		char buffer[10];

		if(argc != 2 || strcmp(argv[1], "--help")==0)
				printf("usage : %s pid \n", argv[0]);
		
		if(signal(SIGINT, sigHandler)==SIG_ERR)
		{
				perror("signal");
				return -1;
		}

		pid = getpid();
		sprintf(buffer, "%d",pid);
		fd=open("./test.txt", O_RDWR | O_CREAT |O_TRUNC, \
								S_IRWXU | S_IWGRP | S_IROTH);
		byteCount=write(fd,buffer,strlen(buffer));
		close(fd);

		pid = atoi(argv[1]);
		printf("send signal proc : %d \n",pid);
		
		for(i=0;i<5;i++)
		{
			sleep(1);
			kill(pid,SIGINT); //pid값을 가지는 프로세서에 SIGINT시그날을 보냄
			pause();		 // SIGINT시그날을 받으면 sigHandler()가 수행되고 다음 행이 수행됨
		}

		return 0;
	
}
