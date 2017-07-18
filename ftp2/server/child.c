#include "func.h"

int curtime(char *buf){
    time_t cur;
    time(&cur);
    char *pcur=ctime(&cur);
    *(pcur+24)=0;
    sprintf(buf,"%s--time:%s\n",buf,pcur);
    return 0;
}

void child_handle(int fdr)
{
	int new_fd;
	train t;
	bzero(&t,sizeof(t));
	short flag=1;
	signal(SIGPIPE,SIG_IGN);
	char buf1[512]={0};
	char buf2[512]={0};
	char log[512]={0};

	while(1)
	{
		recv_fd(fdr,&new_fd);
		while(1)
		{
			bzero(&t,sizeof(t));
			recv_n(new_fd,(char*)&t.len,4);
			recv_n(new_fd,t.buf,t.len);
			printf("command is %s\n",t.buf);
			bzero(buf1,sizeof(buf1));
			strcpy(buf1,t.buf);
			bzero(buf2,sizeof(buf2));
			recv_n(new_fd,(char*)&t.len,4);
			recv_n(new_fd,buf2,t.len);
			int fd=open(LOGFILENAME,O_RDWR|O_APPEND|O_SYNC);
			if(0==strcmp(buf1,"cd"))
			{
				cd(buf2,new_fd);
				bzero(log,sizeof(log));
				sprintf(log,"%s%s %s---",log,buf1,buf2);
				curtime(log);
				write(fd,log,strlen(log)+1);
				close(fd);
			}else if(0==strcmp(buf1,"ls"))
			{
				ls(buf2,new_fd);
				bzero(log,sizeof(log));
				if(buf2[0]=='\0') sprintf(log,"%s%s---",log,buf1);
				else sprintf(log,"%s%s %s---",log,buf1,buf2);
				curtime(log);
				puts(log);
				write(fd,log,strlen(log)+1);
				close(fd);
			}else if(0==strcmp(buf1,"puts"))
			{
				fileputs(buf2,new_fd);
				//send_file(buf2,new_fd);
				bzero(log,sizeof(log));
				sprintf(log,"%s%s %s---",log,buf1,buf2);
				curtime(log);
				puts(log);
				write(fd,log,strlen(log)+1);
				close(fd);
			}else if(0==strcmp(buf1,"gets"))
		    {
				gets_choice(buf2,new_fd);
				bzero(log,sizeof(log));
				sprintf(log,"%s%s %s---",log,buf1,buf2);
				curtime(log);
				puts(log);
				write(fd,log,strlen(log)+1);
				close(fd);
			}else if(0==strcmp(buf1,"pwd"))
			{
				pwd(new_fd);
				bzero(log,sizeof(log));
				if(buf2[0]=='\0') sprintf(log,"%s%s",log,buf1);
				else sprintf(log,"%s%s %s---",log,buf1,buf2);
				curtime(log);
				puts(log);
				write(fd,log,strlen(log)+1);
				close(fd);
			}else if(0==strcmp(buf1,"remove"))
			{
				fileremove(buf2,new_fd);
				bzero(log,sizeof(log));
				sprintf(log,"%s%s %s---",log,buf1,buf2);
				curtime(log);
				puts(log);
				write(fd,log,strlen(log)+1);
				close(fd);
			}else
			{
				printf("error command\n");
			}
		}
		write(fdr,&flag,sizeof(flag));
		
	}
}


void make_child(pcdata p,int n)
{
	int fds[2];
	pid_t pid;
	int i=0;
	int ret;
	for(i=0;i<n;i++)
	{
		ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		if(-1==ret)
		{
			perror("socket");
			return ;
		}
		pid=fork();
		if(0==pid)
		{
			close(fds[1]);
			child_handle(fds[0]);
		}
		p[i].pid=pid;
		p[i].fdw=fds[1];
		p[i].busy=0;
		close(fds[0]);
	}
}
