/***********

쓰레드의 갯수와 숫자를 입력 받아 각각의 쓰레드가 입력받은 숫자의 합을 
구하는 프로그램

Usage : ./threadCreate 3 10000
result : 50005000


**************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THR_SIZE 10

int sum =0;
struct ARG
{
	int start;
	int end;
};
void* thread_summation(void *arg)
{
	struct ARG *my_arg = (struct ARG *)arg;

	printf("start = %d, end = %d\n", my_arg->start,my_arg->end);
	while(my_arg->start<=my_arg->end)
	{
		sum += my_arg->start;
		my_arg->start++;
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	pthread_t id_t[THR_SIZE];
	struct ARG *arg;
	arg = (struct ARG *)malloc(sizeof(struct ARG));
	int i;
	int thread_cnt;
	int num;
	int ADD_SIZE;

	if(argc!=3)
	{
		printf("Usage: %s <ThreadCnt> <Number>\n", argv[0]);
		exit(1);
	}

	thread_cnt = atoi(argv[1]);
	num = atoi(argv[2]);
	
	ADD_SIZE = num/thread_cnt;

	for(i=0;i<thread_cnt; i++)
	{
		arg->start=ADD_SIZE*i+1;
		arg->end=ADD_SIZE*(i+1);
		
		//나누어 떨어지지 않는 값에 대한 보정
		if(i==thread_cnt -1)
			arg->end= arg->end + (num % thread_cnt);

		printf("range[%d][0] = %d\n", i, arg->start);
		printf("range[%d][1] = %d\n", i, arg->end);
		
		
		pthread_create(&id_t[i], NULL, thread_summation, (void *)arg);
		pthread_join(id_t[i], NULL);
	}
	
	printf("result : %d \n", sum);
	return 0;

}
