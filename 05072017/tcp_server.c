#include"socket_header.h"
#include<sys/select.h>

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
	struct sockaddr_in cli;
	int len=sizeof(cli);
	int new_fd=-1;// need exist before if()judge;
	char buf[128]={0};
	fd_set rdset;
	fd_set tempset;
	FD_ZERO(&tempset);
	FD_SET(0,&tempset);
	FD_SET(sfd,&tempset);

	while(1){
		FD_ZERO(&rdset);
		memcpy(&rdset,&tempset,sizeof(fd_set));
		ret=select(11,&rdset,NULL,NULL,NULL);
		if(ret>0){
		
			if(FD_ISSET(sfd,&rdset)){
				new_fd=accept(sfd,(struct sockaddr *)&cli,&len);
				if(-1==new_fd){
					perror("accept");
					return -1;
				}
				FD_SET(new_fd,&tempset);
				printf("client-IP:%s,port:%d\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
			}
			if(FD_ISSET(new_fd,&rdset)){
				memset(buf,0,sizeof(buf));
				ret=recv(new_fd,buf,sizeof(buf),0);
				if(ret>0){
					printf("%s\n",buf);
				}
				else if(0==ret){
					printf("byebye~\n");
					close(new_fd);
					FD_CLR(new_fd,&rdset);
//					break;	
				}
			}
			if(FD_ISSET(0,&rdset)){
				bzero(buf,sizeof(buf));
				ret=read(0,buf,sizeof(buf));
				if(ret>0){
					send(new_fd,buf,strlen(buf)-1,0);
					if(ret<=0){
						printf("send ret=%d\n",ret);
						perror("send");
					}
				}
				else{
					printf("byebye~\n");
					break;
				}
			}
		}
	}
	close(new_fd);
	close(sfd);

	return 0;
}
