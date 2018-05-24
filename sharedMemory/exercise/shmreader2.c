#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<errno.h>
#include<signal.h>

#define SHMSIZE 100

struct msgst{
	long msg_type;
	int writer_pid;
	int reader_pid;
};

//시그널 핸들러
void handler(int signo)
{
	if(signo = SIGUSR1)
	{
		printf("Signal handler signal number : %d\n", signo);
		puts("데이터가 준비됨");
	}
}

int main()
{
	void *shared_mem=(void*)0;
	int shmid;
	int *shmaddr;
	int i;
	struct msgst msg;
	int msgidWtoR, msgidRtoW;
	long int msg_to_receive = 0;

	//시그널 등록
	if(signal(SIGUSR1, handler) == (void*)-1)
	{
		perror("signal");
		exit(1);
	}

	//메시지 큐 생성(from Writer to Reader)
	msgidWtoR = msgget((key_t)1234,0666 | IPC_CREAT);
	if(msgidWtoR == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	//메시지 받음
	if(msgrcv(msgidWtoR, (void*)&msg, sizeof(struct msgst), msg_to_receive, 0) ==-1)
	{
		fprintf(stderr, "msgrc failed with error: %d\n",errno);
		exit(EXIT_FAILURE);
	}

	//받은 메시지 출력
	printf("Recieved  writer's PID : %d\n", msg.writer_pid);

	//메시지 큐 삭제(from Writer to Reder)
	if(msgctl(msgidWtoR, IPC_RMID, 0)==-1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	
	//메시지 큐 생성(from Reader to Writer)
	msgidRtoW = msgget((key_t)5678,0666 | IPC_CREAT);
	if(msgidRtoW == -1)
	{
		fprintf(stderr, "sgget failed with error: %d\n",errno);
		exit(EXIT_FAILURE);
	}

	//PID확인
	msg.reader_pid = getpid();
	printf(" shmreader PID = %d\n", msg.reader_pid);
	
	//메시지 보냄
	if(msgsnd(msgidRtoW, (void*)&msg, sizeof(struct msgst),0)==-1)
	{
		fprintf(stderr,"msgsnd failed\n");
		exit(EXIT_FAILURE);
	}
		

	//1.shmget
	shmid = shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);
	if(shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	//2.shmat
	shared_mem = shmat(shmid, (void*)0,0);
	if(shared_mem == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Memory attached at %p\n", (int*)shared_mem);
	shmaddr = (int*)shared_mem;
	
	//SIGUSR1을 shmwriter2.c로 부터 기다림(쓰기가 준비됨))
	pause();

	//3. memory access
	for(i=0;i<SHMSIZE;i++)
	{
		printf("shmaddr: %p, data: %d\n", shmaddr+i, *(shmaddr+i));
	}
	
	//shmwriter2.c로 SIGUSR1 시그널을 보냄
	if(kill(msg.writer_pid, SIGUSR1)==-1)
	{
		perror("kill");
		exit(1);
	}
	
	//4. shmdt
	if(shmdt(shared_mem) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);

}
