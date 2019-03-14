#include "head.h"
int tcpInit(int *sfd,char *ip,char *port)
{
    int socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==socketfd)
    {
        perror("socketfd");
        return -1;
    }
    struct sockaddr_in ser;
    ser.sin_family=AF_INET;
    ser.sin_port=htons(atoi(port));
    int ret=inet_pton(AF_INET,ip,&ser.sin_addr);
    if(ret==-1)
    {
        perror("inet_pton");
        return -1;
    }
   int reuse=1;
   setsockopt(socketfd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&reuse,sizeof(int));
    ret=bind(socketfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
    if(-1==ret)
    {
        perror("bind");
        return -1;
    }
     listen(socketfd,10);
     *sfd=socketfd;
}
