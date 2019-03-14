#include "factory.h"
int queryAcc(pAcc_t pacc,char *acc)
{

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char* user="root";
    char* password="1234";
    char* database="netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"%s%s%s","select * from account where accName='",acc,"';");
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        return -1;
    }
    t=mysql_query(conn,query);
    if(t)
    {
        return -1;	
    }
    res=mysql_use_result(conn);
    if(res)
    {
        row=mysql_fetch_row(res);
    if(row==NULL) 
    {
        return -1;
    }   
    pacc->accID=atoi(row[0]);
        strcpy(pacc->accName,row[1]);
        strcpy(pacc->cryptograph,row[2]);
    }
    else{
        return -1;
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}
int logfunc(pNode_t pcur)
{
    tranData t;
    int i,ret;
    for(i=0;i<3;++i)
    {
        memset(&t,0,sizeof(tranData));
     recv_n(pcur->ndsfd,&t.datalen,sizeof(int));
        recv_n(pcur->ndsfd,t.buf,t.datalen);
       //
       printf("接收到的账户名%s\n",t.buf);
        ret=queryAcc(&pcur->accMsg,t.buf);
        if(ret==-1)
        {
            printf("获取密文失败\n");
           send_n(pcur->ndsfd,&ret,sizeof(int));//通知客户端在数据库中没有查询到此账户信息
            continue;
        }
        ret=1;
     send_n(pcur->ndsfd,&ret,sizeof(int));//通知客户端在数据库中查询到此账户信息
        strcpy(t.buf,pcur->accMsg.cryptograph);
        t.datalen=strlen(t.buf);
        send_n(pcur->ndsfd,&t,4+t.datalen);
        memset(&t,0,sizeof(tranData));
        recv_n(pcur->ndsfd,&ret,4);//用来记录客户端是否匹配成功
        if(ret==1)
        {
            printf("登录成功\n");
            pcur->nowPath[0]='/';
           pcur->fileMsg.accID=pcur->accMsg.accID; 
            return 1;
        }
    }
    return -1;
}



