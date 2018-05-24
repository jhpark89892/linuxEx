#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

static void sigHandler(int signo)
{
	 
	 static int fd;
	 time_t UTCtime;
	 int bytecount;
	 char buffer[256];
	 struct tm *tm;

	if(signo == SIGINT)
	{
	 	puts("SIGINT is catched");

		fd=open("./timelog.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
		if(fd==-1)
		{
				perror("sigint : open");
				exit(1);
		}

	}
	else if(signo == SIGUSR1)
	{		
			puts("SIGUSR1 is catched");
			
			time(&UTCtime);
			tm=localtime(&UTCtime);
			strftime(buffer, sizeof(buffer), "%Y-%m-%e %H:%M:%S %Y",tm);
			printf("SIGUSR1 : %s\n", buffer);

			bytecount=write(fd,buffer,strlen(buffer));
			if(bytecount != strlen(buffer))
			{
					perror("SIGUSR1 : write");
					exit(1);
			}
			write(fd,"\n",1);
	}
	else if(signo == SIGUSR2)
	{
			puts("SIGUSR2 is catched");
			close(fd);
			exit(0);
	}
}


int main(void)
{

		if(signal(SIGINT, sigHandler)==SIG_ERR)
		{
					perror("sigINT:signal");
					return -1;
		}	
		if(signal(SIGUSR1, sigHandler)==SIG_ERR)
		{
					perror("sigusr1:signal");
					return -1;
		}	
		if(signal(SIGUSR2, sigHandler)==SIG_ERR)
		{
					perror("sigusr2:signal");
					return -1;
		}
		
				pause();
				pause();
				pause();
				pause();
	
}
