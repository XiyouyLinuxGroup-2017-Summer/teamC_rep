/*************************************************************************
	> File Name: test_1.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月09日 星期三 19时42分05秒
 ************************************************************************/

#include<stdio.h>

#include<mysql.h>

int main(void) {
    MYSQL * conn=NULL;

    conn=mysql_init(NULL);

    if(mysql_real_connect(conn,"localhost","root","","newer",3306,NULL,0)==NULL)  {
        printf("coonect eror is %s\n",mysql_error(conn));
        return -1;
}

    printf("connect success\n");
    mysql_close(conn);

    return 0;
}

/*
void *mysql_real_connect(MYSQL* conn,char* servername,char * username,char* password,char*dbname,int prot,void* NULL,int fd)

参数说明：
    MYSQL* conn//上面初始化的服务器句柄

    char* servername//数据库服务所在的服务器的名称或IP地址

    char * username//登入数据的用户

    char* password//用户的密码

    char* dbname//具体的数据库名

    int prot//服务端口号3306 ，也可以写0
	
    NULL

    0
*/
