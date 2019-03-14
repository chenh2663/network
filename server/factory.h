#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "workQue.h"
#include "head.h"
#include "tranfile.h"
typedef void *(*threadFunc_t)(void *);
typedef struct {
pthread_t  *pthID;
int threadNum;
Que_t que;
threadFunc_t downFilefunc;
pthread_cond_t cond;
short startFlag;
}Factory,*pFactory;
void factoryInit(pFactory pf,int threadNum,int queCapacity,threadFunc_t threadFunc);
void factoryStart(pFactory pf);
int tcpInit(int *,char *,char *);
int queryAcc(pAcc_t pacc,char *acc);
void childFunc(pNode_t pcur);
int logfunc(pNode_t pcur);
int recvFile(int socketfd,pNode_t pcur);
int queryFile(pNode_t pcur);
int queryDelte(pNode_t pcur);
#endif
