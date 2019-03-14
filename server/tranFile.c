#include  "tranfile.h"
//   atoli
int queryFileName(pNode_t pcur,char *p)
{
    pFile_t pfile=&pcur->fileMsg;

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char* user="root";
    char* password="1234";
    char* database="netdisk";//要访问的数据库名称
    char query[300]={0};
    
    sprintf(query,"%s%d%s%s%s%d%s","select * from file where procode=",pfile->procode," and name='",p,"' and accID=",\
            pfile->accID," and type='f';");
    
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
    return 0;
}

int tranFile(pNode_t pcur,char *p)
{
    int new_fd=pcur->ndsfd;
    pFile_t pf=&pcur->fileMsg;
    tranData t;
    int ret=-1;
    ret=queryFileName(pcur,p);
    send_n(new_fd,&ret,sizeof(int));//1111111传送返回值
   printf("query返回值%d\n",ret); 
    if(ret==-1)
    {
        return -1;
    }
    memset(&t,0,sizeof(tranData));
    //传送文件大小
   printf("%ld\n",pf->size); 
    ret=send_n(new_fd,&pf->size,sizeof(off_t));//222222传送文件大小 
    
    //接受客户端大小
    int fd=open(pf->md5value,O_RDONLY,0755);
    if(-1==fd)
    {
        perror("open ");
    }
    off_t cliSize;
    recv_n(new_fd,&t.datalen,sizeof(int));
    recv_n(new_fd,&cliSize,t.datalen);
    printf("接收到clisize大小%ld\n",cliSize);
    lseek(new_fd,SEEK_SET,cliSize);
    while(t.datalen=read(fd,t.buf,sizeof(t.buf)))
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
