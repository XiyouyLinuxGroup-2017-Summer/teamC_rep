/*************************************************************************
  > File Name: ioctl_net.c
  > Author: YinJianxiang
  > Mail: YinJianxiang123@gmail.com 
  > Created Time: 2017年07月18日 星期二 10时40分51秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<net/if.h>

unsigned char g_eth_name[16];
unsigned char g_macaddr[6];
unsigned int g_subnetmask;
unsigned int g_ipaddr;
unsigned int g_broadcast_ipaddr;


void init_net(void) {
	int i;
	int sock;
	struct sockaddr_in sin;//和网路有关的结构体
	/*
	   sockaddr_in（在netinet/in.h中定义）：
	   struct  sockaddr_in {
	   short  int  sin_family;                       Address family 
	   unsigned  short  int  sin_port;        Port number 
	   struct  in_addr  sin_addr;               Internet address 
	   unsigned  char  sin_zero[8];          Same size as struct sockaddr 
	   };
	   struct  in_addr {
	   unsigned  long  s_addr;
	   };

	   typedef struct in_addr {
	   union {
	   struct{
	   unsigned char s_b1,
	   s_b2,
	   s_b3,															
	   s_b4;
	   } S_un_b;
	   struct {

	   unsigned short s_w1,
	   s_w2;
	   } S_un_w;
	   unsigned long S_addr;
	   } S_un;
	   } IN_ADDR;

	   sin_family指代协议族，在socket编程中只能是AF_INET
	   sin_port存储端口号（使用网络字节顺序）
	   sin_addr存储IP地址，使用in_addr这个数据结构
	   sin_zero是为了让sockaddr与sockaddr_in两个数据结构保持大小相同而保留的空字节。
	   s_addr按照网络字节顺序存储IP地址

	   sockaddr_in和sockaddr是并列的结构，指向sockaddr_in的结构体的指针也可以指向
	   sockadd的结构体，并代替它。也就是说，你可以使用sockaddr_in建立你所需要的信息,
	   }
	   }
	   }
	   }
	   }
	   }
	   */
	struct ifreq ifr;

	sock = socket(AF_INET,SOCK_DGRAM,0);//打开一个tcp连接
	if(sock == -1) {
		perror("socket");
	}
	strcpy(g_eth_name,"eth0");
	strcpy(ifr.ifr_name,g_eth_name);
	printf("eth name:\t%s\n",g_eth_name);

	//获取并打印网卡地址
	if(ioctl(sock,SIOCGIFHWADDR,&ifr) < 0) {
		perror("ioctl");
	}

	memcpy(g_macaddr,ifr.ifr_hwaddr.sa_data,6);
	
	printf("local mac:\t");
	for(i = 0;i < 5;i++) {
		printf("%.2x\n",g_macaddr[i]);
	}	
	printf("%.2x\n",g_macaddr[i]);

	//获取并打印IP地址
	if(ioctl(sock,SIOCGIFADDR,&ifr) < 0) {
		perror("ioctl");
	}

	memcpy(&sin,&ifr.ifr_addr,sizeof(sin));
	g_ipaddr = sin.sin_addr.s_addr;
	printf("local eth0:\t%s\n",inet_ntoa(sin.sin_addr));

	//获取并打印广播地址
	if(ioctl(sock,SIOCGIFBRDADDR,&ifr) < 0) {
		perror("ioctl");
	}
	memcpy(&sin,&ifr.ifr_addr,sizeof(sin));
	g_ipaddr = sin.sin_addr.s_addr;
	printf("local eth0:\t%s\n",inet_ntoa(sin.sin_addr));

	//获取并打印子网掩码
	if(ioctl(sock,SIOCGIFBRDADDR,&ifr) < 0) {
		perror("ioctl");
	}
	memcpy(&sin,&ifr.ifr_addr,sizeof(sin));
	g_subnetmask = sin.sin_addr.s_addr;
	printf("subnetmask:\t%s\n",inet_ntoa(sin.sin_addr));

	close(sock);
}


int main(void) {
	init_net();

	return 0;
}


