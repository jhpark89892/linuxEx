#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char *argv[])
{
	pid_t pid;
	int i;

	if(argc != 2 || strcmp(argv[1], "--help")==0)
			printf("usage : %s pid \n", argv[0]);

	pid = atoi(argv[1]);
	
			kill(pid, SIGINT);		//open
			sleep(1);
			kill(pid, SIGUSR1);		//write log
			sleep(1);
			kill(pid, SIGUSR1);		//write log
			sleep(1);
			kill(pid, SIGUSR2);		//close
	

	return 0;
}
