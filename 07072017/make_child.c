#include"socket_header.h"

void child_handle(int fdr){
	char flag=1;
	int new_fd;
	while(1){
		recv_fd(fdr,&new_fd);//close write(fds[1]), receive sfd aswrite by new_fd(save ctl_info from sfd).
		puts("Received the new_fd from main process.");
		send_data(new_fd);
		printf("I am child,send file success.\n");
		write(fdr,&flag,sizeof(flag));
	}
}

void make_child(struct child_info* pool,int count){
	int fds[2];
	pid_t pid;
	for(int i=0;i<count;i++){
		//Make pipes for every-process 
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(0==pid){
			close(fds[1]);
			child_handle(fds[0]);
		}
		else{
			close(fds[0]);
			pool[i].pid=pid;
			pool[i].tfds=fds[1];
			pool[i].busy=0;
		}
	}
}
