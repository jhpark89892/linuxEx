#include <stdio.h>
#include <stdlib.h>

static void testExitHandler(int exitStatus, void *arg)
{
		printf("testExitHandler : exitStatus = %d , art = %ld \n",exitStatus, (long)arg );
}

int main(void)
{
		//종료 핸들러 등록
		if(on_exit(testExitHandler, (void*) 10) != 0)
			printf("exit Handler Program1 : Start!! \n");

		if(on_exit(testExitHandler, (void*) 20) != 0)
			printf("exit Handler Program2 : Start!!");

		//프로그램의 시작
		printf("exit Handler Program : start!! \n");
		
		//프로그램 종료
		exit(2);
}
