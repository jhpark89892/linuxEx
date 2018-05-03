#define DEBUG

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main (void)
{
	int fd;
	char tmp[50];
	char rbuf[50];
	int wcount;
	int rcount;
	int pos;
	fd = open("./test.txt",O_RDWR | O_CREAT | O_TRUNC, \
	    		S_IRWXU | S_IWGRP | S_IRGRP | S_IROTH);

	if(fd ==-1)
	{
		printf("file open error!! \n");
		return -1;
	}

	// 버퍼에 테스트 문자열 만들기
	sprintf(tmp,"Do not count the egg before they hatch.");

	// test.txt파일에 문자열 쓰기
	wcount = write(fd,tmp,strlen(tmp));

	// 파일에 쓴 문자열 갯수 출력
	printf("wcount = %d \n",wcount);

	// 파일내의 커서 위치를 시작점 기준으로 offset 0번지로 이동
	pos = lseek(fd, 0, SEEK_SET);
	//lseek(fd, offset, SEEK_END);
	//lseek(fd, offset, SEEK_CUR);
 


	// 파일 읽기 -> rbuf배열에 채움
	rcount = read(fd, rbuf, strlen(tmp));
	
	// 읽은 문자열의 갯수를 확인 
	printf("rcount = %d\n", rcount);

	// 읽은 버퍼의 내용 확인
	printf("rbuf = %s\n", rbuf);

	
	

#ifdef DEBUG
	printf("fd = %d\n",fd);
#endif
	// 파일닫기
	close(fd);
/*
	fd = creat("./test2.txt", 0764);
	
	if(fd==-1)
	{
		printf("file creat  error!! \n");
		return -1;

	}
#ifdef DEBUG
	printf("fd = %d\n",fd);
#endif
*/
	return 0;
}
