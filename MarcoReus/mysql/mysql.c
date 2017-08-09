/*************************************************************************
	> File Name: mysql.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月09日 星期三 19时52分05秒
 ************************************************************************/

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <mysql/mysql.h>  


#define HOST    "localhost"
#define USER    "root"
#define PASSWD  "251024yin"
#define DB_NAME "happychat"


void my_error(const char *err_string, int line);    // 错误处理函数
void mysql_connect(MYSQL *mysql);                   // 连接mysql数据库
void show_databases(MYSQL *mysql);                  // 显示所有数据库
void change_database(MYSQL *mysql);                 // 改变数据库
void drop_database(MYSQL *mysql);                   // 删除数据库
void show_tables(MYSQL *mysql);                     // 显示当前数据库所有表
void desc_table(MYSQL *mysql);                      // 显示表的字段信息
void drop_table(MYSQL *mysql);                      // 删除表
void print_table(MYSQL *mysql);                     // 显示表中内容
void insert_data(MYSQL *mysql);                     // 插入数据
void update_data(MYSQL *mysql);                     // 修改数据
void delete_data(MYSQL *mysql);                     // 删除数据
void close_connection(MYSQL *mysql);                // 关闭mysql数据库


int main(int argc, char *argv[])
{  
    int choice;  
    char *dir = 
        "\t1.Show databases\n\
        2.Change database\n\
        3.Drop database\n\
        4.Show tables\n\
        5.Desc table\n\
        6.Drop table\n\
        7.Print table\n\
        8.Insert table\n\
        9.Update table\n\
        10.Delete data";

    MYSQL *mysql = mysql_init(NULL);           // 创建一个MYSQL句柄并初始化

    if (!mysql) {
        my_error("mysql_init", __LINE__);
    }

    // 连接MYSQL数据库
    mysql_connect(mysql);

    while (1) {  
        printf("%s\n",dir);  
        printf("\nYour choice: ");  
        scanf("%d", &choice);  
        switch(choice) {  
        case 1:
            show_databases(mysql);
            break;  
        case 2:
            change_database(mysql);
            break;  
        case 3:
            drop_database(mysql);
            break;  
        case 4:
            show_tables(mysql);
            break;  
        case 5:
            desc_table(mysql);
            break;  
        case 6:
            drop_table(mysql);
            break;
        case 7:
            print_table(mysql);
            break;
        case 8:
            insert_data(mysql);
            break;
        case 9:
            update_data(mysql);
            break;
        case 10:
            delete_data(mysql);
            break;
        default:
            printf("No your choice!\n");
            break;
        }  
    }

    close_connection(mysql);
    return 0;  
}  

void my_error(const char *err_string, int line) 
{
    fprintf(stderr, "line: %d ", line);
    perror(err_string);
    exit(1);
}

/* 连接mysql数据库 */  
void mysql_connect(MYSQL *mysql)
{  
    if(!mysql_real_connect(mysql, HOST, USER, PASSWD, DB_NAME, 0, NULL, 0)) {  
        printf("Failed to connect:%s\n", mysql_error(mysql));  
        my_error("mysql_real_connect", __LINE__);
    }  
    printf("Connect database sucessfully!\n\n");
}  

/* 显示所有数据库 */
void show_databases(MYSQL *mysql)  
{  
    int t;  
    char *query ="show databases";
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据
    t = mysql_real_query(mysql, query, strlen(query));  
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));
        return;
    }  
    res = mysql_store_result(mysql);
    printf("\n");
    while (row = mysql_fetch_row(res)) {
        for(t = 0; t < mysql_num_fields(res); t++) {
            printf("%s\t",row[t]);
        }  
        printf("\n");
    }
    mysql_free_result(res);
}

/* 改变数据库 */  
void change_database(MYSQL *mysql)  
{  
    int t;  
    char *head = "use ";  
    char query[30];
    char db_name[20];
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    printf("Please enter the database to change: ");
    scanf("%s", db_name);
    sprintf(query, "%s%s", head, db_name);

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }  
    printf("\nChange sucessfully!\n\n");  
}

/* 删除数据库 */
void drop_database(MYSQL *mysql)
{
    int t;  
    char *head = "drop database ";  
    char query[50];
    char db_name[20];
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    printf("Please enter the database to drop: ");
    scanf("%s", db_name);
    sprintf(query, "%s%s", head, db_name);

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }  
    printf("\nDrop database sucessfully!\n\n");  
}

/* 打印当前数据库中的表 */
void show_tables(MYSQL *mysql)
{  
    int t;  
    char *query = "show tables";  
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    t = mysql_real_query(mysql, query, strlen(query));  
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));
        return;
    } 
    res = mysql_store_result(mysql);
    printf("\n");
    while (row = mysql_fetch_row(res)) {
        for(t = 0; t < mysql_num_fields(res); t++) {
            printf("%s\t", row[t]);
        }
        printf("\n");  
    }  
    mysql_free_result(res);
}

