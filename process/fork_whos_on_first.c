/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/


//fork() 이후에 메시지를 쓰려고 경쟁하는 부모와 자식
/* Listing 24-5 */

#include <sys/wait.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int numChildren, j;
    pid_t childPid;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [num-children]\n", argv[0]);			// ./fork_whos_on_first 1
															//  출력 : 0 parent
														    //		  0 child

    numChildren = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-children") : 1;	//자식프로세스를 몇개 만들지를 인자값으로 받아옴

    setbuf(stdout, NULL);               /* Make stdout unbuffered */

    for (j = 0; j < numChildren; j++) {		//받은 인자 값만큼 자식 프로세스 생성
        switch (childPid = fork()) {
        case -1:								//fork() 리턴 값이 -1이면 에러 , 에러처리
            errExit("fork");

        case 0:								//child 출력하고 프로세스 종료
            printf("%d child\n", j);
            _exit(EXIT_SUCCESS);

        default:
            printf("%d parent\n", j);
            wait(NULL);                 /* Wait for child to terminate */  //자식프로세스가 종료되기를 기다림
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
