#include"header.h"

void *Thd_Entry(void *p){
	pthread_mutex_t *lock=(pthread_mutex_t *)p;

	pthread_mutex_lock(lock);
	pthread_mutex_lock(lock);

	printf("hello\n");
	sleep(1);
	pthread_exit(NULL);
}


int main(){
	pthread_mutex_t lock;
	pthread_mutex_init(&lock,NULL);

	pthread_t pthid=0;
	pthread_create(&pthid,NULL,Thd_Entry,(void *)&lock);

	pthread_join(pthid,NULL);
	pthread_mutex_destroy(&lock);
}
