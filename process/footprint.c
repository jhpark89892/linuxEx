/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 24-3 */
/* 
   brk() 함수와 sbrk()함수는 프로세스의 Heap 세그먼트에 할당 메모리 양을 변경하기 위해서 사용
   brk()는 성공시 0, 실패시 -1을 반환, sbrk()는 성공시 세그먼트의 break주소(마지막 경계주소)를 반환, 실패시 -1리턴
  
  - brk()는 brk(0x8efe000)처럼 사용해서 0x8efe000까지 할당
    malloc(1000)을 해도 brk()시스템 콜이 사용됨


  - sbrk()는 sbrk(0x1000)처럼 사용해서 이전 세그먼트에서 + 0x1000 해주는 것
     데이터 세그먼트의 최하위 주소를 얻을 때 sbrk(0)을 호출해서 얻음

*/




#define _BSD_SOURCE     /* To get sbrk() declaration from <unistd.h> in case
                           _XOPEN_SOURCE >= 600; defining _SVID_SOURCE or
                           _GNU_SOURCE also suffices */
#include <sys/wait.h>
#include "tlpi_hdr.h"

static int
func(int arg)
{
    int j;
	
	 //heap 세그먼트에 메모리 할당 [(0x8000 * 0x100)+블록길이정보 사이즈 ]
    for (j = 0; j < 0x100; j++)	
        if (malloc(0x8000) == NULL)	
            errExit("malloc");
    printf("Program break in child:  %10p\n", sbrk(0));		//malloc에 의해 break point가 변경되어 할당되지않은 영역과 힙영역의 경계주소를 출력

    return arg;
}

int
main(int argc, char *argv[])
{
    int arg = (argc > 1) ? getInt(argv[1], 0, "arg") : 0;	//메인 함수로 입력 받은 인자를 검사하여 값이 없으면 0, 있으면 정수형으로 변환하여 arg에 저장
    pid_t childPid;
    int status;

    setbuf(stdout, NULL);           /* Disable buffering of stdout */

    printf("Program break in parent: %10p\n", sbrk(0));	 //함수 초기에는 데이터 세그먼트와 힙 세그먼트 사이의 경계주소 출력

    childPid = fork();			//자식 프로세스생성
    if (childPid == -1)			//자식 프로세스 생설 실패시 처리
        errExit("fork");

    if (childPid == 0)              /* Child calls func() and */
        exit(func(arg));            /* uses return value as exit status */	//func함수의 리턴값으로 경계주소(break point)를 받아 터미널창에 출력하고 자식 프로세스 종료

    /* Parent waits for child to terminate. It can determine the
       result of func() by inspecting 'status' */

    if (wait(&status) == -1)	//자식 프로세스가 종료되기를 기다림
        errExit("wait");

    printf("Program break in parent: %10p\n", sbrk(0));		//경계주소(break point) 출력, func가 종료되어 증가했던 heap영역이 반환되어 초기의 값과 동일한 값이 출력

    printf("Status = %d %d\n", status, WEXITSTATUS(status));   //WEXITSTATUS(status)는 자식 프로세스가 정상 종료되었을 때 반환한 값을 리턴

    exit(EXIT_SUCCESS);
}
