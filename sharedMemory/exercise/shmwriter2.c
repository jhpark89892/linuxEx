#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
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
		puts("데이터를 다 읽음");
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
	msgidWtoR = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgidWtoR == -1)
	{
		fprintf(stderr, "msgget failed with error :%d\n", errno);
		exit(EXIT_FAILURE);
	}

	//PID 확인
	msg.writer_pid = getpid();
	printf(" shmwriter PID = %d\n", msg.writer_pid);

	//메시지 보냄
	if(msgsnd(msgidWtoR, (void*)&msg, sizeof(struct msgst), 0) == -1)
	{
		fprintf(stderr, "msgsnd failed\n");
		exit(EXIT_FAILURE);
	}

	//메시지 큐 생성(from Reader to Writer)
	msgidRtoW = msgget((key_t)5678, 0666 | IPC_CREAT);
	if(msgidRtoW == -1)
	{
		fprintf(stderr, "msgget failed with error : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	//메시지 받음
	if(msgrcv(msgidRtoW, (void*)&msg, sizeof(struct msgst), msg_to_receive, 0) ==-1)
	{
		fprintf(stderr, "msgrc failed with error : %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	//받은 메시지 출력
	printf("Recieved Reader's PID : %d\n", msg.reader_pid);

	//메시지 큐 삭제(from Reader to Writer)
	if(msgctl(msgidRtoW, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
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
	printf("Memory attached at 0x%p\n", shared_mem);
	shmaddr = (int*)shared_mem;

	//3. memory access
	for(i=0;i<SHMSIZE;i++)
	{
		*(shmaddr+i) = i+1;
		printf("shmaddr: %p, data: %d\n", shmaddr+i, *(shmaddr+i));
	}

	//shmreader2.c로 SIGUSR1 시그널을  보냄
	if(kill(msg.reader_pid, SIGUSR1) == -1)
	{
		perror("kill");
		exit(1);
	}
	
	//시그널을 기다림(shmreader2.c가 SIGUSR1을 보냄_데이터를 다 읽었다는 의미)
	pause();

	//4. shmdt
	if(shmdt(shared_mem) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	//5. shmctl
	if(shmctl(shmid, IPC_RMID, 0) ==-1)
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
