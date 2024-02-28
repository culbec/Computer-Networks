// Un client trimite unui server un sir de caractere si doua numere (fie acestea s, i, l). Serverul va returna clientului subsirul de lungime l a lui s care incepe la pozitia i.
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

int main() {
    int serverFd, clientFd;
    struct sockaddr_in server, client;

    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[SERVER] Socket: ");
        exit(EXIT_FAILURE);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;

    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[SERVER] Bind: ");
        exit(EXIT_FAILURE);
    }

    if(listen(serverFd, 1) < 0) {
        perror("[SERVER] Listen: ");
        exit(EXIT_FAILURE);
    }

    int clientSize = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1) {
        printf("[SERVER] Waiting connection...\n");
        
        clientFd = accept(serverFd, (struct sockaddr*) &client, &clientSize);
        printf("[SERVER] Client connected! Address: %s\n", inet_ntoa(client.sin_addr));

        u_int16_t bufferSize = 0;
        recv(clientFd, &bufferSize, sizeof(bufferSize), MSG_WAITALL);

        char buffer[bufferSize];
        recv(clientFd, buffer, bufferSize * sizeof(char), MSG_WAITALL);

        u_int16_t index = 0;
        recv(clientFd, &index, sizeof(index), MSG_WAITALL);

        u_int16_t length = 0;
        recv(clientFd, &length, sizeof(length), MSG_WAITALL);

        char substring[length + 1];
        substring[length] = '\0';
        strncpy(substring, buffer + index, length);

        printf("[SERVER] Sending '%s' to client.\n", substring);
        send(clientFd, substring, (length + 1) * sizeof(char), 0);

        close(clientFd);        
    }
    close(serverFd);

    exit(EXIT_SUCCESS);
}