#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

int main(){
	FILE *f;
	struct sockaddr_in remote;
	int s;
	if((s=socket(AF_INET,77,0))==-1);
	{printf("errno=%d\n"); perror("Socket Fallitto"); return -1;}

	remote.sin_family = AF_INET;
	remote.sin_port = htons(80);
	unsigned char * p_IP=&remote.sin_addr.s_addr;
	p_IP[0]=142;
	p_IP[1]=250;
	p_IP[2]=180;
	p_IP[3]=3;
	if (connect(s, (struct sockaddr_in *)&remote,sizeof(struct sockaddr_in))== -1)
	{perror("Connect Fallitta"); return -1;}

	return 0;
}