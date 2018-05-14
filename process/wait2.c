//test.txt파일을 만들고 내용을 출력하기


#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void pr_exit(int status);

int main()
{
		pid_t pid;
		char *message;
		int fd;
		int n;
		int exit_code;

		puts("fork program starting");
		
		fd=open("./test.txt", O_WRONLY | O_CREAT, 0644);

		if(fd == -1)
		{
			perror("open");
			return -1;
		}

		pid = fork();

		switch(pid)
		{
				case -1: 
						perror("fork failed");
						exit(1);
				case 0:
						message = "This is the child\n";
						n=5;
						exit_code = 37;
						break;
				default:
						message = "This is the parent\n";
						n = 3;
						exit_code = 0;
						break;

		}

		for(;n>0;n--)
		{
				write(fd, message, strlen(message));
				sleep(1);
		}
		// 부모 프로세스인 경우 if문 실행
		if(pid !=0)
		{
				int stat_val;
				pid_t child_pid;
				
				//wait함수를 통해 자식 프로세스가 끝나기를 기다림
				child_pid = wait(&stat_val);
				
				pr_exit(stat_val);	
			/*	printf("Child has finished : PID = %d\n", child_pid);

				//자식 프로세스가 어떻게 종료되었는지 확인
				if(WIFEXITED(stat_val))
						printf("Child exited with code: %d \n",WEXITSTATUS(stat_val));
				else
						printf("Child terminated abnormally \n");
			
			*/
		}
		close(fd);
		exit(exit_code);

}

void pr_exit(int status)
{
		if(WIFEXITED(status))
				printf("normal termination, exit stauts = %d\n",
								WEXITSTATUS(status));
		else if(WIFSIGNALED(status))
				printf("abnormal termination, signal number = %d%s \n",
								WTERMSIG(status),
								WCOREDUMP(status) ? "(core file generated)":"");
		else if(WIFSTOPPED(status))
				printf("child stopped, signal number = %d \n",
								WSTOPSIG(status));
}
