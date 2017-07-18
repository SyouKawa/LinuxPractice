#include"func.h"

int main(int argc,char* argv[])
{
	if(argc!=4)
	{
		printf("error args\n");
		return -1;
	}
	int num=atoi(argv[3]);//创建的子进程数
    pcdata p=(pcdata)calloc(num,sizeof(cdata));
	make_child(p,num);
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[2]));
	ser.sin_addr.s_addr = inet_addr(argv[1]);
	int ret;
	ret = bind(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	listen(sfd,num);
	
	int epfd;
	epfd = epoll_create(1);
	if(-1==epfd)
	{
		perror("epoll_create");
		return -1;
	}
	struct epoll_event event,*evs;
	evs=(struct epoll_event*)calloc(num+1,sizeof(event));
	bzero(&event,sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = sfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);//监控sfd
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	int i;
	for(i=0;i<num;i++)
	{
		event.events = EPOLLIN;
		event.data.fd = p[i].fdw;
		epoll_ctl(epfd,EPOLL_CTL_ADD,p[i].fdw,&event);
	}
	int j,new_fd;
	char buf1[1000]={'\0'};
	char buf2[1000]={'\0'};
	short flag;
	struct sockaddr_in cli;
	bzero(&cli,sizeof(cli));
	int len = sizeof(struct sockaddr);
	while(1)
	{
		bzero(evs,(num+1)*sizeof(event));
		ret = epoll_wait(epfd,evs,num+1,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(evs[i].data.fd==sfd)
				{
					bzero(buf1,sizeof(buf1));
					bzero(buf2,sizeof(buf2));
					new_fd = accept(sfd,(struct sockaddr*)&cli,&len);
					printf("ip = %s,port = %d connect success\n",
							inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
					sprintf(buf2,"ip = %s,port = %d connect success\n",
							inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));

					for(j=0;j<num;j++)
					{
						if(0==p[j].busy)//找到非忙碌的子进程
						{
							send_fd(p[j].fdw,new_fd);
							p[j].busy = 1;
							break;
						}
					}
					close(new_fd);
				}	
				for(j=0;j<num;j++)
				{
					if(evs[i].data.fd == p[j].fdw)//找到刚刚完成任务的子进程
					{
						read(p[j].fdw,&flag,sizeof(flag));//将管道的数据读出来
						p[j].busy = 0;
					}
				}
			}
		}
	}
	close(sfd);
	close(epfd);
	return 0;
}

