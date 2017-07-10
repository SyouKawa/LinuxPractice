#ifndef __FACTORY_H__
#define __FACTORY_H__

#include"socket_header.h"
#include"work_que.h"

typedef void *(*pfunc)(void *);

typedef struct {
	que_t que;
	pthread_cond_t cond;
	int capability;
	int thread_num;	
	pfunc downfile;
	pthread_t *pth;
	short startflag;
}factory,*pfac;

typedef struct{
	int len;
	char buf[1000];
}train,*ptrain;

#define FILENAME "file"

#endif
