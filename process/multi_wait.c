/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

//여러 자식 프로세스를 생성하고 기다리기
/* Listing 26-1 */

#include <sys/wait.h>
#include <time.h>
#include "curr_time.h"              /* Declaration of currTime() */
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int numDead;       /* Number of children so far waited for */
    pid_t childPid;    /* PID of waited for child */
    int j;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)	 //main 함수의 인자 값 검사
        usageErr("%s sleep-time...\n", argv[0]);	// 사용법 : "./multi_wai 7 1"

    setbuf(stdout, NULL);           /* Disable buffering of stdout */

    for (j = 1; j < argc; j++) {    /* Create one child for each argument */  //받은 인자 갯수 만큼 자식 프로세스 생성
        switch (fork()) {
        case -1:
            errExit("fork");

        case 0:                     /* Child sleeps for a while then exits */
            printf("[%s] child %d started with PID %ld, sleeping %s "
                    "seconds\n", currTime("%T"), j, (long) getpid(),
                    argv[j]);
            sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));  //자식프로세스는 입력받은 값만큼 sleep
            _exit(EXIT_SUCCESS);

        default:                    /* Parent just continues around loop */
            break;
        }
    }

    numDead = 0;
    for (;;) {                      /* Parent waits for each child to exit */
        childPid = wait(NULL);		//부모 프로세스는 자식 프로세스가 종료되기를 기다리고 wait의 리턴값으로 종료된 프로세스의 id를 얻음
        if (childPid == -1) {
            if (errno == ECHILD) {		//wait함수는 더 더이상 종료될 자식프로세스가 없으면  error넘버로 ECHILD를 리턴
                printf("No more children - bye!\n");
                exit(EXIT_SUCCESS);
            } else {                /* Some other (unexpected) error */
                errExit("wait");
            }
        }

        numDead++;		// 종료된 자식 프로세스의 수를 카운트
        printf("[%s] wait() returned child PID %ld (numDead=%d)\n",
                currTime("%T"), (long) childPid, numDead);
    }
}
