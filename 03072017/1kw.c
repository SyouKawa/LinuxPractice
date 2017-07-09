#include"header.h"
#define N 10000000

typedef struct{
	int s;
	pthread_mutex_t mutex;
}sdata;

void *add(void *p){
	sdata * temp=(sdata *)p;

	for(int i=0;i<N;i++){
		pthread_mutex_unlock(&(temp->mutex));
		(temp->s)++;
		pthread_mutex_unlock(&(temp->mutex));
	}

	pthread_exit(NULL);
}

int main(){
	pthread_t pthid;
	sdata s;
	s.s=0;
	pthread_mutex_init(&(s.mutex),NULL);
	pthread_create(&pthid,NULL,add,&s);
	
	for(int i=0;i<N;i++){
		pthread_mutex_lock(&(s.mutex));
		(s.s)++;
		pthread_mutex_unlock(&(s.mutex));
	}
	pthread_join(pthid,NULL);
	pthread_mutex_destroy(&(s.mutex));
	printf("%d\n",s.s);

	return 0;
}
