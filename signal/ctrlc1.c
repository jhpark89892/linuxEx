#include<signal.h>
#include<stdio.h>
#include<unistd.h>

void sigHandler(int sig)
{
		printf("OUCH! - I GOT SIGNAL %d \n", sig);
		(void) signal(SIGINT, SIG_DFL);
}

int main ()
{
		if(signal(SIGINT, sigHandler)==SIG_ERR)
		{
				perror("signal error!!");
				return -1;
		}

		while(1)
		{
				printf("Hello World \n");
				sleep(1);
		}

		return 0;
}
