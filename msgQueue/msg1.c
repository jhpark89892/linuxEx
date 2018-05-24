#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct my_msg_st{
	long int my_msg_type;
	char some_text[BUFSIZ];
};

int main()
{

	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;

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
	while(running)
	{
		//메시지 받음
		if(msgrcv(msgid, (void*)&some_data, BUFSIZ, msg_to_receive, 0)==-1)
		{
			fprintf(stderr,"msgrc failed with error: %d\n",errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s", some_data.some_text);
		//"end"를 받으면 running을 0으로 바꿔 while문 빠져나감
		if(strncmp(some_data.some_text, "end", 3) ==0)
		{
			running = 0;
		}
	}
	//메시지 큐 삭제
	if(msgctl(msgid, IPC_RMID, 0)==-1)
	{
		fprintf(stderr,"msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
		
}
