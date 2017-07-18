#include"func.h"

long int get_local_size(char *buf){
    int fd=open(buf,O_RDONLY);
    if(-1==fd) return 0;
    else{
	struct stat local_info;
	stat(buf,&local_info);
	printf("%ld\n",local_info.st_size);
	close(fd);
	long int local_size=local_info.st_size;
	printf("%ld\n",local_size);
	sleep(1);
	return local_size;
    }
}

void handle_puts(char* buf,int sfd)
{
	train t;
	int fd;
	fd = open(buf,O_RDONLY);
	while(bzero(&t,sizeof(t)),(t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	{
		send_n(sfd,(char*)&t,4+t.len);
	}
	t.len = 0;
	send_n(sfd,(char*)&t.len,4);
	close(fd);
}

void handle_gets(char* buf,int sfd)
{
	long int local_size=get_local_size(buf);
	printf("%ld\n",local_size);
	sleep(1);
	send_n(sfd,(char *)&local_size,sizeof(long int));

	train t;
	int fd= open(buf,O_RDWR|O_APPEND);
	if(-1==fd){
		fd=open(buf,O_RDWR|O_CREAT,0666);
	}

	long int whole=0;
	int ret=recv_n(sfd,(char *)&whole,sizeof(long int));
	printf("%ld\n",(long int)whole);
	if(-1==ret){
		printf("ERROR RECV!\n");
		return;
	}

	long int percent=0;

	while(1)
	{
		recv_n(sfd,(char*)&t.len,4);	
		bzero(t.buf,sizeof(t.buf));

		percent+=t.len;

		if(t.len>0)
		{
			recv_n(sfd,t.buf,t.len);
			write(fd,t.buf,t.len);
		}else{
			break;
		}
		double print=(double)percent*100/whole;
		char pic[13]={0};
		int num=print/10;
		for(int i=0;i<num;i++){
		    sprintf(pic,"%s%s",pic,">");
		}
		if(percent!=whole) printf("Now recv||%s%5.2f%s||\r",pic,print,"%");
		else printf("Now recv||%s%5.2f%s\n",pic,print,"%");
		fflush(stdout);
	}
	close(fd);
}
