#include"socket_header.h"

int main(){
	char ip[]="192.168.0.100";
	
	struct in_addr myaddr;

	int iRet=inet_aton(ip,&myaddr);
	if(0==iRet){
		printf("fail to addr exchange to net.");
		return -1;
	}
	printf("inet_aton:%x\n",myaddr.s_addr);
	puts("is same:");
	printf("inet_addr:%x\n",inet_addr(ip));

	iRet=inet_pton(AF_INET,ip,&myaddr);
	if(0==iRet){
		printf("fail to IP exchange to net.");
		return -1;
	}
//	printf("%d\n",iRet);
	printf("inet_pton:%x\n",myaddr.s_addr);

	//n-->ip func
	myaddr.s_addr=0xac100ac4;
	printf("inet_ntoa:%s\n",inet_ntoa(myaddr));
	
	inet_ntop(AF_INET,&myaddr,ip,16);
//	printf("%d\n",iRet);
	if(0==iRet){
		printf("fail to net exchange to IP.");
		return -1;
	}
	printf("inet_ntop:%s\n",ip);

	return 0;
}
