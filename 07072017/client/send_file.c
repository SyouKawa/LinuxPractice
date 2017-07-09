#include"socket_header.h"
int recv_n(int sfd,char *p,int len);
int send_n(int sfd,char *p,int len);

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
