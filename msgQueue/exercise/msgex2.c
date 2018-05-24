/* The sender program is very similar to msg1.c. In the main set
   up, delete the msg_to_receive declaration and replace it
   with buffer[BUFSIZ], remove the message queue delete and
   make the following changes to the running loop. We now
   have a call to msgsnd to send the entered text to the queue.
   */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st{
	long int my_msg_type;
	char name[10];
	int age;
	int id;
	char contiueflag[1];
};

int main()
{
	int running=1;
	struct my_msg_st* some_data;
	some_data = malloc(sizeof(struct my_msg_st));
	int msgid;
	char buffer[10];

	msgid=msgget((key_t)1234, 0666 | IPC_CREAT);
	
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	while(running)
	{
		printf("Enter Name: ");
		scanf("%s", some_data->name);
		printf("Enter Age: ");
		scanf("%d",&some_data->age);
		printf("Enter ID: :");
		scanf("%d",&some_data->id);
		printf("Continue(y or n): ");
		scanf("%s", some_data->contiueflag);

		some_data->my_msg_type = 1;

		//메시지 보냄
		if(msgsnd(msgid, (void*)some_data, sizeof(struct my_msg_st),0)==-1)
		{
			fprintf(stderr,"msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		if(strncmp(some_data->contiueflag, "n",1)==0)
		{
			running =0;
		}
	}
	//메시지 큐의 삭제는 수신측에서 수행
	exit(EXIT_SUCCESS);	
}
