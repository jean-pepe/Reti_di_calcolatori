#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

int main(){
	
	 int sockID = socket(AF_INET,/*IPV4*/
                        SOCK_STREAM,/*TCP*/
						0);

	struct sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(80);
	unsigned char * p_IP=&remote.sin_addr.s_addr;
	p_IP[0]=142;/*IP SERVER  --> E' sbagliato?????*/
	p_IP[1]=250;
	p_IP[2]=180;
	p_IP[3]=3;
	
	char response[1000001];/*char status_line*/
	char *request = "GET / \r\n";
	
	if(connect(sockID,(struct sockaddr*)&remote,sizeof(remote)) != 0);
		{perror("connect error \n");exit(EXIT_FAILURE);}

	write(sockID,request,strlen(request));
		  int n=0,len=0;
		  for(len=0;(n = read(sockID,response + len,1000000-len))>0; len+=n);
			if(n==-1){perror("read fallita");return -1;}
		printf("%s \n", response);
	return 0;
}