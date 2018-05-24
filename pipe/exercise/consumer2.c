/*   consumer.c   */
/***************************

  producer.c 코드에서 doc.txt파일을 읽어서 FIFO에 전달하고,
  consumer.c 에서 FIFO를 읽어서 doc2.txt파일을 생성하시오.

*****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define BUFFER_SIZE 256
#define FIFO_NAME "/tmp/fifo"

int main()
{
		int pipe_fd, file_fd, res;
		int readcount;
		int open_mode = O_RDONLY;
		char buffer[BUFFER_SIZE+1];
		int bytes_read = 0;
		memset(buffer,'\0',sizeof(buffer));
		
		printf("Process %d opening FIFO O_RDONLY\n", getpid());
		
		//  /tmp/fifo가 있는지 없는지 검사
		if(access(FIFO_NAME, F_OK) == -1)
		{
				//  /tmp/fifo를 0777접근 권한으로 생성
				res = mkfifo(FIFO_NAME, 0777);
				if(res !=0)
				{
						fprintf(stderr, "Could not create fifo %s\n",FIFO_NAME);
						exit(EXIT_FAILURE);
				}
		}

		//fifo파일 오픈
		pipe_fd = open(FIFO_NAME, open_mode);
		if(pipe_fd == -1)
		{
				perror("pipe_fd : open");
				exit(1);
		}
		//doc2.txt파일을 없으면 만들고 읽기 전용으로 0644접근 권한으로 생성
		file_fd = open("./doc2.txt", O_WRONLY|O_CREAT|O_TRUNC,0644);
		if(file_fd == -1)
		{
				perror("file_fd : open");
				exit(1);
		}
		
		//fifo의 내용을 buffer에 BUFFER_SIZE크기 만큼 가져옴
		while((readcount = read(pipe_fd, buffer, BUFFER_SIZE)) > 0)
		{		
				//buffer의 내용을 readcount크기만큼 file_fd 디스크립터가 가리키는 파일에 씀
				if(write(file_fd, buffer, readcount) != readcount)
				{
						perror("write");
						exit(1);
				}
		}
		if(readcount == -1)
		{
				perror("read");
				exit(1);
		}
		//열었던 파일일 닫음
		close(pipe_fd);
		close(file_fd);
		
		//doc.txt 파일과 doc2.txt파일이 같은지 비교
		system("diff -s doc.txt doc2.txt");

		return 0;
}
