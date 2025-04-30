#include <stdio.h>       // Libreria per le funzioni di input/output standard
#include <string.h>      // Libreria per le funzioni sulle stringhe
#include <errno.h>       // Libreria per la gestione degli errori
#include <unistd.h>      // Libreria per le funzioni POSIX, come sleep
#include <stdlib.h>      // Libreria standard per le funzioni generiche
#include <sys/socket.h>  // Libreria per le funzioni sui socket
#include <arpa/inet.h>   // Libreria per le funzioni di rete


int main() {
    char response[2000000]; // Buffer response
    char hbuffer[10000]; // Buffer  header

    char* request = "GET / HTTP/1.1\r\nHost:www.google.com\r\n\r\n";
    char* statusline;  // Status-Line response

    struct headers {
        char* name;
        char* value;
    } h[100]; //100 header
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    unsigned char* p = (unsigned char*)&server.sin_addr.s_addr; // Puntatore all'indirizzo IP
    p[0] = 172; p[1] = 217; p[2] = 169; p[3] = 4; // Indirizzo IP del server (142.250.187.196)
    //p[0]=127;p[1]=0;p[2]=0; p[3]=1; // Indirizzo IP alternativo (localhost)

    int sockID = socket(AF_INET, SOCK_STREAM, 0);
    if (sockID == -1) {printf("Errno = %d (%d)\n", errno, EAFNOSUPPORT); perror("Socket fallita:"); }

    int con = connect(sockID, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    if (con != 0) {perror("Connect fallita");return 1;}

    write(sockID, request, strlen(request)); // Invio della richiesta al server

    statusline = h[0].name = hbuffer; // Inizializzazione della riga di stato

    int i, index_buffer;   // Variabili per i socket e contatori
    for (index_buffer = 0, i = 0; 
         read(sockID, hbuffer + i, 1); 
        i++) { // Lettura della risposta dal server
        if ((hbuffer[i] == ':') && (h[index_buffer].value == NULL)) { // Separazione del nome e del valore degli header
            h[index_buffer].value = &hbuffer[i + 1]; // Assegna il puntatore al valore dell'header
            hbuffer[i] = 0; // Termina la stringa del nome dell'header
        }
        if (hbuffer[i] == '\n' && hbuffer[i - 1] == '\r') { // Fine riga header [\r\n]
            hbuffer[i - 1] = 0; // Termina la stringa dell'header
            if (h[index_buffer].name[0] == 0) break; // Se l'header è vuoto, termina il ciclo
            h[++index_buffer].name = &hbuffer[i + 1]; // Passa al prossimo header
        }
    }

    //Content-Length e Transfer-Econding non potranno mai essere presenti entrambi in una richiesta http
    //Content-Length ==> lunghezza completa del body
    //Transfer-Encoding chunked body 

#define CHUNKED -1; // Definizione del valore per il transfer encoding chunked
    int len;
    printf("%s\n", h[0].name); // Stampa la status line

    for (i = 1; i < index_buffer; i++) { // Iterazione sugli header letti
        printf("%s ----> %s\n", h[i].name, h[i].value); // Stampa gli header
        if (strcmp(h[i].name, "Content-Length") == 0) { // Se l'header è Content-Length (strcmp restituisce 0 se le 2 stringhe sono uguali)
            len = atoi(h[i].value); // Converte il valore in un intero
            printf("len = %d\n", len); // Stampa la lunghezza del contenuto
        }
        if (strcmp(h[i].name, "Transfer-Encoding") == 0) { // Se l'header è Transfer-Encoding
            if (strcmp(h[i].value + 1, "chunked") == 0) { // (+1 per togliere lo spazio dopo i ":" e prendere direttamente il valore dell'header) Se il valore è chunked
                len = CHUNKED; // Assegna a len il valore -1 per usarla come flag e indicare che stiamo lavorando con chunks
            }
            printf("%s / %d", h[i].value + 1, len); //Stampa il valore dell'header "Transfer-Encoding" (può essere "chunked" o altro) e la flag len (-1 se "chunked", 0 se altro)
        }
    }
    printf("Lunghezza: %d\n", len); // Stampa la lunghezza del contenuto (-1 se "Transfer-Encodiung" e CHUNKED, len>0 se "Content-Length")

    char lenbuf[100]; // Buffer per la lunghezza del chunk, serve solo per leggere la lunghezza di ogni chunk
    char crlf[2]; // Buffer per i caratteri CRLF ( più ordinato, serve solo per consumare i CRLF)
    int k = 0; //(Content- length) Contatore per la posizione nel buffer di risposta ( per mettere l'ultimo carattere della response = 0)
    int lendec = 0; // Lunghezza del chunk in decimale ( del chunck corrente)
    // sarebbe stato meglio mettere una condizione per verificare effettivamente se siamo in chunked o meno, perché cosi lo tratta come chunked di default
    //questo do while legge un chunk alla volta
    do {
        //es: un chunk è fatto così: "5\r\nHello\r\n"
       //prendi la lunghezza del chunk rispettivo
       //dal socket del client leggi un carattere alla volta e mettilo nel lenbuf, e scannerizza il carattere corrente del lenbuf 
        for (int i = 0; read(sockID, lenbuf + i, 1); i++) {
            if (lenbuf[i - 1] == '\r' && lenbuf[i] == '\n') { // Se il carattere corrente è \n e il precedente \r (CRLF) 
                lenbuf[i - 1] = '\0'; // => ho trovato la lunghezza esadecimale del chunk e termino la stringa
                break; // Esce dal ciclo
            }
        } //es: lenbuf="5\0\nHello\r\n"

         // Stampa la lunghezza del chunk in esadecimale (che è salvata in lenbuf)
        printf("lunghezza del chunck: %s\n", lenbuf);

        // strol converte la lunghezza del chunk in decimale (perché la lunghezza del chunk è in esadecimale)
        lendec = strtol(lenbuf, NULL, 16); //es: lendec=5

        // Stampa la lunghezza in decimale
        printf("lunghezza del chunck in decimale: %d\n", lendec);

        // Dal socket del client leggo il body del chunk sulla base di lendec e salvo il body del chunk nel buffer "response".
        // k tiene conto del carattere corrente nel buffer response e, siccome il buffer response deve contenere tutto il body della response, k non va azzerato ogni volta
        // ma, va incrementato alla lettura di ogni chunk 
        for (int i = 0; con = read(sockID, response + k, lendec - i); i += con, k += con); //leggi fino a lendec=5 caratteri da s2 e salvali in response ="Hello"

        // Consumo i caratteri CRLF perché non sono compresi nella lunghezza del chunk
        read(sockID, crlf, 2); // es: "\r\n"


    } while (lendec > 0); // Finchè non ho raggiunto l'ultimo chunk(perché ha lunghezza 0) ripeto


    response[k] = '\0'; //Metto l'ultimo carattere a \0 per terminare la stringa di response e poterla stampare=> es: response="Hello\0"
    printf("%s", response); // Stampa la risposta


}
/*
Response:
HTTP/1.1 200 OK\r\n
Date: Wed, 31 May 2024 10:44:49 GMT\r\n
Transfer-Encoding: chunked\r\n
Content-Type: text/plain\r\n
\r\n
5\r\n
Hello\r\n
7\r\n
 World!\r\n
0\r\n
\r\n

Output:
HTTP/1.1 200 OK --> NULL
Date -->  Wed, 31 May 2024 10:44:49 GMT
Transfer-Encoding --> chunked
Content-Type --> text/plain
Hello World!
 */
