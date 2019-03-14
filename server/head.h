#ifndef __HEAD_H__
#define __HEAD_H__
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<sys/wait.h>
#include<sys/select.h>
#include<sys/ipc.h>
#include<sys/time.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<signal.h>
#include<sys/msg.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <sys/socket.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include <shadow.h>
#include <crypt.h>
#include<mysql/mysql.h>
#define args_check(argc,num) {if(argc!=num){printf("error args!\n");return -1;}}
#endif

