#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

//fare una richiesta HTTP/1.0 e parsing
int main() {
	char response[1000001];//buffer per memorizzare la risposta del server
	char hbuffer[10000];	//buffer dove salvo la response (solo) fino alla fine degli header
	char* request = "GET / HTTP/1.0\r\n\r\n";	//buffer con request (qui sollo Request-Line senza URI)
	//il primo "\r\n" termina gli header 
	//il secondo "\r\n" termina l'entity body 


	char* statusline;	//puntatore status-line della response
	struct header {
		char* name;
		char* value;
	}h[100]; // creo un array di 100 struct header dove salvare gli header letti

	struct sockaddr_in server; //struttura che contiene le informazioni sul socket del server remoto	
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	unsigned char ipserver[4] = { 172,217,169,4 };
	server.sin_addr.s_addr = *((uint32_t*)ipserver);
	

	int sockID = socket(AF_INET, SOCK_STREAM, 0);//creo il Socket
	if (sockID == -1) {
		printf("ernno = %d\n", errno);
		perror("Socket Fallita");
		return -1;
	}

	if (connect(sockID, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) !=0) {//connect tra client e server
		perror("Connect Fallita");
		return -1;
	}

	write(sockID, request, strlen(request));	//write la richiesta sul Socket
	

	statusline = h[0].name = hbuffer;// punto statusline -> name primo header che punta -> inizio hbuffer
	int i; 	//i scorre il buffer "hbuffer"
	int index_header;

	for (i = 0, index_header = 0; 
		 read(sockID, hbuffer + i, 1); 	//per ogni ciclo, la read legge un 1 byte dal Socket del client e lo salva in hbuffer
		 i++) {
		if (hbuffer[i] == '\n' && hbuffer[i - 1] == '\r') { // guarda se i caratter sono \r\n (ovvero termina l'header )
			hbuffer[i - 1] = '\0';  // => metti il carattere i-1 a \O per terminare il valore dell'header e avere gli header in righe separate
			if (h[index_header].name[0] == '\0') break; // se header.name è '\0' => hbuffer vuoto => abbiamo finito gli header (ora inizia il body).
			h[++index_header].name = &hbuffer[i + 1]; // fai puntare il nome del prossimo header alla cella successiva 
		}
		if ((hbuffer[i] == ':') && (h[index_header].value == NULL)) {//se il carattere == : & value non inizializzato
			hbuffer[i] = '\0'; //metto \0 per indicare che ho finito il nome dell'header
			h[index_header].value = &hbuffer[i + 1]; // faccio puntare il value dell'header all'indirizzo del buffer successivo
		}
	}

	printf("%s\n", h[0].name);// stampo Status-Line (oss h[0].value == null

	
	for (i = 1; i < index_header; i++) {// index_header == size di h[]
		printf("%s : %s\n", h[i].name, h[i].value);
	}

	size_t byteLetti;
	int n;
	//leggo il body della response dal socket
	for (byteLetti = 0; (n = read(sockID, response + byteLetti, 1000000 - byteLetti)) > 0; byteLetti += n);
	response[byteLetti] = '\0';// metto alla fine di response '\0'

	printf("%s\n", response);
}
/* Response:
HTTP/1.0 200 OK\r\n
Content-Length 137\r\n
Connection: Keep-alive\r\n
\r\n
<html>\r\n
<head>\r\n
</head>\r\n
</html>\r\n


Output:
HTTP/1.0 200 OK
Content-Length ---> 137
Connection ---> Keep-alive

<html>
<head>
</head>
</html>

 */