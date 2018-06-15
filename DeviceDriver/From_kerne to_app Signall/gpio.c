#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>

#define BUF_SIZE 100

int fd = -1;

static void signal_handler(int signum)
{
    int rcount;
    char buf[BUF_SIZE];

    puts("user app : signal is catched");
    if(signum = SIGIO)
    {
	puts("SIGIO");
	rcount = read(fd,buf,20);
	if(rcount == -1)
	{
	    perror("signal_handler : read()");
	    exit(1);
	}
	
	printf("read : buf : %s\n",buf);
    }
}

int main(int argc, char **argv)
{
	char buf[BUF_SIZE];
	int count, retval;
	//int fd=-1;

	memset(buf, 0, BUF_SIZE);
	signal(SIGIO,signal_handler);

	printf("GPIO Set : %s\n", argv[1]);
	while(fd<0)
	{
	    fd = open("/dev/gpioled", O_RDWR);
	    printf("errno :%d\n", errno);

	    if(errno == ENXIO)	//ENXIO == 6;
	    {
		retval = system("sudo insmod gpio2SW_module.ko");
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

	sprintf(buf, "%s:%d",argv[1], getpid());
	count = write(fd,buf, strlen(buf));
	printf("buf : %s\n",buf);
	if(count<0)
		printf("Error : write()\n");

	count = read(fd, buf, 20);
	
	printf("Read data : %s\n", buf);
	
	while(1);
	close(fd);
	printf("/dev/gpioled closed\n");

	return 0;
}

