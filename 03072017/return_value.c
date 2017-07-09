#include"header.h"

void *Thd_Entry(void *pArg){
	int iArg=(int)pArg;
	sleep(iArg);

	if(iArg<3){
		return (void*)(iArg*2);
	}
	else{
		pthread_exit((void *)iArg);
	}
}

int main(){
	pthread_t thdID;
	int iRet=0;
	pthread_create(&thdID,NULL,Thd_Entry,(void *)2);
	pthread_join(thdID,(void**)&iRet);
	printf("The first child thread ret is %d\n",iRet);

	pthread_create(&thdID,NULL,Thd_Entry,(void *)iRet);
	pthread_join(thdID,(void **)&iRet);
	printf("The second child thread ret is %d\n",iRet);

	return 0;
}
