#include<stdio.h>

void dalArry(int (*arr)[10], int size)
{
	int i,x,y,sw,num;
	
	x = 0;
	y = 0;
	sw = 1;
	num = 1;

	while(size)
	{	
		for(i = 0; i< size; i++)	
		{
			arr[y][x] = num++;
			x += sw;
		}
		
		x -= sw;
		y += sw;
		size --;

		for(i = 0; i < size; i++)
		{
			arr[y][x] = num++;
			y += sw;
		}

		y -= sw;
		sw *= -1;
		x += sw;
	}
}

int main()
{
	int matrix[10][10] = {0,};
	int i,j;
	int size;
		
	printf("size : ");
	scanf("%d", &size);
	
	dalArry(matrix, size);

	

	puts("* 결과출력 *");
	for(i=0; i<size; i++)
	{
		for(j=0;j<size;j++)
		{
			printf("%3d ", matrix[i][j]);
		}
		printf("\n");
	}
	return 0;
}
