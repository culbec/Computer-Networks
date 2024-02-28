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
    int clientFd, serverFd;
    struct sockaddr_in server, client;

    // incercam crearea socket-ului pentru server
    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[SERVER] Eroare la crearea socket-ului pentru server: ");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234); // port-ul pe care va lucra server-ul
    server.sin_family = AF_INET; // domeniul din care face parte server-ul
    server.sin_addr.s_addr = INADDR_ANY; // adresa pe care va lucra server-ul

    // incercam sa legam socket-ul server-ului la server-ul propriu sis
    if(bind(serverFd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("[SERVER] Eroare la legare de server: ");
        return 2;
    }

    // incercam sa trecem server-ul in modul 'pasiv'
    // doar va asculta ce au de zis clientii (in cazul nostru clientul)
    if(listen(serverFd, 1) < 0) {
        perror("[SERVER] Eroare la listen: ");
        return 3;
    }

    char* phrase = (char*)malloc(sizeof(char) * STR_SIZE);

    int sizeOfClient = sizeof(client);
    memset(&client, 0, sizeof(client));

    // incepem sa ascultam clienti
    while(1) {
        printf("[SERVER] Waiting connection...\n");

        clientFd = accept(serverFd, (struct sockaddr *) &client, &sizeOfClient);
        printf("[SERVER] Client connected!\n");
        memset(&client, 0, sizeof(client));

        read(clientFd, phrase, STR_SIZE); // reading the phrase from the client

        printf("[SERVER] String-ul primit: %s\n", phrase);

        char *found = strtok(phrase, " ");
        uint16_t numberOfSpaces = -1;

        while(found != NULL) {
            ++numberOfSpaces;
            found = strtok(NULL, " ");
        }

        printf("[SERVER] Sending %hu to client...\n", numberOfSpaces);
        numberOfSpaces = htons(numberOfSpaces); // trecem informatia in format-ul serverului
        send(clientFd, &numberOfSpaces, sizeof(numberOfSpaces), 0);
        close(clientFd); // s-a terminat deservirea clientului
    }

    return 0;
}