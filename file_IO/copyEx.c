#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	int cntRead;
	char buf[BUF_SIZE];

	if(argc !=3 )
	{
		printf("usageErr : ./copyEx old-file new-file\n");
		return -1;
	}

	inputFd = open(argv[1], O_RDONLY);

	if(inputFd == -1)
	{
		perror(argv[1]);
		return -1;

	}
	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
		    S_IROTH | S_IWOTH;

	outputFd = open(argv[2], openFlags, filePerms);
	if(outputFd == -1)
	{
		perror(argv[2]);
		return -1;
	}

	while((cntRead = read(inputFd, buf, BUF_SIZE)) > 0)
		if(write(outputFd, buf, cntRead) != cntRead)
			printf("couldn't write whole buffer");

	if(cntRead == -1)
		perror("read");

	if(close(inputFd) == -1)
		perror("close input");
	if(close(outputFd) == -1)
		perror("close output");

	return 0;
}
