#include "process.h"

//recvFile()函数没有写
//
void getsalt(char *salt,char* passwd)
{
    int i,j;
    for(i=0,j=0;passwd[i]&&j!=3;i++)
    {
        if(passwd[i]=='$')
        {
            ++j;
        }
    }
    strncpy(salt,passwd,i-1);
}
int logfunc(int socketfd)
{
    int i,ret;
    tranData t;
    char acc[20],*passwd,cryptograph[1000],salt[100];
    for(i=0;i<3;++i)
    {
        system("clear");
        if(i>0) 
        {
            printf("账号或者密码输入错误%d次，请重新输入\n",i);
        }
        memset(acc,0,sizeof(acc));
        memset(cryptograph,0,sizeof(cryptograph));
        memset(salt,0,sizeof(salt));
        memset(&t,0,sizeof(tranData));
        printf("请输入账号:");
        scanf("%s",t.buf);
        passwd=getpass("请输入密码:");
        t.datalen=strlen(t.buf);
        printf("%s------%s\n",t.buf,passwd);
        send_n(socketfd,&t,4+t.datalen);//传输账户名
        recv_n(socketfd,&ret,4);
        printf("账户传输成功\n");
        if(ret==-1)
        {
            continue; 
        }
        memset(&t,0,sizeof(tranData)) ;
        recv_n(socketfd,&t.datalen,sizeof(int));
        recv_n(socketfd,t.buf,t.datalen);//获取账户名对应的密文
        printf("获取密文成功\n");  
        strcpy(cryptograph,t.buf);
        getsalt(salt,cryptograph); 
        if(!strcmp(cryptograph,crypt(passwd,salt)))
        {
            ret=1;
            send_n(socketfd,&ret,sizeof(int));
            return 1;
        }
        else{
            ret=-1;
            send_n(socketfd,&ret,sizeof(int));

        }

        system("clear");
    }
    return -1;

}
void command(int socketfd)
{
    tranData t;
    char *p;
    int ret;
    while(1)
    {
        memset(&t,0,sizeof(tranData));
        setbuf(stdin, NULL);
        fgets(t.buf,sizeof(t.buf),stdin);
        t.datalen=strlen(t.buf)-1;
        *(t.buf+t.datalen)='\0';
        send_n(socketfd,&t,4+t.datalen);
        if(strncmp(t.buf,"cd",2)==0)
        {
            memset(&t,0,sizeof(t));
            recv_n(socketfd,&t.datalen,sizeof(int));
            recv_n(socketfd,t.buf,t.datalen);
            printf("当前目录:%s\n",t.buf);
        }
        else if(strncmp(t.buf,"ls",2)==0)
        {
            memset(&t,0,sizeof(t));
            recv_n(socketfd,&t.datalen,sizeof(int));
            recv_n(socketfd,t.buf,t.datalen);
            if(t.datalen==0)
            {
                printf("当前目录下无文件或目录\n");
            }
            else
            {
                printf("%s",t.buf);

            }
        }
        else if(strncmp(t.buf,"pwd",3)==0)
        {
            memset(&t,0,sizeof(t));
            recv_n(socketfd,&t.datalen,sizeof(int));
            recv_n(socketfd,t.buf,t.datalen);
            printf("%s\n",t.buf);
        }
        else if(strncmp(t.buf,"puts",4)==0)
        {
            char buf[100]={0};//用于存放文件名
            strcpy(buf,t.buf+5);
            ret=access(buf,F_OK);
            send_n(socketfd,&ret,sizeof(int));//查看文件是否存在
            if(ret!=-1)
            {
                memset(&t,0,sizeof(t));
                printf("正在校验文件.....\n");
                ret=Compute_file_md5(buf,t.buf);
                send_n(socketfd,&ret,sizeof(int)); //1md5校验是否成功
                if(ret==0)
                {
                    t.datalen=strlen(t.buf);
                    send_n(socketfd,&t,4+t.datalen);//发送md5码
                    printf("正在上传中.....\n");
                    tranFile(socketfd,buf);
                    printf("%s 上传成功\n",buf);          
                }
            }else {
                printf("文件不存在\n");
            }
        }
        else if(strncmp(t.buf,"gets",4)==0)
        {
            p=t.buf+5;
            recv_n(socketfd,&ret,sizeof(int));
            if(ret!=-1)
            {
              if(-1!= recvFile(socketfd,p))
              {
                  printf("文件下载成功\n");
              }
              else
              {
                  printf("文件下载失败\n");
              }
            }

        }
    }
}
int main()
{

    int socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==socketfd)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in  addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(2000);
    inet_aton("192.168.3.228",&addr.sin_addr);
    int ret=connect(socketfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
    if(-1==ret)
    {
        perror("connect");
        return -1;
    }
    //连接成功开始进入登录注册界面
    int iNum;
    while(1)
    {
        system("clear");
        printf("请选择以下功能进行操作:\n1.登   录\n2.注    册\n0.退    出\n");
        scanf("%d",&iNum);
        switch(iNum)
        {
        case 1:goto log;
        case 2:goto nolog;//这个是注册按钮   现在不需要登录即可
        case 0:exit(0);
        }
    }
log:
    ret=logfunc(socketfd);
    if(ret==1)
    {
        printf("登录成功,1s后进入主目录\n");
        sleep(1);
    }
    else{
        printf("登录失败\n");
        exit(-1);
    }
nolog:
    system("clear");
    //开始接受第一个
    printf("我已经出来了\n");
    command(socketfd);
    exit(0);   
}

