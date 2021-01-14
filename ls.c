#include "ls.h"
#include "headers.h"
#include <pwd.h>
#include<grp.h>
//	#include<dirent.h>
//int block;
void func(char dir_name[1024])
{
	struct stat a;
	stat(dir_name, &a);
	if(a.st_mode & S_IFREG)
		printf("-");
	else if(a.st_mode & S_IFDIR)
		printf("d");
	else if(a.st_mode & S_IFBLK)
		printf("b");
	else if(a.st_mode & S_IFCHR)
		printf("c");
	else if(a.st_mode & S_IFLNK)
		printf("l");
	else if(a.st_mode & S_IFIFO)
		printf("p");
	else if(a.st_mode & S_IFSOCK)
		printf("s");
	else if(a.st_mode & S_IFMT)
		printf("n");
	(a.st_mode & S_IRUSR)?printf("r"):printf("-");
	(a.st_mode & S_IWUSR)?printf("w"):printf("-");
	(a.st_mode & S_IXUSR)?printf("x"):printf("-");
	(a.st_mode & S_IRGRP)?printf("r"):printf("-");
	(a.st_mode & S_IWGRP)?printf("w"):printf("-");
	(a.st_mode & S_IXGRP)?printf("x"):printf("-");
	(a.st_mode & S_IROTH)?printf("r"):printf("-");
	(a.st_mode & S_IWOTH)?printf("w"):printf("-");
	(a.st_mode & S_IXOTH)?printf("x"):printf("-");
	printf(" %ld ", a.st_nlink);
	struct passwd user;
	user = *getpwuid(a.st_uid);
	printf("%s ", user.pw_name);
	struct group grp;
	grp = *getgrgid(a.st_gid);
	printf("%s ", grp.gr_name);
	printf("%8ld ", a.st_size);
	char time_m[1024];
	strcpy(time_m, ctime(&a.st_mtime));
	int length=strlen(time_m);
	time_m[length-1]= '\0';
	for(int i=4; i<=15; i++)
		printf("%c", time_m[i]);
	printf(" %s", dir_name);
	printf("\n");
}
void ls(char* tok, char* word)
{
	bool flag_l=0, flag_a=0;
	bool flag=1;
	if(strcmp(word, tok)!=0)
		tok=strtok(NULL, " ");
	else
		flag=0;
	int j=0;
	char dir[1024][1024];
	while(tok!=NULL && flag==1)
	{
		if(strlen(tok)>1)
		{
			if(tok[0]=='-')
			{
				tok[0]=='-';
				if(tok[1]=='a')
					flag_a=1;
				if(tok[1]=='l')
					flag_l=1;
				if(strlen(tok)>2)
				{
					if(tok[2]=='a')
						flag_a=1;
					if(tok[2]=='l')
						flag_l=1;
				}
			}
			else if(strlen(tok))
				strcpy(dir[j++], tok);
		}
		else if(strlen(tok))
			strcpy(dir[j++], tok);
		tok=strtok(NULL, " ");
	}
	int length=j;
	if(flag_l==0)
	{
		if(length==0)
		{
			struct dirent *dir_no;
			DIR *dir_t= opendir(".");
			while((dir_no = readdir(dir_t) )!= NULL)
			{
				if(dir_no->d_name[0]!='.' || flag_a==1)
					printf("%s ", dir_no->d_name);
			}
			printf("\n");
			closedir(dir_t);
		}
		else if(length==1)
		{
			struct dirent *dir_no;
			if(open(dir[0], O_DIRECTORY)<0)
			{
				printf("This directory doesn't exist\n");
				return;
			}
			DIR *dir_t= opendir(dir[0]);
			while((dir_no = readdir(dir_t) )!= NULL)
			{
				if(dir_no->d_name[0]!='.' || flag_a==1)
					printf("%s ", dir_no->d_name);
			}
			printf("\n");	
			closedir(dir_t);
		}
		else
		{
			for(j=0; j<length; j++)
			{
				struct dirent *dir_no;
				if(open(dir[j], O_DIRECTORY)<0)
				{
					printf("This directory doesn't exist\n");
					continue;
				}
				printf("%s:\n", dir[j]);
				DIR *dir_t= opendir(dir[j]);
				while((dir_no = readdir(dir_t) )!= NULL)
				{
					if(dir_no->d_name[0]!='.' || flag_a==1)
						printf("%s ", dir_no->d_name);
				}
				printf("\n");
				if(j!=length-1)
					printf("\n");
				closedir(dir_t);
			}
		}
	}
	else
	{
		//block=0;
		if(length==0)
		{
			struct dirent *dir_no;
			DIR *dir_t=opendir(".");
			while((dir_no = readdir(dir_t) )!= NULL)
			{
				if(dir_no->d_name[0]!='.' || flag_a==1)
					func(dir_no->d_name);
			}
			closedir(dir_t);
		}
		 if(length==1)
		{
			if(open(dir[0], O_DIRECTORY)<0)
			{
				printf("This directory doesn't exist\n");
				return;
			}
			char curr[1024];
			getcwd(curr, 1024);
			struct dirent *dir_no;
			chdir(dir[0]);
			DIR *dir_t= opendir(".");
			while((dir_no = readdir(dir_t) )!= NULL)
			{
				if(dir_no->d_name[0]!='.' || flag_a==1)
				{
					//printf("%s\n", dir_no->d_name);
					func(dir_no->d_name);
				}
			}	
			closedir(dir_t);
			chdir(curr);
		}
		else
		{
			for(j=0; j<length; j++)
			{
				if(open(dir[j], O_DIRECTORY)<0)
				{
					printf("This directory doesn't exist\n");
					continue;
				}
				char curr[1024];
				getcwd(curr, 1024);
				printf("%s:\n", dir[j]);
				struct dirent *dir_no;
				chdir(dir[j]);
				DIR *dir_t= opendir(".");
				while((dir_no = readdir(dir_t) )!= NULL)
				{
					if(dir_no->d_name[0]!='.' || flag_a==1)
						func(dir_no->d_name);
				}
				if(j!=length-1)
					printf("\n");
				closedir(dir_t);
				chdir(curr);
			}
		}
	}
}