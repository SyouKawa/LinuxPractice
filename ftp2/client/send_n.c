#include"func.h"

int recv_n(int fd,char* buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret = recv(fd,buf+total,len-total,0);
		if(ret==-1){
		    printf("recv_n: recv error!\n");
		    return -1;
		}
		total = total + ret;
	}
	return total;
}

int send_n(int fd,char* buf,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret= send(fd,buf+total,len-total,0);
		if(-1==ret){
			printf("send_n:send error!\n");
			return -1;
		}
		total = total + ret;
	}

	return total;
}

