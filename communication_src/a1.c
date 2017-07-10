#include"header.h"
  
int main()
{
        int shmid=shmget(221,MIN_MEMO,0600|IPC_CREAT);
        struct proc_info *p=(struct proc_info *)shmat(shmid,NULL,0);
	
		p->shmid=shmid;
		p->pid_A=getpid();
		p->flag='\0';
		
		if(p->flag=='\0'){
        	while(1){
        		if(p->flag=='1'){
        		printf("from B:%s\n",p->str);
				p->flag='\0' ; 
        		}
        	}
		}
        exit(0);  
}
