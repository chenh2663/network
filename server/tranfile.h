#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "factory.h"
typedef struct {
int datalen;
char buf[1000];
}tranData;

int tranFile(pNode_t pcur,char *p);
int send_n(int new_fd,void *p,int len);
int recv_n(int fd,void *p,int len);
#endif

