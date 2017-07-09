#ifndef __HEADER_H_
#define __HEADER_H_


#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

#include<pthread.h>

typedef struct{
	pthread_t pthid1,pthid2;

	int fds[2];
}tran,ptran;

#endif
