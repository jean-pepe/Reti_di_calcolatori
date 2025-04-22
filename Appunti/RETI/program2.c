#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

int main(){
	int n;
	char *request = "GET / \r\n";
	struct sockaddr_in remote;
	/*METTO IO RESPONSE DIM = 1000000*/
	char response[1000000];
	int s;
	
	if((s=socket(AF_INET,SOCK_STREAM,0))==-1);
	{printf("errno=%d\n"); perror("Socket Fallitto"); return -1;}

	remote.sin_family = AF_INET;
	remote.sin_port = htons(80);
	unsigned char * p_IP=&remote.sin_addr.s_addr;
	p_IP[0]=142;/*IP SERVER*/
	p_IP[1]=250;
	p_IP[2]=180;
	p_IP[3]=3;
	
	if (connect(s, (struct sockaddr_in *)&remote,sizeof(struct sockaddr_in))== -1)
	{perror("Connect Fallitta"); return -1;}
	write(s,
		  request,
		  strlen(request));
		  int len=0;
		  for(len=0;(n = read(s,response + len,1000000-len))>0; len+=n);
			if(n==-1){perror("read fallita");return -1;}
			printf("%s \n", response);
	return 0;
}