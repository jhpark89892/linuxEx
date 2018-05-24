#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

static void sig_quit(int);
static void sigHandler(int);

int main(void)
{
	sigset_t newmask, oldmask, pendmask;

	if(signal(SIGQUIT,sigHandler) == SIG_ERR)
			perror("can't catch SIGQUIT");
	if(signal(SIGINT,sigHandler)== SIG_ERR)
			perror("cant's catch SIGINT");

	sigemptyset(&newmask);				//sigset_t 데이터형의 비트를 모두 0으로 설정
	sigaddset(&newmask, SIGQUIT);		//sigset_t 데이터형에 SIGQUIT에 해당하는 비트에 1을 설정
	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask)<0)	//설정된 sigset_t를 프로세스에 블록하라고 적용
			perror("SIG_BLOCK error");
	sleep(10);		//sleep은 시그날을 받으면 깨어남, SIGINT는 sigHandler함수 수행, SIGQUIT는 block 되어 아래에서 팬딩
	if(sigpending(&pendmask)<0)		//시그날의 pending 상태를 넣어 줌
			perror("sigpending error");
	if(sigismember(&pendmask, SIGQUIT))	//pending 상태를 확인, SIGQUIT에 해당하는 비트가 1인가? 묻는 것
			printf("\n SIGQUIT pending \n");
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL)<0)	//SIG_BLOCK한 것을 다시 원상복구
			perror("SIG_SETMASK error");
	printf("SIGQUIT unblocked \n");

	sleep(5);
	exit(0);
}

static void sig_quit(int signo)
{
		printf("caught SIGQUIT \n");
		if(signal(SIGQUIT, SIG_DFL)==SIG_ERR)
					perror("can't reset SIGQUIT");

}

static void sigHandler(int signo)
{
		printf("signal no : %d \n",signo);
}
