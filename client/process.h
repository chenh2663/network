#include<func.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <errno.h>
#include <crypt.h>
#include "md5.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define READ_DATA_SIZE  1024
#define MD5_SIZE        16
#define MD5_STR_LEN     (MD5_SIZE * 2)
int Compute_file_md5(const char *file_path, char *value);
typedef struct {
pid_t pid;
int busy;//记录进程的状态   0为不忙碌   1为忙碌
int fds;
}pro_data;
typedef struct {
int datalen;
char buf[1000];
}tranData;
int tcp_init(int *sfd,char *ip,char *port);
void send_fd(int socketpipew,int fd);
void recv_fd(int socketpiper,int *fd);
void child_wait();
int create_child(int process_num,pro_data *pChild);
int send_n(int ,void *,int);
int recv_n(int ,void *,int );
void child_wait(int );
int tranFile(int new_fd,char *p);
int recvFile(int socketfd,char *fileName);
