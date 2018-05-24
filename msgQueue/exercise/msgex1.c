#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct my_msg_st{
	long int my_msg_type;
	char name [10];
	int age;
	int id;
	char contiueflag[1];
};

int main()
{

	int running = 1;
	int msgid;
	struct my_msg_st* some_data;
	some_data = malloc(sizeof(struct my_msg_st));
	long int msg_to_receive = 0;
	char buffer[256];
	int fd, bytecnt;

	/*first, we set up the message queue. */
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	/*then the messages are retrieved from the queue, 
	 until an end message is encountered.
	 Lastly, the message queue is deleted.  */
	
	  //파일 생성 및 오픈
	  fd = open("./msgex.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	  if(fd ==-1)
	  {
		 perror("open");
		 exit(1);
	  }


	while(running)
	{
		//메시지 받음
		if(msgrcv(msgid, (void*)some_data, sizeof(struct my_msg_st), msg_to_receive, 0)==-1)
		{
			fprintf(stderr,"msgrc failed with error: %d\n",errno);
			exit(EXIT_FAILURE);
		}
		sprintf(buffer, "Name: %s, Age: %d, ID: %d\n", 
						some_data->name, some_data->age, some_data->id);
		printf("%s", buffer);

		//파일에  쓰기
		bytecnt = write(fd, buffer, strlen(buffer)+1);
		if(bytecnt == -1)
		{
			perror("write");
			exit(1);
		}

		//"n"를 받으면 running을 0으로 바꿔 while문 빠져나감
		if(strncmp(some_data->contiueflag, "n", 1) ==0)
		{
			running = 0;
		}
	}
	//파일 닫기
	close(fd);

	//메시지 큐 삭제
	if(msgctl(msgid, IPC_RMID, 0)==-1)
	{
		fprintf(stderr,"msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
		
}
