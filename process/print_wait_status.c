/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Lesser General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the files COPYING.lgpl-v3 and COPYING.gpl-v3 for details.           *
\*************************************************************************/

/* Listing 26-2 */
//wait()와 관련된 시스템 호출이 리턴하는 상태값 출력하기

#define _GNU_SOURCE     /* Get strsignal() declaration from <string.h> */
#include <string.h>
#include <sys/wait.h>
#include "print_wait_status.h"  /* Declaration of printWaitStatus() */
#include "tlpi_hdr.h"

/* NOTE: The following function employs printf(), which is not
   async-signal-safe (see Section 21.1.2). As such, this function is
   also not async-signal-safe (i.e., beware of calling it from a
   SIGCHLD handler). */

void                    /* Examine a wait() status using the W* macros */
printWaitStatus(const char *msg, int status)
{
    if (msg != NULL)
        printf("%s", msg);

    if (WIFEXITED(status)) {										//자식 프로세스가 정상적으로 종료했을 때 참값을 리턴
        printf("child exited, status=%d\n", WEXITSTATUS(status));	//WEXITSTATUS(status)는 자식의 종료 상태를 리턴

    } else if (WIFSIGNALED(status)) {								//자식 프로세스가 시그널에 의해 종료됐을 때만 참값을 리턴
        printf("child killed by signal %d (%s)",
                WTERMSIG(status), strsignal(WTERMSIG(status)));		//WTERMSIG(status)는 프로세스를 종료시킨 시그널의 번호를 리턴
																	//strsignal(WTERMSIG(status))는 "(sig - 시그널번호 )" 문자열 리턴
																	
#ifdef WCOREDUMP        /* Not in SUSv3, may be absent on some systems */
        if (WCOREDUMP(status))										//프로세스가 코어덤프 파일을 생성했을 경우 참값을 리턴
            printf(" (core dumped)");
#endif
        printf("\n");

    } else if (WIFSTOPPED(status)) {								//자식 프로세스가 시그널에 의해 멈췄을 경우 참값을 리턴
        printf("child stopped by signal %d (%s)\n",					
                WSTOPSIG(status), strsignal(WSTOPSIG(status)));		//WSTOPSIG(status)는 프로세스를 멈추게한 시그널 번호를 리턴
																	//strsignal(WSTOPSIG(status))는 "(sig - 시그널번호)" 문자열 리턴

#ifdef WIFCONTINUED     /* SUSv3 has this, but older Linux versions and
                           some other UNIX implementations don't */
    } else if (WIFCONTINUED(status)) {								//자식 프로세스가 SIGCONT를 받고 재개됐을 경우 참값을 리턴
        printf("child continued\n");
#endif

    } else {            /* Should never happen */
        printf("what happened to this child? (status=%x)\n",
                (unsigned int) status);
    }
}
