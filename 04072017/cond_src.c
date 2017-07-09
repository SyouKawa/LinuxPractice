#include"header.h"
#include<string.h>
#include<time.h>

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct timespec abstime;

}data;

void *Thd_Entry(void *p){
	data *d=(data *)p;

	printf("From child thread.\n");
	time_t t;
	time(&t);
	memset(&d->abstime,0,sizeof(d->abstime));
	d->abstime.tv_sec=t+5;
	pthread_mutex_lock(&(d->mutex));
	//pthread_cond_wait(&(d->cond),&(d->mutex));
	pthread_cond_timedwait(&(d->cond),&(d->mutex),&d->abstime);
	pthread_mutex_unlock(&d->mutex);
	
	printf("Hello.\n");
	pthread_exit(NULL);	
}

int main(){
	data t;
	memset(&t.abstime,0,sizeof(t.abstime));

	pthread_mutex_init(&t.mutex,NULL);
	pthread_cond_init(&t.cond,NULL);

	pthread_t pthid;
	pthread_create(&pthid, NULL,Thd_Entry,&t);
	//sleep(1);

	time_t times;
	time(&times);
	t.abstime.tv_sec=times+7;
//	int ret=pthread_cond_signal(&t.cond);
//	if(0!=ret){
//		printf("ERROR!\n");
//		return -1;
//`	}
	pthread_join(pthid,NULL);
	exit(0);
}
