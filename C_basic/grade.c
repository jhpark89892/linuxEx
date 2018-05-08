#include <stdio.h>
#include <stdlib.h>
#define DEBUG 
#define GRADE_MAX 100

struct Grade{
	unsigned char name[8];
	unsigned char kor;
	unsigned char eng;
	unsigned char math;
	unsigned char german;
	unsigned short total;
	float average;
}Grade;
int main()
{
	struct Grade grade[GRADE_MAX];
	int i,j,n;
	n = sizeof(grade)/sizeof(Grade);

	for(i=0;i<n;i++)
	{
		for(j=0;j<8;j++)
		{
			grade[i].name[j] = rand()%26+65;
			if(j==7)
			{
				grade[i].name[j] = '\0';
			}

		}

		grade[i].kor = (rand()%100)+1;
		grade[i].eng = (rand()%100)+1;
		grade[i].math = (rand()%100)+1;	
  		grade[i].german = (rand()%100)+1;
		
	}
	
	for(i=0;i<n;i++)
		grade[i].total = grade[i].kor+grade[i].eng+grade[i].math+grade[i].german;

	for(i=0;i<n;i++)
		grade[i].average = (float)grade[i].total/4.0;
#ifdef DEBUG
	for(i=0;i<n;i++)
		printf(" %d. name : %s, kor : %d, eng = %d, math = %d, german = %d, total = %d, average = %.2f \n"
			,i, grade[i].name, grade[i].kor, grade[i].eng, grade[i].math, grade[i].german, grade[i].total, grade[i].average);

#endif

	return 0;
}
