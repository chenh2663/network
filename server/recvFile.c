#include"factory.h"
int deleteRecord(pNode_t pcur)
{
    pFile_t pf=&pcur->fileMsg;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char* user="root";
    char* password="1234";
    char* database="netdisk";//要访问的数据库名称
    char query[300]={0};
 int t;
    sprintf(query,"%s%d%s%s%s%d%s",\
                "delete from file where type='f' and procode=",pf->procode," and name='",pf->name,"' and accID=",pf->accID,";");
        puts(query);
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

int queryDelte(pNode_t pcur)
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
    int t;
    sprintf(query,"%s%d%s%s%s%d%s",\
            "select md5value from file where procode=",pf->procode," and name='",pf->name,"' and accID=",pf->accID,";");
    puts(query); 
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
        if(row!=NULL)
        {

                    strcpy(pf->md5value,row[0]);
    puts(pf->md5value);
                    //判断是否存在相同的md5的文件
            sprintf(query,"%s%s%s",\
                    "select * from file where md5value='",pf->md5value,"';");
            puts(query);
            deleteRecord(pcur);
            conn=mysql_init(NULL);
            if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
            {
                return -1;
            }
            t=mysql_query(conn,query);
            res=mysql_use_result(conn);
            row=mysql_fetch_row(res);
                if(row==NULL)
                {
                    printf("ulink   ====    %s\n",pf->md5value);
                    unlink(pf->md5value);


                }
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
int recvFile(int socketfd,pNode_t pcur)
{

    int datalen;
    char buf[1000]={0};
    recv_n(socketfd,(void *)&datalen,sizeof(int));
    recv_n(socketfd,(void *)buf,datalen);
    strcpy(pcur->fileMsg.name,buf);//用于存放发来的文件名字
    //删除数据库同等文件
    queryDelte(pcur);
    pcur->fileMsg.type='f';
    off_t totalSize=0;
    recv_n(socketfd,(void *)&datalen,sizeof(int));
    recv_n(socketfd,(void *)&totalSize,datalen);
    pcur->fileMsg.size=totalSize; 
    int slice=totalSize/10000;
    int startSize=0,nowSize=0;
    int fd=open(pcur->fileMsg.md5value,O_CREAT|O_RDWR,0644);
    if(-1==fd)
    {
        perror("open");
        return -1;
    }
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
        }else
        {
            printf("100.00%s\n","%");
            printf("recv sucess\n");
            break;
        }
        return 0;
    }
}
