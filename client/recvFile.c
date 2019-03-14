#include "process.h"
int recvFile(int socketfd,char *fileName)
  {

      tranData t;
      int datalen,ret;
      char buf[1000]={0};

      off_t totalSize=0; 
      recv_n(socketfd,&totalSize,sizeof(off_t));
     int fd=open(fileName,O_RDWR|O_CREAT,0644);
     if(-1==fd)
     {
         perror("open");
     }
     struct stat statbuf;
      fstat(fd,&statbuf);
      memcpy(t.buf,&statbuf.st_size,sizeof(statbuf.st_size));
      t.datalen=sizeof(statbuf.st_size);
      ret= send_n(socketfd,(void *)&t,4+t.datalen);
      if(ret==-1)
      {
          return -1;
      }
      int slice=totalSize/10000;
      int startSize=0,nowSize=0;
      time_t start,now;
      start=time(NULL);
      while(1)
      {
recv_n(socketfd,(void *)&datalen,sizeof(int));
          if(datalen>0)
          {
              recv_n(socketfd,(void *)buf,datalen);
              write(fd,buf,datalen);
              nowSize+=datalen;
              now=time(NULL);
              if(nowSize-startSize>=slice)
              {
                  printf("%5.2f%s\r",(double)nowSize/totalSize*100,"%");
                  startSize=nowSize;
              }
          }
          else
          {
              printf("100.00%s\n","%");
              printf("recv sucess\n");
              break;
          }
      }
      
          return 0;
 }

