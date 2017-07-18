#include<mysql/mysql.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<string.h>
#include<sys/uio.h>
#include<fcntl.h>

#include<pwd.h>
#include<grp.h>
#include<dirent.h>
#include<signal.h>
#include<shadow.h>
#include<error.h>
#include<sys/mman.h>

#define USER_ROOT_DIR "/home/syou/ftp1/server/users"
#define MYSQLSERVERNAME "localhost"
#define MYSQLUSERNAME "root"
#define MYSQLPASSWD "Orekiyuki2me"
#define MYSQLDATABASE "users"
#define MYSQL_USERINFO_TABLE "login"
#define MYSQL_LOGFILE_TABLE "logfile"

#define SCREENWIDE 20
#define SCREENROW 11
#define FILENAME file
#define FILENAMELEN 4
#define TRAIN_BUFLEN 1000

typedef struct loginfo{
        char ip[48];
        char port[8];
        char user[24];
        char ops[40];
}loginfo_t,*ploginfo_t;

typedef struct{
	pid_t pid;
	int fdw;
	short busy;
}cdata,*pcdata;

typedef struct{
	int len;
	char buf[1000];
}train,*ptrain;

typedef struct train{
	int dlen;
	char dbuf[TRAIN_BUFLEN];
}train_t,*p_train_t;

#define LOGFILENAME "../FTP_LOG"
