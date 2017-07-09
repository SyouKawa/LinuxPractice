#include"socket_header.h"
#include<time.h>
#include"send_file.c"
int main(int argc,char **argv){
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
	ser.sin_addr.s_addr=inet_addr(argv[1]);
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
	recv_n(sfd,&len,sizeof(len));
	recv_n(sfd,buf,len);
	//printf("%d\n",len);
	printf("==The copy file name is:%s==\n",buf);
	printf("Loading...\n");
	fd=open(buf,O_CREAT|O_WRONLY,0666);
	if(-1==fd){
		perror("open");
		return -1;
	}
	
//	recv(sfd,&len,sizeof(len),0);
	//printf("len=%d\n",len);

		//2.2 copy the content to the same name file.
	while(1){
			//2.2.1 get the length (THIS TIME)
		recv_n(sfd,&len,sizeof(len));
		if(len>0){
			//2.2.2 copy (without judge)
			bzero(buf,sizeof(buf));
			recv_n(sfd,buf,len);
			write(fd,buf,len);
		}
		else{
			printf("download success.\n");
			break;
		}
	}
	close(fd);
	close(sfd);
}
