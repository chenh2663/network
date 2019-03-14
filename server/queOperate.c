#include "workQue.h"
void queInsert(pQue_t pq,pNode_t pnew)
{
    if(pq->queHead==NULL)
    {
        pq->queHead=pq->queTail=pnew;
    }else{
        pq->queTail->ndnext=pnew;
        pq->queTail=pnew;
    }
    pq->queSize++;
}
void queGet(pQue_t pq,pNode_t *pcur)
{
    if(!pq->queSize)
    {  return;
    }
    *pcur=pq->queHead;
    pq->queHead=pq->queHead->ndnext;
    pq->queSize--;
}
