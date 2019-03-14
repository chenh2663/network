#ifndef __WORKQUE_H__
#define __WORKQUE_H__
#include "head.h"
typedef struct {
      int accID;
      char accName[20];
      char cryptograph[512];
  }Acc_t,*pAcc_t;
typedef struct{
	int procode;
	int code;
	char name[20];
	char type;
	int accID;
	char md5value[50];
    off_t size;
}File_t,*pFile_t;
typedef struct tagQue{
int ndsfd;
Acc_t accMsg;
File_t fileMsg;
char nowPath[1000];
struct tagQue *ndnext;
}Node_t,*pNode_t;
typedef struct {
pNode_t queHead,queTail;
int queCapacity;
int queSize;
pthread_mutex_t mutex;
}Que_t,*pQue_t;
void queInsert(pQue_t pq,pNode_t pnew);
void queGet(pQue_t pq,pNode_t *pcur);
#endif
