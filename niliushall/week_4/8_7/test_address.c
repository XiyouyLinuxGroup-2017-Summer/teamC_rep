#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	char buffer[32];
	int ret = 0;
	int host = 0;
	int network = 0;
	unsigned int address = 0;
	struct in_addr in;

	in.s_addr = 0;

	printf("input a ip:  ");
	fgets(buffer, 31, stdin);
	buffer[31] = 0;

	if((ret = inet_aton(buffer, &in)) == 0)
		printf("inet_aton: invalid address\n");
	else
		printf("inet_aton: 0x%x\n", in.s_addr);

	if((address = inet_addr(buffer)) == INADDR_NONE)
		printf("inet_addr: invalid address\n");
	else
		printf("inet_addr: 0x%lx\n",address);

	return 0;
}