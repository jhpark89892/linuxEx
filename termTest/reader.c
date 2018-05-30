#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>

#define SHMSIZE 1024

void handler (int signo)
{
	if(signo == SIGUSR1)		
		puts("공유 메모리를 다 채웠다는 시그널 받음");
}

int main(void)
{
	int ppid;					//부모프로세스 id
	int shmid;					//공유메모리 id
	int *shmaddr;				//논리메모리에서 사용할 공유메모리 주소
	void *shared_mem=(void*)0;	//공유메모리 접근 시작주소
	int i;
	int fd;						//파일 디스크립터
	int bytecnt;				//읽고 쓰기에 사용하는 바이트 수
	char buffer[BUFSIZ];		//문자열을 만들 임시버퍼

	if(signal(SIGUSR1, handler)==(void*)-1)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}

	//공유메모리 생성
	shmid = shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666|IPC_CREAT);
	if(shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	
	//공유메모리 attach
	shared_mem = shmat(shmid, (void*)0,0);
	if(shared_mem == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	
	shmaddr = (int*)shared_mem;

	//부모프로세스에게 공유메로리가 준비되어 시그널 보냄
	ppid =getppid();
	if(kill(ppid, SIGUSR2)==-1)
	{
		perror("kill");
		exit(EXIT_FAILURE);
	}

	//부모 프로세스로 부터 시그널 기다림
	pause();

	//파일 생성과 오픈
	fd = open("./data.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if(fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	//공유메모리에 접근하여 데이터 읽음
	for(i=0;i<1024;i++)
	{	
		//파일에 쓸 내용을 만듦
		sprintf(buffer,"shmaddr: %p, data: %d\n", shmaddr+i, *(shmaddr+i));
		
		//파일에 내용을 씀
		bytecnt = write(fd, buffer, strlen(buffer)+1);
	}
	
	//파일 닫기
	close(fd);

	//공유메모리의 데이터를 모두 읽어서 부모 프로세스에 시그널 보냄
	kill(ppid, SIGUSR2);

	//공유메모리 detach
	if(shmdt(shared_mem)==-1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
