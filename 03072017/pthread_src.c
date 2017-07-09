#include"header.h"

void *Thd_Entry(void *pArg){
	for(int i=0;i<3;i++){
//		sleep(3);
		printf("From Child thread,arg is:%d\n",(int)pArg);
		sleep(10);
	}
	pthread_exit(NULL);
}

int main(){
	pthread_t thdID;
	pthread_create(&thdID,NULL,Thd_Entry,(void*)123);
	
	for(int i=0;i<3;i++){
//		sleep(10);
		printf("From Main thread,and child thread is %x,%d\n",thdID,thdID);
		sleep(4);
	}
	while(1);
	return 0;
}
