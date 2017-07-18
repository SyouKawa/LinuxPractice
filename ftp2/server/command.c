#include"func.h"

#define OUT

void mode(OUT char out[],int bits){
	int arr[16]={0};
	char mode[10]={0};
	int i=0;
	while(bits>=1){
		arr[i++]=bits%2;
		bits/=2;
	}
	if(arr[15])mode[0]='-';
	else mode[0]='d';
	int j=0;
	for(j=0;j<9;j++){
		int ret;
		if(arr[j]){
			ret = (j+1)%3;
			if(ret==1)mode[9-j]='x';
			if(ret==2)mode[9-j]='w';
			if(ret==0)mode[9-j]='r';
		}
		else mode[9-j]='-';
	}
	memcpy(out,mode,10);
}

void gettime(char* p_time,char T[])
{
	char *m;
	int i=0,j=0;
	m=p_time;
	int a,b;
	char t[25]={'\0'};
	while(*m!='\0')
	{
		if(' '==*m)
		{
			a=i;
			break;
		}
		i++;
		m++;
	}
	while(*m!='\0')
	{
		if(':'==*m)
		{
			b=j+a;
		}
		j++;
		m++;
	}
	memcpy(T,p_time+a,b-a);
}

void cd(char* buf,int new_fd)
{
	train t;
	bzero(&t,sizeof(t));
	chdir(buf);
	char buf1[100]={0};
	getcwd(buf1,sizeof(buf1));
	struct stat buf2;
	DIR *dir;
	dir = opendir(buf1);
	if(NULL==dir){
		perror("opendir");
		return;
	}
	struct dirent *p;
	while((p=readdir(dir))!=NULL){
		if(strcmp(p->d_name,".")&&strcmp(p->d_name,"..")){
		}
		else{
			memset(&buf2,0,sizeof(buf2));
			stat(p->d_name,&buf2);
			char out[11]={0};
			char T[15]={'\0'};
			mode(out,buf2.st_mode);
			char *TIME=ctime(&buf2.st_mtime);
			gettime(TIME,T);
			bzero(&t,sizeof(t));
			sprintf(t.buf,"%10s %-14s %-10s",out,T,p->d_name);
			t.len = strlen(t.buf);
			send_n(new_fd,(char*)&t,4+t.len);
		}
	}
}

void ls(char* buf,int new_fd)
{
	train t;	
	char buf1[200]={'\0'};
	char out[11]={0};
	chdir(buf);
	char buf2[200]={'\0'};
	getcwd(buf2,sizeof(buf2));
	struct stat buf3;
	DIR *dir;
	dir = opendir(buf2);
	struct dirent *p;
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
		{
		}else{
			memset(&buf3,0,sizeof(buf3));
			stat(p->d_name,&buf3);
			char T[15]={'\0'};
			mode(out,buf3.st_mode);
			char *TIME=ctime(&buf3.st_mtime);
			gettime(TIME,T);
			sprintf(t.buf,"%10s  %-3d %3s %3s %ld %-14s %-10s",out,buf3.st_nlink,
					getpwuid(buf3.st_uid)->pw_name,getgrgid(buf3.st_gid)->gr_name,
					buf3.st_size,T,p->d_name);
			t.len = strlen(t.buf);
			send_n(new_fd,(char*)&t,4+t.len);
		}
	}
}

void fileputs(char* buf,int new_fd)
{
	//system("clear");
	printf("upload file is %s\n",buf);
	train t;
	bzero(&t,sizeof(t));
	int fd;
	fd = open(buf,O_RDWR|O_CREAT,0666);
	if(-1==fd)
	{
		perror("open(buf,O_RDWR|O_CREAT,0666)");
		return;
	}
	printf("upload begin\n");
	while(1)
	{
		recv_n(new_fd,(char*)&t.len,4);
		bzero(t.buf,sizeof(t.buf));
		if(t.len>0)
		{
			recv_n(new_fd,t.buf,t.len);
			int ret = write(fd,t.buf,t.len);
			if(ret==-1)
			{	
				perror("write");
			}
		}else{
			break;
		}
	}
	printf("upload done\n");
	close(fd);
}

void filegets(char* buf,int new_fd)
{
	train t;
	int fd;
	fd = open(buf,O_RDONLY);
	//system("clear");
	printf("download file is%s\n",buf);
	printf("download begin!\n");
	while(bzero(&t,sizeof(t)),(t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	{
		send_n(new_fd,(char*)&t,4+t.len);
	}
	printf("download done!\n");
	t.len = 0;
	send_n(new_fd,(char*)&t.len,4);
	close(fd);
}

void fileremove(char* buf,int new_fd)
{	
	//system("clear");
	int ret;
	ret = unlink(buf);
	if(0 == ret)
	{
		printf("delete file %s succeed!\n",buf);
	}else{
		printf("delete file %s failed\n",buf);
	}
	return;
}

void pwd(int new_fd)
{
	train t;
	bzero(&t,sizeof(t));
	sprintf(t.buf,"%s",getcwd(NULL,0));
	t.len = strlen(t.buf);
	send_n(new_fd,(char*)&t.len,4);
	send_n(new_fd,t.buf,t.len);
}	
