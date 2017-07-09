#include"socket_header.h"
#include<signal.h>
int main(int argc,char **argv){
	if(argc!=3){
		printf("Error Argument!\n");
		return -1;
	}

	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
		if(-1==sfd){
			perror("socket");
			return -1;
		}

	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);

	int ret=connect(sfd,(struct sockaddr *)&ser,sizeof(ser));
	if(-1==ret){
		perror("connect");
		return -1;
	}
	
	char buf[128]={0};
	fd_set rdset;
	while(1){
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(sfd,&rdset);

		ret=select(sfd+1,&rdset,NULL,NULL,NULL);
		if(ret>0){
			if(FD_ISSET(sfd,&rdset)){
				bzero(buf,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);
				if(ret>0){
					printf("%s\n",buf);
				}
				else if(0==ret){
					printf("byebye~\n");
					break;
				}
			}
			if(FD_ISSET(0,&rdset)){
				if(ret>0){
					bzero(buf,sizeof(buf));
					ret=read(0,buf,sizeof(buf));
					if(ret>0) send(sfd,buf,strlen(buf)-1,0);
				}
				else{
					printf("byebye~\n");
					break;
				}
			}
		}
	}
//	close(sfd);
	return 0;
}
