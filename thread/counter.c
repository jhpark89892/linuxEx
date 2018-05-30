#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

static int count =10;
static pthread_mutex_t countlock = 	PTHREAD_MUTEX_INITIALIZER;

void* increment(void)
{
	int error;
	while(1)
	{
		if(error=pthread_mutex_lock(&countlock) != 0)
		{
			perror("increment: mutex_lock");
			exit(1);
		}
		count++;
		pthread_mutex_unlock(&countlock);
		usleep(10);
	}
}

void* decrement(void)
{
	int error;
	while(1)
	{
		if(error=pthread_mutex_lock(&countlock) != 0)
		{   
			perror("decrement: mutex_lock");
			exit(1);
		}   

		count--;
	
		pthread_mutex_unlock(&countlock);
		usleep(10);
	}
}

void* getcount(int *countp)
{
	int error;
	while(1)
	{
		
		/*
		if(error=pthread_mutex_lock(&countlock) != 0)
		{   
			perror("increment: mutex_lock");
			exit(1);
		}   

		*countp = count;
		pthread_mutex_unlock(&countlock);
		*/
		sleep(1);
		printf("count = %d\n", count);
	}
}

int main ()
{
	pthread_t p_thread[3];
	int status;
	int err;

	if((err= pthread_create(&p_thread[0],NULL, increment, NULL)) != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	if((err= pthread_create(&p_thread[1],NULL, decrement, NULL)) != 0)
	{   
		perror("pthread_create");
		exit(1);
	}
	if((err= pthread_create(&p_thread[2],NULL, getcount, &count)) != 0)
	{   
		perror("pthread_create");
		exit(1);
	}

	err = pthread_detach(p_thread[0]);
	err = pthread_detach(p_thread[1]);
	err = pthread_detach(p_thread[2]);
	
	while(1);
	return 0;
}
