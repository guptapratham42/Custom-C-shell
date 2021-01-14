#include "prompt.h"
#include "headers.h"
#include "trim.h"
#include "ls.h"
#include "pinfo.h"
int main()
{
	int fd_inp=open("test.txt", O_WRONLY);
	dup2(fd_inp, 0);
	char *com [1024];
	com[0]= "cat";
	com[1]= "test.txt";
	//strcpy(com[1], NULL);
	//if(!fork())
		execvp(com[0], com);
	return 0;
}