#include"header.h"

void handle(int sig)
{ 
	int shmid=shmget(221,MIN_MEMO,IPC_CREAT|0600);
	struct proc_info *p=(struct proc_info *)shmat(shmid,NULL,0);
	shmctl(p->shmid,IPC_RMID,NULL);
	kill(p->pid_A,SIGKILL);
	kill(p->pid_B,SIGKILL);

	exit(0);  
}

int main(){

	int fd_writeB=open("writeB_readA.pipe",O_WRONLY);
	int fd_readB=open("readB_writeA.pipe",O_RDONLY);

	int shmid=shmget(221,MIN_MEMO,IPC_CREAT|0600);
	struct proc_info *p=(struct proc_info *)shmat(shmid,NULL,0);
	write(fd_writeB,&(p->pid_A),sizeof(p->pid_A));
	read(fd_readB,&(p->pid_B),sizeof(p->pid_B));
	bzero(p->str,MAX_STR);
	
	int ret;
	fd_set rdset;
	p->flag='\0';
	while(1){
		signal(SIGINT,handle);
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fd_readB,&rdset);
		int fd_num=-1;
		fd_num=(fd_writeB<fd_readB)?fd_readB+1:fd_writeB+1;
		ret=select(fd_num,&rdset,NULL,NULL,0);
		p->flag='\0';
		if(ret>0){
			if(FD_ISSET(0,&rdset)){
				bzero(p->str,MAX_STR);
				ret=read(0,p->str,MAX_STR);
				p->flag='1';
				system("clear");
				if(ret>0){
					write(fd_writeB,p->str,MAX_STR);
				}
				else{
					printf("BYEBYE~\n");
					break;
				}
			}
			if(FD_ISSET(fd_readB,&rdset)){
				bzero(p->str,MAX_STR);
                ret=read(fd_readB,p->str,MAX_STR);
				p->flag='1';
				if(ret<=0){
					printf("BYEBYE~\n");
					break;
				}
				else{
			//	printf("B:%s\n",p->str);
				}
			}
		}
	}
	close(fd_readB);
	close(fd_writeB);

	return 0;
}
