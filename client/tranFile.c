#include  "process.h"
int tranFile(int new_fd,char *p)
{
    tranData t;
    memset(&t,0,sizeof(tranData));
    //传送文件名
    strcpy(t.buf,p);
    t.datalen=strlen(p);
    int ret;
    ret=send_n(new_fd,(void *)&t,4+t.datalen); 
    if(ret==-1)
    {
        return -1;
    }
    //下面开始传送内容
    int fd=open(p,O_RDONLY,0644);
    if(-1==fd)
    {
        perror("open ");
    }
    struct stat statbuf;
    fstat(fd,&statbuf);
    memcpy(t.buf,&statbuf.st_size,sizeof(statbuf.st_size));
    t.datalen=sizeof(statbuf.st_size);
    ret= send_n(new_fd,(void *)&t,4+t.datalen);
    if(ret==-1)
    {
        return -1;
    }
    while(t.datalen=read(fd,t.buf,sizeof(t.buf))>0)
    {
        ret=send_n(new_fd,(void *)&t,4+t.datalen); 
        if(ret==-1)
        {
            return -1;
        }
    }
    ret= send_n(new_fd,(void *)&t,4+t.datalen); 
    if(ret==-1)
    {
        return -1;
    }
}
