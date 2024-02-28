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
    int clientFd;
    struct sockaddr_in server;

    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[CLIENT] Socket: ");
        exit(EXIT_FAILURE);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;

    if(connect(clientFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[CLIENT] Connect: ");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    printf("[CLIENT] Introduce a string: ");
    fflush(stdout);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0';

    u_int16_t index = 0;
    printf("[CLIENT] Introduce an index: ");
    fflush(stdout);
    scanf("%hu", &index);

    if(index < 0 || index >= strlen(buffer)) {
        printf("[CLIENT] Index not valid.\n");
        exit(EXIT_FAILURE);
    }

    u_int16_t length = 0;
    printf("[CLIENT] Introduce a length: ");
    fflush(stdout);
    scanf("%hu", &length);

    if(length < 0 || length > strlen(buffer)) {
        printf("[CLIENT] Length not valid.\n");
        exit(EXIT_FAILURE);
    }

    if(length + index > strlen(buffer)) {
        printf("[CLIENT] Invalid length_index combination.\n");
        exit(EXIT_FAILURE);
    }

    uint16_t bufferSize = strlen(buffer) + 1;
    send(clientFd, &bufferSize, sizeof(bufferSize), 0);
    send(clientFd, buffer, bufferSize * sizeof(char), 0);
    send(clientFd, &index, sizeof(index), 0);
    send(clientFd, &length, sizeof(length), 0);

    char substring[length + 1];
    recv(clientFd, substring, (length + 1) * sizeof(char), 0);

    printf("[CLIENT] Substring received from server: %s\n", substring);

    exit(EXIT_SUCCESS);
}