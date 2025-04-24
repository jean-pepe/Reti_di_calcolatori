#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

//Il programma effettua una connessione a un server remoto tramite socket e invia una richiesta HTTP GET
int main() {


	
	char response[1000001];//buffer per memorizzare la risposta del server
	char* request = "GET / \r\n";  	//buffer dove è salvata la request




	struct sockaddr_in  server; 	//struttura che contiene le informazioni sul socket del server remoto	
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	unsigned char ipserver[4] = { 172,217,169,4 };//indirizzo IP del server
	server.sin_addr.s_addr = *((uint32_t*)ipserver);



	int sockID = socket(AF_INET, SOCK_STREAM, 0);
	if (sockID == -1) {
		printf("ernno = %d\n", errno);
		perror("Socket Fallita");
		return -1;
	}

	

	int con = connect(sockID, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
	if (con != 0) { perror("Connect Fallita"); return -1; }

	//faccio la write del buffer "request" sul socket del client "s"
	write(sockID, request, strlen(request));

	size_t byteLetti = 0;
	int n=0;
	
	for (byteLetti = 0; (n = read(sockID, response + byteLetti, 1000000 - byteLetti)) > 0; byteLetti += n);

	printf("%s\n", response);
}