#include"socket_header.h"
#include"make_child.c"
#include"send_recv.c"
#include"send_file.c"
int main(int argc,char **argv){
	if(argc!=4){
		printf("PLease input like this: [.outfile][IPaddress][Port][sum count of child process you want]");
		return -1;
	}

	//1. build the main server.(sock_fd->bind->listen)
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_addr.s_addr=inet_addr(argv[1]);
	addr.sin_port=htons(atoi(argv[2]));
	addr.sin_family=AF_INET;
	int bind_ret=bind(sfd,(struct sockaddr *)&addr,sizeof(addr));
	if(-1==bind_ret){
		perror("bind");
		return -1;
	}
	listen(sfd,atoi(argv[3]));

	//2.build ALL child proc
	struct child_info * pool=(struct child_info *)calloc(atoi(argv[3]),sizeof(struct child_info));
	make_child(pool,atoi(argv[3]));

	//3.epoll add + main proc add-in
	int epfd=epoll_create(1);
	struct epoll_event event;
	bzero(&event,sizeof(struct epoll_event));
	struct epoll_event *evs=(struct epoll_event *)calloc(atoi(argv[3])+1,sizeof(struct epoll_event));
	event.events=EPOLLIN;
	event.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);

	//4.loop-add every child-process to epfd
	for(int i=0;i<atoi(argv[3]);i++){
		event.events=EPOLLIN;
		event.data.fd=pool[i].tfds;
		epoll_ctl(epfd,EPOLL_CTL_ADD,pool[i].tfds,&event);
	}

	//5.loop-check
	char flag;//?????
	int wait_ret;
	while(1){
		wait_ret=epoll_wait(epfd,evs,atoi(argv[3])+1,-1);
		for(int i=0;i<wait_ret;i++){
			if(evs[i].data.fd==sfd){
				int new_fd=accept(sfd,NULL,NULL);
				for(int j=0;j<atoi(argv[3]);j++){
					if(0==pool[j].busy){
						send_fd(pool[j].tfds,new_fd);
						pool[j].busy=1;
						printf("new_fd has been send to a not busy process.\n");
						break;
					}
				}
				close(new_fd);
			}
			for(int i=0;i<atoi(argv[3]);i++){
				if(evs[i].data.fd==pool[i].tfds){
					read(pool[i].tfds,&flag,sizeof(flag));
					pool[i].busy=0;
					printf("child is not busy.\n");
//					break;
				}
			}
		}
	}
}
