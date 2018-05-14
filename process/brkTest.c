#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
		char *ptr;
	
		printf("before end data seg addr : %10p\n", sbrk(0));
		ptr = (char*)malloc(0x1000);
		printf("after end data seg addr : %10p\n", sbrk(0));
		
		if(sbrk(0x1000) == (void*)-1)
		{
				puts("sbrk error!!");
		}
		printf("call sbrk(0x1000) : %10p \n", sbrk(0));

		free(ptr);

		return 0;
}
