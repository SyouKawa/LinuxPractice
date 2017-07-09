#include"socket_header.h"

int main(int argc,char* argv[]){
	if(argc!=3){
		printf("Error Argument !\n");
		return -1;
	}

	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd){
		perror("socket");
		return -1;
	}
	
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=bind(sfd,(struct sockaddr *)&ser,sizeof(ser));
	if(-1==ret){
		perror("bind");
		return -1;
	}

	listen(sfd,10);
	
	int new_fd;
	struct sockaddr_in cli;
	int len=sizeof(cli);
	new_fd=accept(sfd,(struct sockaddr *)&cli,&len);
	if(-1==new_fd){
		perror("accept");
		return -1;
	}
	printf("client addr=%s,client port=%d\n",\
			inet_ntoa(cli.sin_addr),\
			ntohs(cli.sin_port));
	
	char buf[128]={0};
	ret=recv(new_fd,buf,sizeof(buf),0);
	printf("ret=%d,buf=%s\n",ret,buf);
	ret=send(new_fd,"I am server.\n",13,0);
	printf("send ret=%d\n",ret);

	return 0;
}
