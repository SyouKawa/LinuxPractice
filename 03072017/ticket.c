#include"header.h"

int count=20;
pthread_mutex_t mutex;

void *SaleWindow(void *p){
		int num=(int)p;

		while(1){
				pthread_mutex_lock(&mutex);
				if(count>0){

						if(1==num) printf("Window-%d- :Left %d tickets.\n",num,count);
						if(2==num) printf("Window-%d- :Left %d tickets.\n",num,count);
						count--;
						printf("Now ,Left tickets : %d.\n",count);
				}else{
						pthread_mutex_unlock(&mutex);
						pthread_exit(NULL);
				}
				pthread_mutex_unlock(&mutex);
				sleep(2);
		}
}

int main(){

		pthread_t pthid1=0;
		pthread_t pthid2=0;

		int ret=pthread_mutex_init(&mutex,NULL);
	
		pthread_create(&pthid2,NULL,SaleWindow,(void *)2);
		pthread_create(&pthid1,NULL,SaleWindow,(void *)1);
	
		pthread_join(pthid1,NULL);
		pthread_join(pthid2,NULL);

		pthread_mutex_destroy(&mutex);
	
		return 0;
}
