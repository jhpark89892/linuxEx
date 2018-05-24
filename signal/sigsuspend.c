#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

volatile sig_atomic_t quitflag;

static void sig_int(int signo)
{
		if(signo == SIGINT)
				printf("\n interrupt \n");
		else if(signo == SIGQUIT)
				quitflag = 1;
}

int main(void)
{
		sigset_t newmask, oldmask, zeromask;

		if(signal(SIGINT,sig_int)==SIG_ERR)
				perror("signal(SIGINT)error");
		if(signal(SIGQUIT, sig_int)==SIG_ERR)
				perror("signal(SIGQUIT) error");

		sigemptyset(&zeromask);
		sigemptyset(&newmask);
		sigaddset(&newmask, SIGQUIT);

		if(sigprocmask(SIG_BLOCK,&newmask, &oldmask)<0)	//newmask에 SIGQUIT를 블록함
				perror("SIG_BLOCK error");

		while(quitflag==0)
				sigsuspend(&zeromask);	//zeromask에 suspend했기때문에 SIGQUIT가 블록되지않음, SIGQUIT를 블록하려면 newmask 사용

		quitflag=0;

		if(sigprocmask(SIG_SETMASK, &oldmask, NULL)<0)
				perror("SIG_SETMASK error");
		exit(0);
}
