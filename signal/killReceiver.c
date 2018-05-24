#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sigHandler(int sig)
{
		printf("killTest : I got signal %d \n", sig);
		(void)signal(SIGINT,SIG_DFL);
}

int main(void)
{
		signal(SIGINT,sigHandler);
		while(1)
		{
				puts("Hello World");
				sleep(1);
		}
}
