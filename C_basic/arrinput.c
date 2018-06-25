//char배열에 다양한 형식의 값을 포인터를 이용해 저장

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int i;
	char msg[30]={0,};

	printf("operand cnt : ");
	msg[0]=getchar();
	while(getchar() != '\n');
	msg[0] = msg[0] - '0';

	for(i=0; i<msg[0]; i++)
	{   
		printf("number %d: ",i+1);
		scanf("%d", (int *)&msg[4*i+1]);
		while(getchar() != '\n');
	}

	printf("operator : ");
	scanf("%c", &msg[4*i+1]);
	
	puts(" ");
	puts("*** output ***");
	printf("operand cnt: %d\n",msg[0]);
	for(i=0;i<msg[0];i++)
		printf("number%d : %d\n",i+1, msg[4*i+1]);

	printf("operator : %c\n", msg[4*i+1]);


}
