/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 4-3 */

#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    size_t len;		//size_t 시스템 데이터형 //부호없는 정수
    off_t offset;	//off_t  시스템 데이터형 //부호있는 정수
    int fd, ap, j;	
    char *buf;
    ssize_t numRead, numWritten;	//ssize_t 시스템 데이터형 //부호있는 정수

    //main 함수가 받는 인자값을 검사하여 올바른 명령을 받았는지 확인
    //잘못된 명령을 받았으면 사용법을 출력해주고 프로세스 종료
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file {r<length>|R<length>|w<string>|s<offset>}...\n",
                 argv[0]);

    //argv[1] 인자로 전달된 파일을 read,write 전용으로 열고(O_RDWR), 없으면 생성(O_CREAT)
    //그 파일의 권한은 rw-rw-rw 
    fd = open(argv[1], O_RDWR | O_CREAT,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH);                     /* rw-rw-rw- */
   
    // 파일 디스크립터 값을 검사
    // 0은 stdin, 1은 stdout, 2는 stderr, open()의 리턴값이 -1이면 open실패
    if (fd == -1)
        errExit("open");

   // r<length>|R<length>|w<string>|s<offset>  옵션 처리부분
    for (ap = 2; ap < argc; ap++) {
        switch (argv[ap][0]) {
        case 'r':   /* Display bytes at current offset, as text */
        case 'R':   /* Display bytes at current offset, in hex */
            len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
	    
            //입력받은 크기만큼 버퍼(메모리)생성
            buf = malloc(len);
           
           //malloc()는 메모리 할당을 실패할 경우 NULL을 반환하기 때문에
	   //NULL을 이용해서 에러처리 
	   if (buf == NULL)
                errExit("malloc");

	   //파일디스크립터(fd)가 가리키는 파일에서 len길이 만큼 읽어 buf에 저장
            numRead = read(fd, buf, len);
            if (numRead == -1)
                errExit("read");

	   //read()함수의 리턴값이 0이면 end-of-file 내용없는 빈파일
            if (numRead == 0) {
                printf("%s: end-of-file\n", argv[ap]);
            } else {
                printf("%s: ", argv[ap]);
                for (j = 0; j < numRead; j++) {
                     //'r'옵션이면 읽은 수만큼 루프를 돌면서 buf에 저장된 문자열을 하나씩 출력
		     //isprint()함수는 출력가능한 문자인지를 판별, 출력가능하면 그대로 출력, 
		     //아니면'?'을 출력 ex) '\t'탭 문자는 출력불가 
		    if (argv[ap][0] == 'r')
                        printf("%c", isprint((unsigned char) buf[j]) ?
                                                buf[j] : '?');

                    //'r'이 아닌 옵션 즉, 'R'옵션에 대해 16진수로 출력
                    // buf에 저장된 문자열을 명시적 형변환하여(unsigned int)출력
		    else
                        printf("%02x ", (unsigned int) buf[j]);
                }
                printf("\n");
            }

            free(buf);
            break;

        case 'w':   /* Write string at current offset */
	    //파일디스크립터fd가 가리키는 파일에 'w'옵션 이후의 문자열(&argv[ap][1])을
	    //문자열 길이만큼(strlen(&argv[ap][1]) 쓰기 
            numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
            if (numWritten == -1)
                errExit("write");

	   //쓰기한 문자열과 쓴 파일의 바이트 수 출력
            printf("%s: wrote %ld bytes\n", argv[ap], (long) numWritten);
            break;

        case 's':   /* Change file offset */
	    //getLogng()함수는 문자열을 정수로 변경하여 입력받은 문자열의 갯수를 리턴
            offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
            동
            //입력받은 문자열의 길이만큼 커서의 위치를 이
	    if (lseek(fd, offset, SEEK_SET) == -1)
                errExit("lseek");
            printf("%s: seek succeeded\n", argv[ap]);
            break;

        default:

	    //입력받은 옵션이 [rRws]가 아니면 에러 메시지 출력
            cmdLineErr("Argument must start with [rRws]: %s\n", argv[ap]);
        }
    }

    if (close(fd) == -1)
        errExit("close");

    exit(EXIT_SUCCESS);
}
