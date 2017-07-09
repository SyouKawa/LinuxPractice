#include"header.h"
#include<string.h>

void *Thd_Entry(void *p){
	char buf[10];
	memcpy(buf,p,10);
	printf("From Child thread:%s\n",buf);

	pthread_exit(NULL);
}

int main(){
	char buf[10]="Hello";
	pthread_t thdID;
	pthread_create(&thdID,NULL,Thd_Entry,buf);
	sleep(1);
	return 0;
}
