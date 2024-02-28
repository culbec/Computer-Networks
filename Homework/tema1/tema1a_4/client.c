// Un client trimite unui server doua siruri de caractere ordonate. Serverul va interclasa cele doua siruri si va returna clientului sirul rezultat interclasat.

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/ip.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

int main() {
    int clientFd;
    struct sockaddr_in server;

    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[CLIENT] Error on socket creation: ");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if(connect(clientFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[CLIENT] Error on connecting to the server: ");
        exit(EXIT_FAILURE);
    }

    char sir1[] = "AGMRagmr";
    char sir2[] = "BHNSbhns";

    printf("[CLIENT] Sending to server '%s' and '%s'\n", sir1, sir2);

    int sir1Size = strlen(sir1), sir2Size = strlen(sir2);
    send(clientFd, &sir1Size, sizeof(sir1Size), 0);
    send(clientFd, &sir2Size, sizeof(sir2Size), 0);
    send(clientFd, sir1, strlen(sir1) * sizeof(char), 0);
    send(clientFd, sir2, strlen(sir2) * sizeof(char), 0);

    char sirMerged[sir1Size + sir2Size + 1];

    recv(clientFd, sirMerged, (sir1Size + sir2Size + 1) * sizeof(char), 0);
    printf("[CLIENT] Received from server: %s\n", sirMerged);

    close(clientFd);

    exit(EXIT_SUCCESS);
}