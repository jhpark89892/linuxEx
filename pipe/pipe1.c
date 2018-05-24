#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>



int main()
{

		int data_processed;
		int file_pipes[2];
		const char some_data[] = "123";
		const char some_data2[] = "456";
		char buffer[BUFSIZ + 1];
		int status;

		pid_t fork_result;
		memset(buffer, '\0', sizeof(buffer));
		
		if(pipe(file_pipes) ==0)
		{
				fork_result = fork();
				if(fork_result == -1)
				{
						fprintf(stderr, "Fork failure");
						exit(EXIT_FAILURE);
				}
				if(fork_result==0)
				{
						//read함수는 플래그를 설정하지 않으면 블록킹 IO로 진행
						//따라서 파이프에 읽을 내용이 있을 때 까지 대기
						data_processed = read(file_pipes[0], buffer, BUFSIZ);
						printf("Read %d bytes: %s\n", data_processed, buffer);
						
						//data_processed=write(file_pipes[1], some_data2,strlen(some_data2));

						exit(EXIT_SUCCESS);

				}
				else
				{
						data_processed=write(file_pipes[1], some_data,strlen(some_data));
						printf("Wrote %d bytes\n", data_processed);

						//동기화 되지 않아 무한정 대기하는 경우 발생
						//data_processed = read(file_pipes[0], buffer, BUFSIZ);
						//printf("Read %d bytes: %s\n", data_processed, buffer);
				}
		}
		//wait(&status);
		exit(EXIT_SUCCESS);

}






/*******
int main()
{
		int data_processed;
		int file_pipes[2];
		const char some_data[] = "123";
		char buffer[BUFSIZ + 1];
		memset(buffer, '\0', sizeof(buffer));
		
		if(pipe(file_pipes) == 0)
		{
				//내가 내것을 쓰고 읽으면 다른 프로세스에서 읽을 수 없다.
				//동기화를 위해서는 시그널과 같은 보조 루틴이 필요하다.
				data_processed = write(file_pipes[1], some_data, strlen(some_data));
				printf("Wrote %d bytes \n", data_processed);
				data_processed=read(file_pipes[0], buffer, BUFSIZ);
				printf("Read %d bytes : %s \n", data_processed,buffer);
				exit(EXIT_SUCCESS);
		}
		exit(EXIT_FAILURE);
}
*************/
