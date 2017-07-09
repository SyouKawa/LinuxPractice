#include"socket_header.h"

int main(){
	if(argc!=3)	{
		printf("Error Argument!\n");
		return -1;
	}
	//1.the server --need to be link 
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd){
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_addr=inet_aton(argv[1]);
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_family=AF_INET;
	int connect_ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==connect_ret){
		perror("connect");
		return -1;
	}

	//2.recv with struct train.
	int len;
	char buf[1000]={0};
	int fd;
		//2.1 get the name of file and create a same file
	recv(sfd,&len,sizeof(len),0);
	recv(sfd,buf,len,0);
	fd=open(buf,O_CREAT|O_WRONLY,0666);
	if(-1==fd){
		perror("open");
		return -1;
	}
		//2.2 copy the content to the same name file.
	while(1){
			//2.2.1 get the length (THIS TIME)
		recv(sfd,&len,sizeof(len),0);
		if(len>0){
			//2.2.2 copy (without judge)
			bzero(buf,sizeof(buf));
			recv(sfd,buf,len,0);
			write(fd,buf,len);
		}
		else{
			break;
		}
	}
	close(fd);
	close(sfd);
}
