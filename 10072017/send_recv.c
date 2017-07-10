#include"socket_header.h"

void send_fd(int fdw,int fd){
	struct cmsghdr *cmsg=(struct cmsghdr *)calloc(1,CMSG_LEN(sizeof(fd)));
	cmsg->cmsg_len=CMSG_LEN(sizeof(fd));
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	int *fdptr=(int *)CMSG_DATA(cmsg);
	*fdptr=fd;

	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	msg.msg_control=cmsg;
	msg.msg_controllen=CMSG_LEN(sizeof(fd));

	struct iovec iov[3];
	char part1[6]="Hello";
	char part2[6]="World";
	char part3[3]="~\n";
	
	iov[0].iov_base=part1;
	iov[1].iov_base=part2;
	iov[2].iov_base=part3;
	
	iov[0].iov_len=5;	
	iov[1].iov_len=5;
	iov[2].iov_len=3;

	msg.msg_iov=iov;
	msg.msg_iovlen=3;

	int ret=sendmsg(fdw,&msg,0);
	if(-1==ret){
		perror("sendmsg");
		return;
	}
}


void recv_fd(int fdr,int* fd){
	struct cmsghdr *cmsg=(struct cmsghdr *)calloc(1,CMSG_LEN(sizeof(fd)));
	cmsg->cmsg_len=CMSG_LEN(sizeof(fd));
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	//int *fdptr=(int *)CMSG_DATA(cmsg);
	//*fdptr=fd;

	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	msg.msg_control=cmsg;
	msg.msg_controllen=CMSG_LEN(sizeof(fd));

	struct iovec iov[3];
	char part1[6]="Hello";
	char part2[6]="World";
	char part3[3]="~\n";
	
	iov[0].iov_base=part1;
	iov[1].iov_base=part2;
	iov[2].iov_base=part3;
	
	iov[0].iov_len=5;	
	iov[1].iov_len=5;
	iov[2].iov_len=3;

	msg.msg_iov=iov;
	msg.msg_iovlen=3;

	int ret=recvmsg(fdr,&msg,0);
	if(-1==ret){
		perror("recvmsg");
		return;
	}
	*fd=*(int *)CMSG_DATA(cmsg);
}
