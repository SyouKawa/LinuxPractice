#ifndef __SOCKET_HEADER_H_
#define __SOCKET_HEADER_H_

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#include<sys/epoll.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
//#include"send_recv.h"

void send_fd(int fdw,int fd);
void recv_fd(int fdr,int *fd);
void send_data(int fd);
struct child_info{
	pid_t pid;
	int tfds;
	short busy;
};

void make_child(struct child_info *pool,int count);

struct train{
	int len;
	char buf[1000];
};

#endif
