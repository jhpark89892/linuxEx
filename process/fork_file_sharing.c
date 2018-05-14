/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 24-2 */
//부모와 자식 사이에 파일 오프셋과 열린 파일 상태 플래그 공유하기


#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";	//임시파일 경로를 문자열로 저장

    setbuf(stdout, NULL);                   /* Disable buffering of stdout */

    fd = mkstemp(template);	//임시파일을 생성
    if (fd == -1)			//임시파일 생성 실패시 처리
        errExit("mkstemp");

    printf("File offset before fork(): %lld\n",		//fork하기 전에 file offset 출력
            (long long) lseek(fd, 0, SEEK_CUR));	//off_t가 62비트 환경에서long보다 큰 long long으로 정의 될 수 있기 때문에 long long 으로 명시적 형변환 

    flags = fcntl(fd, F_GETFL);		//F_GETFL : 파일상태 플래그 조회할때 사용, open()함수 호출시 설정한 플래그 값들을 반환 ex)O_CREAT, O_APPEND 같은 플래그
    if (flags == -1)
        errExit("fcntl - F_GETFL");
    printf("O_APPEND flag before fork() is: %s\n",
            (flags & O_APPEND) ? "on" : "off"); //파일상태 플래그가 O_APPEND이면 "on", 아니면 "off" 출력

    switch (fork()) {	//프로세스 생성
    case -1:			//프로세스 생성 실패 시 에러메시지 출력후 종료
        errExit("fork");

    case 0:     /* Child: change file offset and status flags */  //자식 프로세스가 처리하는 부분
        if (lseek(fd, 1000, SEEK_SET) == -1)	//생성한 임시파일의 파일 오프셋을 파일의 시작부터 1000으로 지정, 실패시 에러메시지 출력 후 프로세스 종료
            errExit("lseek");

        flags = fcntl(fd, F_GETFL);         /* Fetch current flags */   //현재 파일디스크립터(fd)가 가리키는 파일의 파일 플래그들을 가져와 flags에 저장
        if (flags == -1)
            errExit("fcntl - F_GETFL");
        flags |= O_APPEND;                  /* Turn O_APPEND on */  //O_APPEND플래그를 on 시킴
        if (fcntl(fd, F_SETFL, flags) == -1)
            errExit("fcntl - F_SETFL");
        _exit(EXIT_SUCCESS);

    default:    /* Parent: can see file changes made by child */	//부모 프로세스 수행부분
        if (wait(NULL) == -1)		//부모 프로세스만 수행하는 부분 //자식 프로세스가 종료되기를 기다림								
            errExit("wait");                /* Wait for child exit */
        printf("Child has exited\n");

        printf("File offset in parent: %lld\n",		//파일의 포인터가 가리키는 오프셋값을 출력
                (long long) lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);		//파일의 상태 플래그를 가져와 flags에 저장
        if (flags == -1)
            errExit("fcntl - F_GETFL");
        printf("O_APPEND flag in parent is: %s\n",	//파일의 상태 플래그값을 출력
                (flags & O_APPEND) ? "on" : "off");
        exit(EXIT_SUCCESS);
    }
}
