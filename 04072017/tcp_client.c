#include"socket_header.h"

int main(int argc,char **argv){
	if(argc!=3){
		printf("Error Argument !\n");
		return -1;
	}

	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd){
		perror("socket");
		return -1;
	}

	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret=connect(sfd,(struct sockaddr *)&ser,sizeof(ser));
	if(-1==ret){
		perror("connect");
		return -1;
	}

	char buf[128]={0};
	ret=send(sfd,"I am client.\n",13,0);
	printf("send ret=%d\n",ret);
	ret=recv(sfd,buf,sizeof(buf),0);
	printf("client ret=%d,buf=%s\n",ret,buf);
	
	return 0;
}
