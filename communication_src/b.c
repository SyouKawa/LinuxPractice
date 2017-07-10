#include"header.h"

void handle(int sig)
{ 
    int shmid=shmget(108,MIN_MEMO,IPC_CREAT|0600);
    struct proc_info *p=(struct proc_info *)shmat(shmid,NULL,0);
    shmctl(p->shmid,IPC_RMID,NULL);
    kill(p->pid_A,SIGKILL);
	kill(p->pid_B,SIGKILL);
 
    exit(0);  
}

int main(){
	int fd_readA=open("writeB_readA.pipe",O_RDONLY);
	int fd_writeA=open("readB_writeA.pipe",O_WRONLY);
	
	int shmid=shmget(108,MIN_MEMO,IPC_CREAT|0600);
	struct proc_info *p=(struct proc_info *)shmat(shmid,NULL,0);
	read(fd_readA,&(p->pid_A),sizeof(p->pid_A));
	write(fd_writeA,&(p->pid_B),sizeof(p->pid_B));
	memset(p->str,0,MAX_STR);
	
	int ret;
	fd_set rdset;
	p->flag='\0';
	while(1){
		signal(SIGINT,handle);

		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fd_readA,&rdset);
		int fd_num=-1;
		fd_num=(fd_readA<fd_writeA)?fd_readA+1:fd_writeA+1;
		ret=select(fd_num,&rdset,NULL,NULL,0);
		if(ret>0){
			if(FD_ISSET(0,&rdset)){
				memset(p->str,0,MAX_STR);
				ret=read(0,p->str,MAX_STR);
				p->flag='1';
				system("clear");
				if(ret>0){
					write(fd_writeA,p->str,MAX_STR);
				}
				else{
					printf("BYEBYE~\n");
					break;
				}
			}
			if(FD_ISSET(fd_readA,&rdset)){
				memset(p->str,0,MAX_STR);
				ret=read(fd_readA,p->str,MAX_STR);
				p->flag='1';
				if(ret<=0){
					printf("BYEBYE~\n");
					break;
				}
				else{
					//printf("A:%s\n",p->str);
				}
			}
		}
	}
	close(fd_readA);
	close(fd_writeA);

	return 0;
}
