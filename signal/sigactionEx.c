#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void ouch(int sig)
{
		printf("ouch : i got signal : %d\n", sig);
}

int main()
{
	struct sigaction newAct, oldAct;
	newAct.sa_handler = ouch;
	sigemptyset(&newAct.sa_mask);
	newAct.sa_flags = 0;
	sigaction(SIGINT,&newAct,&oldAct);
	while(1)
	{
			printf("Hello World! \n");
			sleep(1);
	}
}
