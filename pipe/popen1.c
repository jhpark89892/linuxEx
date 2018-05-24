#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
		FILE *read_fp;
		char buffer[BUFSIZ + 1];
		int chars_read;

		memset(buffer, '\0', sizeof(buffer));
		read_fp = popen("cat popen*.c | wc -l", "r");
		if(read_fp != NULL)
		{
				chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);

				while(chars_read > 0)
				{
						buffer[chars_read -1 ] = '\0';
						printf("Reading:-\n%s\n",buffer);
						chars_read = fread(buffer, sizeof(char),BUFSIZ, read_fp);
				}
				pclose(read_fp);
				exit(EXIT_SUCCESS);
		}
		exit(EXIT_FAILURE);
}





/*
int main()
{
		FILE *read_fp;
		char buffer[BUFSIZ +1];
		int chars_read;

		memset(buffer, '\0', sizeof(buffer));
		read_fp = popen ("ps -ax", "r");

		if(read_fp !=NULL)
		{
				chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
				while(chars_read > 0 )
				{
						buffer[chars_read] = '\0';
						printf("Reading : \n%s\n",buffer);
						chars_read = fread(buffer, sizeof(char), BUFSIZ,read_fp);
				}
				pclose(read_fp);
				exit(EXIT_SUCCESS);
		}
		exit(EXIT_FAILURE);
}

*/



/*
int main()
{
		FILE *write_fp;
		char buffer[BUFSIZ +1];
		sprintf(buffer, "Oonce upon a time, there was... \n");
		write_fp=popen("od -c", "w");
		if(write_fp != NULL)
		{
				fwrite(buffer, sizeof(char), strlen(buffer),write_fp);
				pclose(write_fp);
				exit(EXIT_SUCCESS);
		}
		exit(EXIT_FAILURE);
}
*/


/* 출력결과

   $ ./popen2
   0000000 O n c e u p o n a t i m e
   0000020 , t h e r e w a s . . . \n
   0000037
   cf) $ echo “Once upon a time, there was...” | od -c

*/







/*
int main()
{
		FILE *read_fp;
		char buffer[BUFSIZ +1];
		int char_read;
		memset(buffer, '\0', sizeof(buffer));

		read_fp = popen("uname -a", "r");
		if(read_fp !=NULL)
		{
				char_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
				if(char_read > 0)
						printf("Output was : -\n%s\n", buffer);

				pclose(read_fp);
				exit(EXIT_SUCCESS);
		}

		exit(EXIT_FAILURE);
}

*/

//출력 결과
/*
./popen1
Output was:-
Linux gw1 2.4.20-8 #1 Thu Mar 13 17:54:28 EST 2003 i686 i686 i386
GNU/Linux

*/
