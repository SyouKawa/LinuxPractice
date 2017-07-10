#include"header.h"

int main()
{
        int shmid=shmget(108,4096,IPC_CREAT|0600);
        struct proc_info *p=(struct proc_info*)shmat(shmid,NULL,0);

		p->shmid=shmid;
        p->pid_B=getpid();
		p->flag='\0';

		if(p->flag=='\0'){
			while(1){
        		if(p->flag=='1'){  
             		printf("from A:%s\n",p->str);
             		p->flag='\0' ; 
        		}
			}
		}
       return 0;
}
