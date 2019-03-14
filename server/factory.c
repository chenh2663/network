#include "factory.h"
void factoryInit(pFactory pf,int threadNum,int queCapacity,threadFunc_t threadFunc)
{
    memset(pf,0,sizeof(Factory));
    pf->pthID=(pthread_t *)calloc(threadNum,sizeof(pthread_t));
    pf->threadNum=threadNum;
    pf->que.queCapacity=queCapacity;
    pthread_mutex_init(&pf->que.mutex,NULL);
    pf->downFilefunc=threadFunc;
    pthread_cond_init(&pf->cond,NULL);
    return;

}
void factoryStart(pFactory pf)
{
    if(pf->startFlag!=0)
    {
        return;
    }
    int i;
    for(i=0;i<pf->threadNum;++i)
    {
        pthread_create(pf->pthID+i,NULL,pf->downFilefunc,pf);
    }
    pf->startFlag=1;

}
