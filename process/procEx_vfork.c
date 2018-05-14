/*
	문제2. 1 ~ 10000까지 더하는 프로그램을 작성하시오 (vfork()함수 이용)
	fork()함수를 사용하여 부모 프로세스에서는 1 ~ 5000
	자식 프로세스에서는 5001 ~ 10000을 더하는 프로그램을 작성하시오.
	부모 : 1 ~ 5000까지 더한 값 출력, 자식: 5001 ~ 10000까지 더한 값 출력

	부모 프로세스의 결과와 자식 프로세스의 결과를 더해서 최종 출력
	sum = sum1 + sum2
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
		pid_t pid;
		long  sum1=0, sum2=0;
		long sum =0;
		int i;
		puts("vfork 프로그램 시작");
		
		pid = vfork();
		switch(pid)
		{
				case -1:
						perror("vfork failed");
						exit(1);
				case 0:
						puts("자식 프로세스");
						for(i=5001;i<10001;i++)
								sum1 += i;
						printf("자식PID: %d,5001 ~ 10000의 합(sum1) : %ld \n",getpid(), sum1);
						exit(0);
		}
		
		puts("부모 프로세스");
		for(i=1;i<5001;i++)
		    sum2 += i;
		printf("부모PID:%d, 1 ~ 5000의 합sum2) : %ld \n",getpid(), sum2);


		sum = sum1 + sum2;
		printf("PID : %d, 1 ~ 10000의 합(sum) : %ld\n",getpid(), sum);
		
		return 0;
}
