#include "func.h"


int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	int len;
	char buf[1000]={0};
	int fd;
	recv_n(sfd,&len,sizeof(len));
	recv_n(sfd,buf,len);
	fd=open(buf,O_CREAT|O_WRONLY,0666);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	while(1)
	{
		recv_n(sfd,&len,sizeof(len));//接火车头
		if(len>0)
		{
			memset(buf,0,sizeof(buf));
			recv_n(sfd,buf,len);
			write(fd,buf,len);
		}else{
			break;
		}
	}
	close(fd);
	close(sfd);
}
