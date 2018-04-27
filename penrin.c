#include<stdio.h>
#include<string.h>
#define DEBUG

int main(void)
{
	int i;
	char strInput[51];
	int strSize;
	//1.문자열을 입력받는다.(최대 50문자 + NULL문자);
	scanf("%s",strInput);
#ifdef DEBUG
	printf("DEBUG : %s\n",strInput);
#endif

	//2.입력받은 문자열이 영문소문자인지 확인한다.
	//  그 이외의 입력을받게되면 메세지를 출력하고 종료
	for(i=0;i<strlen(strInput);i++)
	{
		if(!(strInput[i]>='a'&& strInput[i]<='z'))
		{
			printf("Error : strInput\n");
			return -1;
		}
	}
#ifdef DEBUG
	printf("DEBUG : strInput is ok\n");
#endif

	//3.팰린드롬 인지 확인한다.
        strSize = strlen(strInput);
	if(strSize%2==1)
		printf("strSize is odd \n");
	else
		printf("strSize is even \n");

	// strSize>>1 //strSize/2 와 동일
        for(i=0;i<(strSize>>1);i++)
	{
		if(!(strInput[i]==strInput[strSize-1-i]))
		{
			printf("strInput is not palindrome \n");
			return -1;	
		}
	}
	printf("strInput is palindrome \n");

}
