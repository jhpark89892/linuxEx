/*
  환경변수를 저장하고 Hello.c를 컴파일 해주는 소스코드
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define DEBUG

extern char** environ;

int main()
{
	//0. Hello World를 출력하는 Hello.c 생성
	//1. cmd창에 export FILEDIR=/home/jhpark/gitTest/linuxEx/ 입력
	//2. cmd창에 export FILENAME=Hello.c 입력
	//3. echo $FILENAME 로 확인
	//4. gcc -o envEX2 envEX2.c 로 컴파일
	//5. ./Hello 로 결과확인

	//문자열 저장
	char strDIR[50];
	char strNAME[50];
	char strResult[50];	
#ifdef DEBUG
	printf("FILEDIR = %s\n", getenv("FILEDIR"));
	printf("FILENAME = %s\n", getenv("FILENAME"));
#endif

	sprintf(strDIR,"%s", getenv("FILEDIR"));
	sprintf(strNAME,"%s", getenv("FILENAME"));
	sprintf(strResult, "gcc -o Hello ");
	
	//strResult에 명령어를 덧붙여 최종 명령어 완성
	//gcc -o Hello /home/jhpark/gitTest/linuxEx/Hello.c 
	strcat(strResult,strDIR);
	strcat(strResult,strNAME);
#ifdef DRBUG
	printf("%s", strResult);
#endif
	//cmd창에 문자열을 출력하여 실행
	system(strResult);


	//현재 설정된 환경변수 값을 모두 출력할 경우
#ifdef DEBUG
	while(*environ)
	{
		printf("%s\n", *environ++);
	}
	return 0;
#endif
}
