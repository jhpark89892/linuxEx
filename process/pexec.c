#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
		printf("Running ps whit execlp \n");
		execlp("ps","ps","-ax", NULL);  //NULL에 0넣으면 주소인데 값을 넣었다고 warning 나옴
										//요망한 것!!

		printf("난 출력되지 않아요");
		exit(0);
}
