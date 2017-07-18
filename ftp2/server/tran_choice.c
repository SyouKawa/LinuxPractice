#include"func.h"

int gets_choice(char* buf,int new_fd)
{
    int fd=open(buf,O_RDONLY);
    printf("%s\n",buf);
    if(fd==-1){// 1.file not exist;
	perror("open");
	return -1;
    }

    //if exist (open success):

    long int local_size=0;
    int ret=recv_n(new_fd,(char *)&local_size,sizeof(long int));
    if(-1==ret) {printf("recv error");return -1;}
    printf("recv_len:%d,local_size=%ld\n",ret,local_size);

    //2.1 get the size of file
    struct stat size_cond;
    stat(buf,&size_cond);
    long int size=size_cond.st_size;
    int atten=0;
    atten=send_n(new_fd,(char *)&size,sizeof(size));//client whole show;
    sleep(1);


    if(size<=104857600){//2.1.0 <100M normal-tran
	filegets(buf,new_fd);
	return 0;
    }
    else{//2.1.1 >100M mmap tran

        //add <100M continue later.
        long int left=size-(long int)local_size;
	off_t pa_offset=local_size & ~(sysconf(_SC_PAGE_SIZE)-1);

	printf("%s\'s size >100M,need mmap to tran...\n",buf);
	printf("size:%ld\n",size);
	printf("left:%ld\n",left);
	sleep(1);
	char *mmap_p=(char *)mmap(NULL,left,PROT_READ,MAP_SHARED,fd,pa_offset);
	if(-1==mmap_p||NULL==mmap_p) {
	    perror("mmap");
	    return -1;
	}
	train t;
	long int map_index=0,t_index=0;

	printf("download with mmap start...\n");
	printf("map_index start at:%ld\n",local_size);

	while(1){
	    bzero(&t,sizeof(t));
	    t_index=0;

	    while(map_index<left&&t_index<1000){
//		printf("loading...\n");
		memcpy(t.buf+t_index,mmap_p+map_index,1);
//		printf("1.map_index=%ld\n",map_index);
//		printf("1.t_index=%ld\n",t_index);
		t_index++;
		map_index++;
//		printf("2.map_index=%ld\n",map_index);
//		printf("2.t_index=%ld\n",t_index);
//		sleep(3);
	    }
	    t.len=t_index;
	    printf("Now send:%5.2f%s\r",(double)(map_index+local_size)*100/size,"%");
	    fflush(stdout);
	    if(send_n(new_fd,(char *)&t,4+t.len)==-1) return -1;
	    //printf("download successfully.\n");
	    if(t_index==0) break;
	}
	if(munmap(mmap_p,size)==-1){
	    perror("munmap");
	    return -1;
	}
    }
    printf("OK\n");
    close(fd);
    return 0;
}
