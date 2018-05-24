/*  producer2.c */
/*********************************
  producer.c 코드에서 doc.txt파일을 읽어서 FIFO에 전달하고,
  consumer.c 에서 FIFO를 읽어서 doc2.txt파일을 생성하시오.

  **********************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define FIFO_NAME "/tmp/fifo"

int main()
{
		int pipe_fd, file_fd;
		int bytecount;
		int res;
		char buffer[BUFFER_SIZE +1];

		
		file_fd = open("./doc.txt", O_RDONLY);
		if(file_fd == -1)
		{
				perror("file_fd : open");
				exit(1);
		}

		if(access(FIFO_NAME, F_OK)==-1)
		{
				res = mkfifo(FIFO_NAME, 0777);
				if(res != 0)
				{
						fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
						exit(EXIT_FAILURE);
				}
		}
		
		puts("fifo create : O_WRONLY");
		pipe_fd = open(FIFO_NAME, O_WRONLY);
		if(pipe_fd==-1)
		{
				perror("pipe_fd : open");
				exit(1);
		}
		while((bytecount = read(file_fd, buffer, BUFFER_SIZE)) > 0)
		{
				if(write(pipe_fd, buffer, bytecount) != bytecount)
				{
						perror("fifo : write");
						exit(1);
				}				
		}
		if(bytecount==-1)
		{
				perror("read");
				exit(1);
		}

		close(pipe_fd);
		close(file_fd);
		return 0;
}
