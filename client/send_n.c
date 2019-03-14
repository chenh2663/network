#include "process.h"


int send_n(int new_fd,void *p,int len)
{
    char *ptran=(char *)p;
    int total=0;
    int ret;
    while(total<len)
    {
   ret=send(new_fd,ptran+total,len-total,0);
   total+=ret; 
    }
return 0;
}
int recv_n(int fd,void *p,int len)
{
    char *ptran=(char *)p;
    int total=0;
    int ret;
    while(total<len)
    {
   ret=recv(fd,ptran+total,len-total,0);
   total+=ret; 
    }
return 0;
}


