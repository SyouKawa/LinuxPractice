#include"socket_header.h"
#include"factory.h"
int recv_n(int sfd,char *p,int len);
int send_n(int sfd,char *p,int len);

#define NAME "file"

void send_data(int new_fd){
	train t;
	bzero(&t,sizeof(t));
	strcpy(t.buf,NAME);
	t.len=strlen(t.buf);

	//1.send the name of file 
	int ret=send_n(new_fd,(char *)&t,4+t.len);
	if(-1==ret){
		perror("send");
		return ;
	}

	//2.open file and start to send the CONTENT
	int fd=open(NAME,O_RDONLY);
	if(-1==fd){
		perror("open");
		return;
	}
	while(bzero(&t,sizeof(t)),(t.len=read(fd,t.buf,sizeof(t.buf)))>0){
		send_n(new_fd,(char *)&t,4+t.len);
	}
	t.len=0;
	//3.send the over flag(empty len)
	send_n(new_fd,(char *)&t,4+t.len);
	close(new_fd);
}

int send_n(int sfd,char*p,int len){
	int total=0;
	int ret;
	while(total<len){
		ret=send(sfd,p+total,len-total,0);
		total+=ret;
	}
	return 0;
}

int recv_n(int sfd,char *p,int len){
	int total=0;
	int ret;
	while(total<ret){
		ret=recv(sfd,p+total,len-total,0);
		total+=ret;
	}
	return 0;
}
