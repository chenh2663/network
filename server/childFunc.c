#include "factory.h"
int queryCode(pFile_t pfile)
{

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char* user="root";
    char* password="1234";
    char* database="netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"%s%d%s","select * from file where code=",pfile->procode,";");
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
            return -1;
        pfile->procode=atoi(row[0]);
        pfile->code=atoi(row[1]);
        strcpy(pfile->name,row[2]);
        strncpy(&pfile->type,row[3],1);
        pfile->accID=atoi(row[4]);
        strcpy(pfile->md5value,row[5]);
        pfile->size=atol(row[6]);
        printf("procode=%d\n",pfile->procode);

    }
    else
    {
        return -1;
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}
int queryCddir(pFile_t pfile,char *p)
{

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char* user="root";
    char* password="1234";
    char* database="netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"%s%d%s%s%s","select * from file where procode=",pfile->procode," and name='",p,"' and type='d';");
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
            return -1;
        pfile->procode=atoi(row[1]);
        printf("procode=%d\n",pfile->procode);
    }
    else
    {
        return -1;
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}
int cd_func(pNode_t pcur,char *p)
{
    tranData t; 
    char *q;
    memset(&t,0,sizeof(tranData));
    if(strcmp(p,"..")==0)
    {
        if(strlen(pcur->nowPath)>1)
        {
            q=pcur->nowPath+strlen(pcur->nowPath)-1;
            while(q>(pcur->nowPath+1)&&*q!='/')
            {
                --q;
            }
            *q='\0';
            queryCode(&pcur->fileMsg);
        }
        strcpy(t.buf,pcur->nowPath);
        t.datalen=strlen(pcur->nowPath);
        send_n(pcur->ndsfd,&t,4+t.datalen);


    }
    else if(strcmp(p,".")==0)
    {
        strcpy(t.buf,pcur->nowPath);
        t.datalen=strlen(pcur->nowPath);
        send_n(pcur->ndsfd,&t,4+t.datalen);
    }
    else{

        int ret=queryCddir(&pcur->fileMsg,p);
        if(ret==1)
        {
            if(strlen(pcur->nowPath)>1)
            {
                strcat(pcur->nowPath,"/");
            }
            strcat(pcur->nowPath,p);
        }
        strcpy(t.buf,pcur->nowPath);
        t.datalen=strlen(pcur->nowPath);
        send_n(pcur->ndsfd,&t,4+t.datalen);
        //绝对路径      先不写！！！！！！！
    }
    return 1;
}
int queryLs(pFile_t pfile,tranData *p)
{

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char* user="root";
    char* password="1234";
    char* database="netdisk";//要访问的数据库名称
    char query[300]={0};
    File_t f;
    sprintf(query,"%s%d%s%d%s","select * from file where procode=",pfile->procode," and accID=",pfile->accID,";");
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
        while(( row=mysql_fetch_row(res))!=NULL)
        {
            memset(&f,0,sizeof(File_t));
            f.procode=atoi(row[0]);
            f.code=atoi(row[1]);
            strcpy(f.name,row[2]);
            strcpy(&f.type,row[3]);
            f.accID=atoi(row[4]);
            strcpy(f.md5value,row[5]);
            f.size=(off_t)atoi(row[6]);
            sprintf(p->buf,"%s%c%-4c%-20s%-20ld\n",p->buf,'-',f.type,f.name,f.size);
        }
    }
    else
    {
        return -1;
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

void ls_func(pNode_t pcur)
{
    tranData *p=(tranData *)calloc(1,sizeof(tranData));
    queryLs(&pcur->fileMsg,p);
    p->datalen=strlen(p->buf);
    send_n(pcur->ndsfd,p,4+p->datalen);

}
int queryFile(pNode_t pcur)
{

    pFile_t  pf=&pcur->fileMsg;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char* user="root";
    char* password="1234";
    char* database="netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"%s%d%s%s%s%c%s%d%s%s%s%ld%s",\
            "insert into file(procode,name,type,accID,md5value,size) values(",pf->procode,",'",pf->name,"','",pf->type,"',"\
            ,pf->accID,",'",pf->md5value,"',",pf->size,");");
    puts(query); 
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
    mysql_close(conn);
    return 1;
}
void childFunc(pNode_t pcur)
{
    tranData t;
    char *p;
    int sfd=pcur->ndsfd;
    while(1)
    {
        memset(&t,0,sizeof(tranData));
        recv_n(sfd,&t.datalen,sizeof(int));
        recv_n(sfd,t.buf,t.datalen);
        if(strncmp(t.buf,"cd",2)==0)
        {
            p=t.buf+3;
            cd_func(pcur,p);
        }
        else if(strncmp(t.buf,"ls",2)==0)
        {
            ls_func(pcur);
        }
        else if(strncmp(t.buf,"pwd",3)==0)
        {
            memset(&t,0,sizeof(tranData));
            t.datalen=strlen(pcur->nowPath);
            strcpy(t.buf,pcur->nowPath); 
            send_n(pcur->ndsfd,&t,4+t.datalen);
        }
        else if(strncmp(t.buf,"puts",4)==0)
        {
            int ret=-1;
            recv_n(pcur->ndsfd,&ret,sizeof(int)); //文件是否存在
            if(ret==0)
            {
                recv_n(pcur->ndsfd,&ret,sizeof(int)); //校验码
                if(ret==0) 
                {
                    memset(&t,0,sizeof(t));
                    recv_n(pcur->ndsfd,&t,sizeof(int));
                    recv_n(pcur->ndsfd,t.buf,t.datalen);//接收文件md5码
                    strcpy(pcur->fileMsg.md5value,t.buf);//校验码存放结构体中
                    printf("%s\n",t.buf); 
                    if(!recvFile(pcur->ndsfd,pcur))//接收文件
                    {
                        strcpy(pcur->fileMsg.md5value,t.buf);
                        printf("%ld\n",pcur->fileMsg.size);
                        queryFile(pcur);
                        printf("%ld\n",pcur->fileMsg.size);
                    }

                }
            }
        }
        else if(strncmp(t.buf,"gets",4)==0)
        {
            p=t.buf+5;
            tranFile(pcur,p);
        }
        else if(strncmp(t.buf,"remove",6)==0)
        {
            p=t.buf+7;
            strcpy(pcur->fileMsg.name,p);
            queryDelte(pcur);

        }
    
    }
}
