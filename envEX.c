#include <stdio.h>
#include <stdlib.h>

#define DEBUG

extern char** environ;

int main()
{
	setenv("MYDIR", "/home/pi",0);
	setenv("KERNEL_SRC", "/usr/src/linux",0);

	printf("MYDIR = %s\n", getenv("MYDIR"));
	printf("KERNEL_SRC = %s \n", getenv("KERNEL_SRC"));

	//현재 설정된 환경변수 값을 모두 출력할 경우
#ifdef DEBUG
	while(*environ)
	{
		printf("%s\n", *environ++);
	}
	return 0;
#endif
}
