//Un client trimite unui server un sir de caractere. Serverul va returna clientului numarul de caractere spatiu din sir.

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/ip.h>
#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

unsigned int STR_SIZE = 100;

int main() {
    int clientFd; // file descriptor-ul pentru client
    struct sockaddr_in server; // server-ul la care se va conecta clientul

    // incercam initializarea socket-ului pentru client
    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Eroare la crearea socket-ului pentru client: ");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234); // setam portul pe care server-ul va lucra
    server.sin_family = AF_INET; // selectam domeniul din care face parte server-ul
    server.sin_addr.s_addr = INADDR_ANY; // selectam adresa pe care functiona server-ul

    // incercam conectarea clientului la server
    if(connect(clientFd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Eroare la conectarea clientului la server: ");
        return 2;
    }

    char* phrase = (char*)malloc(sizeof(char) * STR_SIZE);

    printf("Introduceti un text: ");
    fflush(stdout);
    fgets(phrase, STR_SIZE, stdin);

    phrase[strlen(phrase) - 1] = '\0'; // so that we don't include the '\n'

    // incercam trimiterea textului la server prin socket-ul clientului
    if(send(clientFd, phrase, strlen(phrase) * sizeof(char), 0) < 0) {
        perror("Error on sending: ");   
        free(phrase);
        return 3;
    }

    printf("[CLIENT] Sent string to server...\n");

    u_int16_t numberOfSpaces = 0;
    
    // incercam primirea informatiei din partea server-ului
    if(recv(clientFd, &numberOfSpaces, sizeof(numberOfSpaces), 0) < 0) {
        perror("Error on receiving: ");
        free(phrase);
        return 4;
    }

    numberOfSpaces = ntohs(numberOfSpaces);

    printf("[CLIENT] Numarul de spatii din textul '%s' este: %hu\n", phrase, numberOfSpaces);

    close(clientFd);
    free(phrase);

    return 0;
}