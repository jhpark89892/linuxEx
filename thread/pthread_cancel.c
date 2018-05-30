#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

//스레드 종료시 호출될 함수
void clean_up(void *);

//스레드 함수
void *thread_func(void *);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lmu = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	pthread_t pt;
	pthread_create(&pt, NULL, thread_func, NULL);

	//생성된 스레드 pt에 취소요청을 보낸다.
	pthread_cancel(pt);

	//5초를 쉰 후 시그널을 보낸다.
	sleep(5);
	pthread_cond_signal(&cond);

	//join 후 종료한다.
	pthread_join(pt, NULL);
	printf("exit\n");
	exit(1);
}

//스레드 종료시 호출될 함수
//여기에 자원해제루틴을 입력할 수 있을 것이다.
void clean_up(void *arg)
{
	puts("Thread cancel Clean_up function");
}

void *thread_func(void *arg)
{
	//DISABLE 상태다
	//스레드에 대한 취소요청을 무시한다.
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

	//스레드 종료시 호출될 함수 등록
	pthread_cleanup_push(clean_up, (void*) NULL);

	pthread_mutex_lock(&lmu);

	puts("THREAD cont wait");
	pthread_cond_wait(&cond, &lmu);
	puts("GET CONT SIGNAL");

	pthread_mutex_unlock(&lmu);

	puts("EXIT");
	pthread_cleanup_pop(0);
}
