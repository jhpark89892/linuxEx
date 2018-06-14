//User Application

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BUF_SIZE 100

int main(int argc, char **argv)
{
	char buf[BUF_SIZE];
	int count, retval;
	int fd=-1;

	memset(buf, 0, BUF_SIZE);

	printf("GPIO Set : %s\n", argv[1]);
	while(fd<0)
	{
	    fd = open("/dev/gpioled", O_RDWR);
	    printf("errno :%d\n", errno);

	    if(errno == ENXIO)	//ENXIO == 6;
	    {
		retval = system("sudo insmod gpiofunction_module.ko");
		printf("System : Exit Status %d\n", retval);
	    }
	    else if(errno == ENOENT)	//ENOENT == 2;
	    {
		retval =  system("sudo mknod /dev/gpioled c 200 0");
		printf("System : Exit Status %d\n", retval);
		retval =  system("sudo chmod 666 /dev/gpioled");
		printf("System : Exit Status %d\n", retval);
	    }	
	}
	count = write(fd,argv[1], strlen(argv[1]));
	if(count<0)
		printf("Error : write()\n");

	count = read(fd, buf, 20);
	
	printf("Read data : %s\n", buf);

	close(fd);
	printf("/dev/gpioled closed\n");

	return 0;
}

