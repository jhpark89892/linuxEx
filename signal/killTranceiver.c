#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
		int s, sig;
		if(argc != 3 || strcmp(argv[1], "--help")==0)
				printf("%s pid sig-num \n", argv[0]);
		sig = atoi(argv[2]);
		s=kill(atoi(argv[1]),sig);
		//파라메터 입력이 정상적이지 않은 경우
		if(sig !=0)
		{		
				//정상적으로 kill함수가 호출되지 않은 경우
				if(s==-1)
					puts("Error : system call kill()");
		}
		else
		{
				//정상적으로 kill함수가 호출된 경우
				if(s==0)
					puts("Process exsists and we can send it a signal");
		}
	return 0;
}
