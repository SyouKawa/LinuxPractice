#ifndef __HEADER_H_
#define __HEADER_H_

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/time.h>
#define MIN_MEMO 4096
#define MAX_STR 1000

struct proc_info{
	int shmid;
	pid_t pid_A;
	pid_t pid_B;	
	char flag;
	char str[1000];
};

#endif
