//data.txt파일을 읽어서 copy.txt로 복사하는 프로그램

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#define SHMSIZE 10


void handler(int signo)
{
	if(signo == SIGUSR2)
		puts("자식프로세스로부터 시그널 받음");
}

int main (int argc, char *argv[])
{
	int pid;						//자식 프로세스 id
	int status;						//종료 상태값 확인
	int shmid;						//공유메모리 id
	int *shmaddr;					//논리메모리에서 사용할 공유메모리 주소
	void *shared_mem = (void*)0;	//공유메모리 접근 시작주소
	int i;
	int fd;							//파일 디스크립터
	int bytecnt;

	if(argc<2)
	{
		printf("Usage : ./writer2 [filename]");
		exit(EXIT_FAILURE);
	}

	//자식 프로세스 생성
	pid = fork();
	switch(pid)
	{
		case -1:
			fprintf(stderr, "fork failed\n");
			exit(EXIT_FAILURE);
		case 0:
			if(execlp("./reader2", "reader2", (char*)NULL) == -1)
			{
				perror("execlp");
				exit(EXIT_FAILURE);
			}

		default:

			if(signal(SIGUSR2, handler) == (void*)-1)
			{
				perror("signal");
				exit(EXIT_FAILURE);
			}

			//공유메모리 생성
			shmid = shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);
			if(shmid == -1)
			{
				fprintf(stderr, "shmget failed\n");
				exit(EXIT_FAILURE);
			}

			//공유 메모리에 attach
			shared_mem = shmat(shmid, (void*)0, 0);
			if(shared_mem == (void*)-1)
			{
				fprintf(stderr, "shmat failed\n");
				exit(EXIT_FAILURE);
			}

			shmaddr = (int*)shared_mem;	

			//파일오픈
			fd = open(argv[1], O_RDONLY);
			if(fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}

			//execlp가 수행되는 시간 기다림
			pause();
			
			//파일 내용 읽어서 파일에 있는 총 바이트 수를 구함
			while((bytecnt = read(fd, (shmaddr+(sizeof(int)*2)), SHMSIZE))>0)
			{
				*(shmaddr+4) += bytecnt;
			}
			lseek(fd,0,SEEK_SET);

			
			*(shmaddr) = 10;
			//*(shmaddr)은 파일에 있는 총 파이트 수

			/*
			if(bytecnt == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}

			printf("부모 : total = %d\n", *(shmaddr));
			lseek(fd,0,SEEK_SET);

			while(1)
			{
				printf("부모 : while: total = %d\n", *(shmaddr));
				if(*(shmaddr) == 0)
						break;

				bytecnt = read(fd,(shmaddr+sizeof(int)), SHMSIZE);
				if(bytecnt ==-1)
				{
					perror("read");
					exit(EXIT_FAILURE);
				}
				sleep(2);
			}
			*/

			close(fd);
			
			/*
			//공유메모리에 데이터를 씀
			for(i=0;i<1024;i++)
			{
				*(shmaddr+i) = i+1;
				printf("shmaddr: %p, data: %d\n",shmaddr+i, *(shmaddr+i));

			}
			*/
			//데이터를 다 썼다고 자식 프로세스에게 시그널 보냄
			if(kill(pid, SIGUSR1)==-1)
			{
				perror("kill");
				exit(EXIT_FAILURE);
			}
			
			//자식 프로세스가 데이터를 다 읽었다고 시그널을 줄때까지 기다림
			pause();

			//공유메모리 detatch
			if(shmdt(shared_mem)==-1)
			{
				fprintf(stderr, "shmdt failed\n");
				exit(EXIT_FAILURE);
			}

			//공유메모리 제거
			if(shmctl(shmid, IPC_RMID, 0) ==-1)
			{
				fprintf(stderr, "shmctl(IPC_RMID) failed\n");
				exit(EXIT_FAILURE);
			}
	}


	exit(EXIT_SUCCESS);

}

