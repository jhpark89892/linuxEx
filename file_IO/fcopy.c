#include <stdio.h>
#define BUF_SIZE 1024

int main (int argc, char **argv)
{
	int byteCount;
	FILE *infp, *outfp;
	char buf[BUF_SIZE];
	
	//실행시 파라메터의 입력(개수)이 정상인지 확인
	if(argc != 3)
	{
		//write(2,"Usage : fcopy file1 file2 \n",26 )
		fprintf(stderr," Usage : fcopy file1 file2 \n");
		return -1;
	}

	//원본파일을 열기
	if(infp = fopen(argv[1], "r")) == NULL)
	{
		perror(argv[1]);
		return -1;
	}
	
	//대상파일을 열기
	if((outfp = fopen(argv[2], "w")) == NULL)
	{
		perror(argv[2]);
		return -1;
	}

	while((byteCount = fread(buf,sizeof(char),BUF_SIZE, infp))>0)
		fwrite(buf, sizeof(char), byteCount, outfp);

	fclose(infp);
	fclose(outfp);

	return 0;
}
