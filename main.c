#include "prompt.h"
#include "headers.h"
#include "trim.h"
#include "ls.h"
#include "pinfo.h"
struct job_str
{
	int job_pid;
	char job_name[100];
	char state[100];
};
int no_jobs=0;
struct job_str jobs[100];
void fun(int k)
{
	int status;
	pid_t process=waitpid(-1, &status, WNOHANG);
	if(process>0)
		printf("The background process with Pid %d exited normally\n", process);	
}
void ctrlC(int sig)
{
	int i=0;
	pid_t pid;
	for(; i<no_jobs; i++)
	{
		pid=getpid();
		if(jobs[i].job_pid==pid)
		{
			exit(0);
			break;
		}
	}
	return;
}
void ctrlZ(int sig)
{
	return;
}
int main()
{
	bool shell_fl=0;
	char init_loc[1024];
	getcwd(init_loc, 1024);
	signal(SIGINT, ctrlC);
	signal(SIGTSTP, ctrlZ);
	//jobs a;
	while(1)
	{
		//if(shell_fl)
		//	return 0;
		int no_of_back=0, pid[1024];
		char *back[1024];
		char curr_loc[1024];
		getcwd(curr_loc, 1024);
		if(shell_fl==0)
			prompt(init_loc, curr_loc);
		char inp[1024];
		//scanf("%[^\n]", inp);
		//getchar();
		for(int ij=0; ij<1024; ij++)
			inp[ij]='\0';
		if(fgets(inp, 1024, stdin)==NULL)
		{
			shell_fl=1;
			printf("Shell exited\n");
			exit(0);
		}
		inp[strlen(inp)-1]='\0';
		char arg[1024][1024];
		char* token =strtok(inp, ";");
		int i=0;
		while(token!=NULL)
		{
			strcpy(arg[i++], token);
			token =strtok(NULL, ";");
		}
		int length=i;
		bool next_fl=0;
		for(i=0; i<length; i++)
		{
			getcwd(curr_loc, 1024);
			char word[1024], temp[1024], temp2[1024];
			trim(arg[i], word);
			int n=0;
			for(int ii=0; ii<strlen(word); ii++)
			{
				if(word[ii]=='|')
					n++;
			}
			strcpy(temp, word);
			strcpy(temp2, word);
			int filedes[2];
			int std_inp=dup(0), std_out=dup(1), ii=0;
			char* pip_tok=strtok(temp2, "|");
			while(pip_tok!=NULL)
			{
				trim(pip_tok, pip_tok);
				//printf("%s\n", pip_tok);
				int fd_temp=0;
				pipe(filedes);
				int pipe_pid=fork();
				if(pipe_pid!=0)
				{
					dup2(fd_temp, 0);
					if(ii!=n)
						dup2(filedes[1], 1);
					close(filedes[0]);
					char temp3[1024]="\0", temp4[1024]="\0";
					if(ii==0)
					{
						bool inp_red=0;
						char tem_inp[1024];
						strcpy(tem_inp, pip_tok);
						trim(tem_inp, tem_inp);
						//printf("%s\n", tem_inp);
						char* inp_tok=strtok(tem_inp, " ");
						while(inp_tok!=NULL)
						{
							if(inp_red)
							{
								int fd_inp=open(inp_tok, O_WRONLY, 0644);
								if(fd_inp <0)
								{
									//printf("%s\n", inp_tok);
									printf("No such input file\n");
									next_fl=1;
								}
								else
									dup2(fd_inp, 0);
								inp_red=0;
							}
							if(strcmp(inp_tok, "<")==0)
								inp_red=1;
							else
							{
								strcat(temp3, inp_tok);
								strcat(temp3, " ");
							}
							inp_tok=strtok(NULL, " ");
						}
					}
					if(next_fl)
						break;
					//printf("%s\n", temp3);
					strcpy(pip_tok, temp3);
					if(ii==n)
					{
						bool out_red=0, over=0, app=0;
						char tem_out[1024];
						strcpy(tem_out, pip_tok);
						trim(tem_out, tem_out);
						char* out_tok=strtok(tem_out, " ");
						while(out_tok!=NULL)
						{
							if(out_red)
							{
								int fd_out;
								if(over)
									fd_out=open(out_tok, O_CREAT | O_WRONLY | O_TRUNC, 0644);
								else
									fd_out=open(out_tok, O_CREAT | O_WRONLY | O_APPEND, 0644);
								dup2(fd_out, 1);
								out_red=0;
							}
							else if(strcmp(out_tok, ">")==0)
							{
								over=1;
								out_red=1;
							}
							else if(strcmp(out_tok, ">>")==0)
							{
								app=1;
								out_red=1;
							}
							else
							{
								strcat(temp4, out_tok);
								strcat(temp4, " ");
							}
							out_tok=strtok(NULL, " ");
						}	
					}
					strcpy(temp4, pip_tok);
					trim(pip_tok, temp4);
					char temp5[1024];
					strcpy(temp5, temp4);
					char* space[1024];
					char** spa_poi=space;
					int no_arg=0;
					char* spa_tok=strtok(temp5, " ");
					while(spa_tok!=NULL)
					{
						//strcpy(space[no_arg], spa_tok);
						*spa_poi++=spa_tok;
						no_arg++;
						spa_tok=strtok(NULL, " ");
					}
					*spa_poi=NULL;
				//	printf("%d\n", no_arg);
					//write(std_out, temp4, strlen(temp4));
					//printf("%s\n", temp4);
					char* tok=strtok(temp4, " ");
					if(strcmp(tok, "cd")==0)
					{
						tok=strtok(NULL, " ");
						if(strlen(tok)>0)
						{
							if(tok[0]=='~')
							{
								if(strlen(tok)>1)
								{
									char test[1024], test2[1024];
									for(int i=1; i<strlen(tok); i++)
									{
										test[i-1]=tok[i];
									}
									strcpy(test2, init_loc);
									strcat(test2, test);
									if(chdir(test2)<0)
										printf("No such directory\n");
									getcwd(curr_loc, 1024);
								}
								else
								{
									chdir(init_loc);
									getcwd(curr_loc, 1024);	
								}
							}
							else
							{
								if(chdir(tok)<0)
									printf("No such directory\n");
								getcwd(curr_loc, 1024);
								//write(std_out, curr_loc, strlen(curr_loc));
							}
						}
					}
					else if(strcmp(tok, "pwd")==0)
					{
						printf("%s\n", curr_loc);
					}
					else if(strcmp(tok, "echo")==0)
					{
						tok=strtok(NULL, " ");
						while(tok!=NULL)
						{
							printf("%s ", tok);
							tok=strtok(NULL, " ");
						}
						printf("\n");
					}
					else if(strcmp(tok, "ls")==0)
					{
						//printf("%s\n", temp);
						//dup2(1, std_out);
						ls(tok, temp);
						//dup2(filedes[1], 1);
					}
					else if(strcmp(tok, "pinfo")==0)
					{
						pinfo(tok);
					}
					else if(strcmp(tok, "setenv")==0)
					{
						if(no_arg>3 || no_arg==1)
						{
							printf("Incorrect no of arguments\n");
							break;
						}
						else if(no_arg==2)
						{
							if(setenv(space[1], "\0", 2)<0)
							{
								printf("Wrong keywords\n");
							}
						}
						else if(setenv(space[1], space[2], 2)<0)
						{
							printf("Wrong keywords\n");
						}
					}
					else if(strcmp(tok, "unsetenv")==0)
					{
						if(no_arg!=2)
							printf("Incorrect no of arguments\n");
						else if(unsetenv(space[1])<0)
							printf("Wrong keywords\n");
					}
					else if(strcmp(tok, "jobs")==0)
					{
						if(no_arg!=1)
							printf("Wrong Syntax\n");
						else
						{
							char jobs_loc[1024];
							for(int j=0; j<no_jobs; j++)
							{
								sprintf(jobs_loc, "/proc/%d/status", jobs[j].job_pid);
								FILE *fp;
								fp=fopen(jobs_loc, "r");
								if(fp<=0)
								{
									printf("Error: This job either terminated or does not exist\n");
									continue;
								}
								else
								{
									char file_cont[1024];
									while(file_cont!=NULL)
									{
										fgets(file_cont, 1024, fp);
										char* tok1=strtok(file_cont, ":");
										if(strcmp(tok1, "State")==0)
										{
											tok1 =strtok(NULL, " ");
											for(int ik=0; ik<strlen(tok1); ik++)
											{
												if(tok1[ik]!=' ')
												{
												//	printf("%c\n", tok1[ik]);
													if(tok1[ik]=='T')
														strcpy(jobs[j].state, "Stopped");
													else
														strcpy(jobs[j].state, "Running");
												}
											}
											break;
										}
									}
									printf("[%d] %s %s [%d]\n", j+1, jobs[j].state, jobs[j].job_name, jobs[j].job_pid);
								}
							}
						}
					}
					else if(strcmp(tok, "kjob")==0)
					{
						if(no_arg!=3)
							printf("Wrong no of arguments\n");
						else
						{
							int x=atoi(space[1]), y=atoi(space[2]);
							if(x>no_jobs)
								printf("No such job exists\n");
							else
								kill(jobs[x-1].job_pid, y);
						}
					}
					else if(strcmp(tok, "bg")==0)
					{
						if(no_arg!=2)
							printf("Wrong no of arguments\n");
						else
						{
							int l=atoi(space[1]);
							if(l>no_jobs)
								printf("No such job exists\n");
							else
								kill(jobs[l-1].job_pid, SIGCONT);
						}
					}
					else if(strcmp(tok, "fg")==0)
					{
						if(no_arg!=2)
							printf("Wrong no of arguments\n");
						else
						{
							int all=atoi(space[1]);
							if(all>no_jobs)
								printf("No such job exists\n");
							else
							{
								//printf("Yo\n");
								int pid_fg=jobs[all-1].job_pid;
								//printf("%d\n", pid_fg);
								char name[1024];
								strcpy(name, jobs[all-1].job_name);
								for(int kk=all-1; kk<no_jobs-1; kk++)
								{
									jobs[kk].job_pid=jobs[kk+1].job_pid;
									strcpy(jobs[kk].job_name, jobs[kk+1].job_name);
								}
								signal(SIGTTOU, SIG_IGN);
								signal(SIGTTIN, SIG_IGN);
								int child=pid_fg;
								tcsetpgrp(STDIN_FILENO, child);
								kill(pid_fg, SIGCONT);
								int state_fg;
								waitpid(pid_fg, &state_fg, WUNTRACED);
								tcsetpgrp(STDIN_FILENO, getpgid(pid_fg));
								signal(SIGTTIN, SIG_DFL); signal(SIGTTOU, SIG_DFL);
								//printf("%s\n", name);
								if(WIFSTOPPED(state_fg))
								{
									//printf("Yo\n");
									printf("%s with PID %d suspended\n", name, child);
									strcpy(jobs[no_jobs].job_name, name);
							        jobs[no_jobs++].job_pid = child;
								}
							}
						}
					}
					else if(strcmp(tok, "overkill")==0)
					{
						if(no_arg==1)
						{
							for(int iii=0; iii<no_jobs; iii++)
								kill(jobs[iii].job_pid, 9);
						}
						no_jobs=0;
					}
					else
					{
						int i=0;
						bool and_fl=0, inp_fl=0;
						int out_fl=0;
						char *comm[1024];
						char **point= comm;
						while(tok!=NULL)
						{
							if(strcmp(tok, "&")==0)
								and_fl=1;
							else
								*point++ =tok;
							tok=strtok(NULL, " ");
						}
						*point=NULL;
						signal(SIGCHLD, fun);
						int fork_val=fork();
						// if(shell_fl)
						// 	exit(0);
						if(and_fl==0)
						{
							if(fork_val==0)
							{
								//printf("Yo\n");
								//setpgid(0, 0);
								execvp(comm[0], comm);
								printf("Wrong command\n");
								exit(0);
							}
							else
							{
								int k;
								waitpid(fork_val, &k, WUNTRACED);
								//exit(0);
									// if(WIFSTOPPED(k))
									// {
									// 	jobs[no_jobs].job_pid=fork_val;
									// 	strcpy(jobs[no_jobs++].job_name, space[0]);
									// }
							}
						}
						else
						{
							if(fork_val==0)
							{
								setpgid(0, 0);
								execvp(comm[0], comm);
								printf("Wrong command\n");
								exit(0);
							}
							else
							{
								jobs[no_jobs].job_pid=fork_val;
								strcpy(jobs[no_jobs].job_name, temp);
								no_jobs++;
								printf("%d\n", fork_val);			
								pip_tok=strtok(NULL, "|");
								ii++;
								//continue;
							}
						}
					//	exit(0);
					}
					//exit(0);
				}
				else
				{
					//if(shell_fl)
					//	return 0;
					wait(NULL);
					close(filedes[1]);
					fd_temp=filedes[0];
					exit(0);
				}
				pip_tok=strtok(NULL, "|");
				ii++;
			}
			dup2(std_inp, 0);
			dup2(std_out, 1);
		}
		if(next_fl)
			continue;
	}
	//printf("Yo\n");
	//if(shell_fl)
	//	return 0;
	return 0;
}
