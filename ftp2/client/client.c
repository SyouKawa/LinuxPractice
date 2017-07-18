#include"func.h"

int curtime(char *buf){
    time_t cur;
    time(&cur);
    char *pcur=ctime(&cur);
    *(pcur+24)=0;
    sprintf(buf,"%s--time:%s\n",buf,pcur);
    return 0;
}

void get_salt(char *salt,char *passwd)
{
	int i,j;

	for(i=0,j=0;passwd[i] && j != 3;++i)
	{
		if(passwd[i] == '$') ++j;
	}	
	strncpy(salt,passwd,i-1);                                           
}

void divide(char* buf,char* buf1,char* buf2)
{
	char* m;
	char* n;
	n=buf;
	int i=0,j=0;
	int a,b;
	int flag=0;
	while(*n!='\n')
	{
		if(' '==*n)
		{
			flag=1;
			break;
		}
		n++;
	}
	m=buf;
	if(1==flag)
	{
		while(*m!='\n')
		{
			if(' '==*m)
			{
				a=i;
				break;
			}
			i++;
			m++;
		}
		memcpy(buf1,buf,a);
		memcpy(buf2,buf+a+1,strlen(buf)-a-2);
	}else{
		char p[4]="null";
		memcpy(buf1,buf,strlen(buf)-1);
		memcpy(buf2,p,3);
	}
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}

	struct spwd *sp;
    char username[15]={0};
    char *passwd={0};
    char salt[512]={0};
    char log_info[512]={0};

label:system("clear");
    printf("Please input your user-name:\n");
    read(0,username,sizeof(username));
    int over=strlen(username)-1;
    username[over]='\0';
    passwd=getpass("\npassword:");
    if((sp=getspnam(username))==NULL){
            perror("CANT GET USER OR YOUR PASSWORD\n");
            return -2;
    }

    get_salt(salt,sp->sp_pwdp);
    if(strcmp(sp->sp_pwdp,crypt(passwd,salt))==0){
            printf("Success!\n");
	    sprintf(log_info,"%s%s%s",log_info,"Success:log_user->",username);
	    curtime(log_info);
    }
    else{
            printf("Sorry,Check error!\n");
            goto label;
    }

	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in cli;
	bzero(&cli,sizeof(cli));
	cli.sin_family = AF_INET;
	cli.sin_port = htons(atoi(argv[2]));
	cli.sin_addr.s_addr = inet_addr(argv[1]);
	int ret;
	ret = connect(sfd,(struct sockaddr*)&cli,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("connect");
		return -1;
	}
	sprintf(log_info,"%s%s",log_info,"--connect");
	curtime(log_info);
	int log_fd=open(LOGFILENAME,O_RDWR|O_APPEND|O_SYNC);
	
	write(log_fd,log_info,strlen(log_info)+1);
	close(log_fd); 

	int epfd;
	epfd = epoll_create(1);
	if(-1 == epfd)
	{
		perror("epoll_create");
		return -1;
	}
	struct epoll_event event,evs[2];
	bzero(&event,sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);

	bzero(&event,sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = STDIN_FILENO;
	epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);

	int i;
	char buf[1000]={0};
	train t;
	char buf1[1000]={0};
	char buf2[1000]={0};


	while(1)
	{
		bzero(evs,sizeof(evs));
		int num;
		num = epoll_wait(epfd,evs,2,-1);
		if(num>0)
		{
			for(i=0;i<num;i++)
			{
				if(evs[i].data.fd==STDIN_FILENO)
				{
					bzero(buf,sizeof(buf));
					puts("............\n");
					ret = read(STDIN_FILENO,buf,sizeof(buf));
					if(ret>0)
					{
						bzero(buf1,sizeof(buf1));
						bzero(buf2,sizeof(buf2));
						bzero(&t,sizeof(t));
					
						divide(buf,buf1,buf2);
					
						t.len = strlen(buf1);
						strcpy(t.buf,buf1);
						send_n(sfd,(char*)&t,4+t.len);//发送命令给服务器
					
						bzero(&t,sizeof(t));
						t.len = strlen(buf2);
						strcpy(t.buf,buf2);
						send_n(sfd,(char*)&t,4+t.len);//发送命令后的内容给服务器
						
						if(0==strcmp(buf1,"puts"))
						{
							handle_puts(buf2,sfd);
						}else if(0==strcmp(buf1,"gets")){
						//	send_local_size(sfd,buf2);
							handle_gets(buf2,sfd);
						}
					}
				}
				if(evs[i].data.fd==sfd)//接收服务器端相应的命令操作
				{
					bzero(&t,sizeof(t));
					recv_n(sfd,(char*)&t.len,4);
					recv_n(sfd,t.buf,t.len);
					puts(t.buf);
				}
			}
		}
	}
	close(sfd);
	return 0;
}