/* 打印表的字段 */  
void desc_table(MYSQL *mysql)
{  
    int t;  
    char *head = "desc ";
    char table_name[20];  
    char query[30];  
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    memset(table_name, '\0', sizeof(table_name));
    memset(query, '\0', sizeof(query));

    printf("Please enter the table name: ");  
    scanf("%s", table_name);  
    sprintf(query, "%s%s", head, table_name);

    t = mysql_real_query(mysql, query, strlen(query));  
    if (t) {  
        printf("Failed to query: %s\n",mysql_error(mysql));  
        return;
    }  
    res = mysql_store_result(mysql);
    printf("\n");
    while (row = mysql_fetch_row(res)) {
        for(t = 0; t < mysql_num_fields(res); t++) {  
            printf("%s\t",row[t]);
        }  
        printf("\n");  
    } 
    mysql_free_result(res);
}  

/* 打印表中数据 */
void print_table(MYSQL *mysql)  
{  
    int t;
    char *head = "select * from ";
    char table_name[20];
    char query[50];
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    memset(query, '\0', sizeof(query));
    memset(table_name, '\0', sizeof(table_name));

    printf("Please enter table name: ");  
    scanf("%s", table_name);
    sprintf(query, "%s%s", head,table_name);

    t = mysql_real_query(mysql, query, strlen(query));  

    if (t) {
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }  
    printf("\nQuery successfully!\n\n");  

    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res)) {  
        for(t = 0; t < mysql_num_fields(res); t++) {  
            printf("%s\t", row[t]);  
        }  
        printf("\n");
    }  
    mysql_free_result(res);
}  

/* 删除表 */
void drop_table(MYSQL *mysql)
{
    int t;  
    char *head = "drop table ";  
    char query[50];
    char table_name[20];
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    printf("Please enter the table to drop: ");
    scanf("%s", table_name);
    sprintf(query, "%s%s", head, table_name);

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }  
    printf("\nDrop table sucessfully!\n\n");  
}

/* 插入数据 */
void insert_data(MYSQL *mysql)
{  
    int t;  
    char *head = "insert into ";  
    char query[200];
    char table_name[20];  
    char field[50];
    char *left = "(";  
    char *right = ") ";  
    char *values = "values";  
    char message[100];  
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    memset(field, '\0', sizeof(field));
    memset(table_name, '\0', sizeof(table_name));
    memset(message, '\0', sizeof(message));

    printf("Please enter table_name: ");  
    scanf("%s", table_name); 

    printf("Please enter the Field you want to write(split with comma): ");
    scanf("%s", field);  

    printf("Please write the message to the field(split with comma): ");
    scanf("%s", message);  

    /* 把几个变量字符串连接成一个完整的mysql命令 */  
    sprintf(query, "%s%s%s%s%s%s%s%s%s", head, table_name, left, 
                            field, right, values, left, message, right);
    printf("%s\n", query);  

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }  
    printf("\nInsert sucessfully!\n\n");
}  

/* 删除数据 */
void delete_data(MYSQL *mysql)
{  
    int t;
    char *head = "delete from ";  
    char query[200];  
    char table_name[20];  
    char del_name[20];  
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    memset(table_name, '\0', sizeof(table_name));
    memset(del_name, '\0', sizeof(del_name));

    printf("Please enter the table: ");  
    scanf("%s",table_name);  

    printf("please enter del name: ");  
    scanf("%s",del_name);  

    sprintf(query, "%s%s where name =\"%s\"", head, table_name, del_name);
    printf("%s\n", query);

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }
    printf("Delete data sucessfully!\n");  
}  

/* 更新数据 */
void update_data(MYSQL *mysql)
{  
    int t;
    char *head = "update ";
    char query[100];
    char table_name[20];  
    char field_name[20];
    char up_value[30];
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    memset(table_name, '\0', sizeof(table_name));
    memset(field_name, '\0', sizeof(field_name));
    memset(up_value, '\0', sizeof(up_value));

    printf("Please enter the table_name: ");  
    scanf("%s", table_name);  

    printf("please enter field_name: ");
    scanf("%s", field_name);  

    printf("please enter up value: ");
    scanf("%s", up_value);  

    sprintf(query, "%s%s set %s=\"%s\"", head, table_name, field_name, up_value);
    printf("%s\n", query);

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }
    printf("Update data sucessfully!\n");
}

void close_connection(MYSQL *mysql)
{
    mysql_close(mysql);
}
