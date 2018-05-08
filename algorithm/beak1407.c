#include <stdio.h> 

int func(int num)			//입력 받은 값중에서 가장 큰 배수 리턴
{
	int count = 0, big = 1, i;
	while(1)
	{

		if(num%2==1)
		{
			break;
		}
		else
		{
			count++;
		}
		num = num/2;
	}

	if(count == 0)
		return big = 1;		//2의 0승은 1
	else
	{
		for(i=0;i<count;i++)
		{
			big = big*2;  //2의 1승은 2, 2의 2승은 4
		}
		
		return big;
	}
}


int main(void)  
{ 
	int num1, num2, res=0;
	printf("자연수 Num1 : ");
	scanf("%d", &num1);
	printf("자연수 Num2 : ");
	scanf("%d", &num2);
	
	while(num1<=num2)
	{
		res += func(num1++);
	}
	

	printf("%d \n", res);

return 0; 
}
