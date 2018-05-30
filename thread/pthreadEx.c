/*****************************
  문제1. 1 ~ 200000까지 더하는 프로그램을 작성하시오.
  프로그램 실행시 arg값을 넘겨받아 실행하기
  argv[1] = 1인 경우,	one thread
  argv[2] = 2인 경우, 	two thread

*****************************/

#include<time.h>
#include<sys/time.h>
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


void* t_func1(void *data)
{
	int i;
	long sum=0;
	for(i=1;i<=500000;i++)
		sum +=i;
	return (void*)(sum);
}

void* t_func2(void* data)
{
	int i;
	long sum=0;
	for(i=500001;i<=1000000;i++)
		sum +=i;
	return (void*)(sum);
}

int main(int argc, char *argv[])
{
	pthread_t p_thread[2];
	int arg;
	int err;
	int i;
	long sum=0;
	long status1, status2;
	struct timeval UTCtime_start, UTCtime_end;

	if(argc !=2)
	{
		printf("usage: ./pthreadEx (1 or 2)");
		exit(1);
	}
	
	arg = atoi(argv[1]);

	if(arg ==1)
	{
		gettimeofday(&UTCtime_start,NULL);
		for(i = 1; i<=100000; i++)
			sum += i;

		printf("one thread sum : %ld\n", sum);
		gettimeofday(&UTCtime_end, NULL);
		printf("one thread : time interval : %ld/%ld\n",
				(UTCtime_end.tv_sec-UTCtime_start.tv_sec), (UTCtime_end.tv_usec-UTCtime_start.tv_usec));

	}
	if(arg == 2)
	{
		gettimeofday(&UTCtime_start,NULL);
		if(err=pthread_create(&p_thread[0], NULL, t_func1, NULL)<0)
		{
			perror("thread create error: ");
			exit(1);
		}


		if(err=pthread_create(&p_thread[1], NULL, t_func2, NULL)<0)
		{
			perror("thread create error:");
			exit(2);

		}

		pthread_join(p_thread[0], (void **)&status1);
		printf("thread join:status1: %ld\n", status1);
		pthread_join(p_thread[1], (void **)&status2);
		printf("thread join:staus2: %ld\n", status2);

		printf("two thread sum : %ld\n", status1+status2);
		gettimeofday(&UTCtime_end, NULL);
		printf("one thread : time interval : %ld/%ld\n",
				(UTCtime_end.tv_sec-UTCtime_start.tv_sec), (UTCtime_end.tv_usec-UTCtime_start.tv_usec));

	}
	return 0;
}
