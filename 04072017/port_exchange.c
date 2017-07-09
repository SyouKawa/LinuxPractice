#include"socket_header.h"
#include<netdb.h>

int main(int argc,char **argv){
	if(argc!=2){
		printf("Error Argument!\n");
		return -1;
	}
	
	struct hostent *p;
	p=gethostbyname(argv[1]);
	if(NULL==p){
		perror("gethostbyname");
		return -1;
	}

	printf("host_name:%s\n",p->h_name);

	for(int i=0;p->h_aliases[i]!=NULL;i++) 
		printf("host_aliases=%s\n",*(p->h_aliases));

	char ip[16]={0};
	for(int i=0;p->h_addr_list[i]!=NULL;i++){
		bzero(ip,sizeof(ip));
		inet_ntop(p->h_addrtype,p->h_addr_list[i],ip,sizeof(ip));
		printf("%s\n",ip);
	}

	return 0;
}

