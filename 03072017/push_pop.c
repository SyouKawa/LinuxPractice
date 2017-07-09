#include"header.h"

void CleanFunc(void *pArg){
	printf("Cleaned %d.\n",(int)pArg);
}

void *Thd_Entry(void *pArg){
	pthread_cleanup_push(CleanFunc,(void *)1);
	pthread_cleanup_push(CleanFunc,(void *)2);

	sleep(2);
	
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
}

int main(){
	pthread_t thdID;
	pthread_create(&thdID,NULL,Thd_Entry,NULL);
//`	pthread_create(&thdID,NULL,Thd_Entry,NULL);

	sleep(5);
	
	return 0;
}
