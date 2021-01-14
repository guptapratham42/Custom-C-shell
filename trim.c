#include "trim.h"
#include "headers.h"
void trim(char* temp, char* word)
{
	int i, j=0, flag=1;
	for(i=0; i<strlen(temp); i++)
	{
		if(temp[i]!=' ' && temp[i]!='\t')
		{
			word[j++]=temp[i];
			flag=0;
		}
		else
		{
			if(flag==0)
			{
				word[j++]=' ';
				flag=1;
			}
		}
	}
	word[j]='\0';
}