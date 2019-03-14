#include "factory.h"
int exitFds[2];
void sigFunc(int signum)
{
    write(exitFds[1],&signum,1);
}
void cleanup(void *p)
{
    pthread_mutex_unlock(&((pQue_t)p)->mutex);
}
void *threadFunc(void *p)
{
    pFactory pf=(pFactory)p;
    pQue_t pq=&pf->que;
    pthread_cleanup_push(cleanup,pq);
    pNode_t pcur=NULL; 
    while(1)
    {
        pthread_mutex_lock(&pq->mutex);
        if(!pq->queSize)
        {
            pthread_cond_wait(&pf->cond,&pq->mutex);
        }
        queGet(pq,&pcur);
        pthread_mutex_unlock(&pq->mutex);

        if(pcur!=NULL)
        { 
            printf("客户端开始尝试登录\n");
           if( logfunc(pcur)==-1)
           {
               printf("客户端登录不成功\n");
               goto lable;
           }
           printf("客户端登录成功\n");
           childFunc(pcur);
           sleep(10);
            //      tranFile(pcur->ndsfd);
            free(pcur);
        }
lable:
        pcur=NULL;


    }
    pthread_cleanup_pop(1);
}
int main(int argc,char **argv)
{
    args_check(argc,5);
    pipe(exitFds);
    if(fork())
    {
        close(exitFds[0]);
        signal(SIGUSR1,sigFunc);
        wait(NULL);
        exit(0);
    }
    int threadNum=atoi(argv[3]);
    int queCapacity=atoi(argv[4]);
    Factory f;
    factoryInit(&f,threadNum,queCapacity,threadFunc);
    factoryStart(&f);
    int socketfd,ret;   
    ret=tcpInit(&socketfd,argv[1],argv[2]);
    if(-1==ret) {
        printf("TCPINIT FAILED!\n");
        return -1;
    }
    int epfd=epoll_create(1);
    struct epoll_event event,evs[2];
    event.events=EPOLLIN;
    event.data.fd=socketfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,socketfd,&event);
    event.data.fd=exitFds[0];
    epoll_ctl(epfd,EPOLL_CTL_ADD,exitFds[0],&event);
    int i,j,epollNum;
    int newFd;
    pNode_t pnew;
    pQue_t pq=&f.que;
    while(1)  
    {
        epollNum=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<epollNum;++i)
        {
            if(evs[i].data.fd==socketfd)

            {
                newFd=accept(socketfd,NULL,NULL);
                printf("NewFD is %d\n",newFd);
                pnew=(pNode_t)calloc(1,sizeof(Node_t));
                pnew->ndsfd=newFd; 
                pthread_mutex_lock(&pq->mutex);
                queInsert(pq,pnew);
                pthread_mutex_unlock(&pq->mutex);
                pthread_cond_signal(&f.cond);
            }
            if(evs[i].data.fd==exitFds[0])
            {
                event.events=EPOLLIN;
                event.data.fd=socketfd;
                epoll_ctl(epfd,EPOLL_CTL_DEL,socketfd,&event);
                for(j=0;j<threadNum;++j)
                {
                    pthread_cancel(f.pthID[j]);
                }
                for(j=0;j<threadNum;++j)
                {
                    pthread_join(f.pthID[j],NULL);
                }
                printf("the child thread exit sucess.\n");
                exit(0);
            }
        }
    }   
    return 0;
}

