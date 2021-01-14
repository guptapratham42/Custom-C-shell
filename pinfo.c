#include "headers.h"
#include "pinfo.h"
void pinfo(char* tok)
{
	int pid;
	tok=strtok(NULL, " ");
	if(tok==NULL)
	{
		pid=getpid();
	}
	else
	{
		pid=atoi(tok);
	}
	printf("pid -- %d\n", pid);
	char file_loc[1024]="/proc/";
	char pi[1024];
	sprintf(pi, "%d", pid);
	strcat(file_loc, pi);
	char exec[1024];
	strcpy(exec, file_loc);
	strcat(file_loc, "/status");
	FILE *fp;
	fp=fopen(file_loc, "r");
	char file_status[1024], temp[1024];
	while(file_status!=NULL)
	{
		strcpy(temp, file_status);
		fgets(file_status, 1024, fp);
		if(strcmp(temp, file_status)==0)
			break;
		char* token =strtok(file_status, ":");
		while(token!=NULL)
		{
			if(strcmp(token, "State")==0)
			{
				token =strtok(NULL, " ");
				char to;
				for(int i=0; i<strlen(token); i++)
				{
					if(token[i]!=' ')
					{
						to=token[i];
					}
				}
				printf("Process Status -- %c\n", to);	
			}
			else if(strcmp(token, "VmSize")==0)
			{
				token =strtok(NULL, ":");
				int mem;
				mem=atoi(token);
				printf("Memory(in kB) -- %d\n", mem);
			}
			else	
				token =strtok(NULL, " ");
		}
	}
	strcat(exec, "/exe");
	char path[1024];
	readlink(exec, path, 1024);
	printf("Executable Path -- %s\n", path);
}