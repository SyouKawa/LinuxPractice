#include"header.h"
#include<string.h>

void *threadfunc(void *p){
	tran *pt=(tran *)p;
	printf("I am child thread_1\n");
	long i;//8-bytes for void *
	read(pt->fds[0],&i,sizeof(i));//waitting input from main thread.

	printf("I am child thread_2\n");

	// waitting thread2 create-->return from main thread.
	pthread_join(pt->pthid2,(void **)&i);
	printf("thread_2 return %ld\n",i);
	pthread_exit((void *)0);
}

void *threadfunc2(void *p){
	pthread_exit((void *)2);
}

int main(){
	tran t;
	memset(&t,0,sizeof(t));
	char buf[50]={0};

	pipe(t.fds);
	printf("you can see me.\n");
	int ret=pthread_create(&t.pthid1,NULL,threadfunc,&t);
	if(ret!=0){
		printf("pthread_create 1  failed ret=%d\n",ret);
		return -1;
	}
	ret=pthread_create(&t.pthid2,NULL,threadfunc2,&t);
	if(ret!=0){
		printf("pthread_create 2 failed ret=%d\n",ret);
		return -1;
	}

	//pipe write receive the return value of create_2 .
	write(t.fds[1],&ret,sizeof(ret));
	long i;
	ret=pthread_join(t.pthid1,(void**)&i);
	if(ret!=0){
		printf("pthread_join failed ret=%d\n",ret);
		return -1;
	}
	printf("I am main thread,and pthid_1 return %d.\n",ret);
	return 0;
}
