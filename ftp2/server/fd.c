#include"func.h"

void send_fd(int fdw,int new_fd)
{
	struct msghdr msg;
	bzero(&msg,sizeof(msg));
	char buf1[5]="Hi";
	char buf2[10]="hello";
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 2;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr* cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg) = new_fd;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret;
	ret = sendmsg(fdw,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
}	

void recv_fd(int fdr,int* new_fd)
{
	struct msghdr msg;
	bzero(&msg,sizeof(msg));
	char buf1[5]="Hi";
	char buf2[10]="hello";
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 2;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr* cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret;
	ret = recvmsg(fdr,&msg,0);
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}
	*new_fd = *(int*)CMSG_DATA(cmsg);
}	

