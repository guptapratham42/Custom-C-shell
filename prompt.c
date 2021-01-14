#include "prompt.h"
#include "headers.h"
#include <sys/utsname.h>
void prompt(char* init_loc, char* curr_loc) 
{
	int i;
	struct utsname buf;
	uname(&buf);
	char str[1024], directory[1024];
	directory[0]='~';
	if(strlen(curr_loc)>=strlen(init_loc))
	{
		bool flag=1;
		for(i=0; i<strlen(init_loc); i++)
		{
			if(init_loc[i]!=curr_loc[i])
				flag=0;
		}
		if(flag)
		{
			for(i=strlen(init_loc); i<strlen(curr_loc); i++)
			{
				directory[i+1-strlen(init_loc)]=curr_loc[i];
			}
			directory[i+1-strlen(init_loc)]='\0';
		}
		else
		{
			for(i=0; i<strlen(curr_loc); i++)
			directory[i]=curr_loc[i];
			directory[i]='\0';
		}
	}
	else
	{
		for(i=0; i<strlen(curr_loc); i++)
			directory[i]=curr_loc[i];
		directory[i]='\0';
	}
	getlogin_r(str, 1024);
	printf("<%s@%s:%s> ", str, buf.nodename, directory); 
}
