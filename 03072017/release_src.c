#include<malloc.h>
#include<stdio.h>
#include<pthread.h>


void *Thd_Entry(void *pArg){
	int size=(int)pArg;

	char *p=(char *)malloc(size);
	int i=0;
	for(;i<size;i++) sleep(1);
	printf("The size is %d.\n",i);
	sleep(1);
	
	free(p);
	p=NULL;
	puts("p has been freed.\n");

	pthread_exit((void *)size);
}

int main(){
	pthread_t thdID;
	int pArg;
	printf("Please input the in_Arg for pthread: ");
	scanf("%d",&pArg);
	pthread_create(&thdID,NULL,Thd_Entry,(void *)pArg);

	if(pArg>4||pArg%2==0){
		printf("Can\'t finish normally later.\n");
	}
	else{
		printf("Can finish normally later.\n");
	}
	
	for(int i=0;i<4;i++){
		printf("%d\n",i);
		sleep(1);
	}
	
	if(pArg%2==0) pthread_cancel(thdID);

	int retvalue=0;
	pthread_join(thdID,(void**)&retvalue);
	printf("return value is%d\n",retvalue);

	return 0;
}
