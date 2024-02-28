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
    int serverFd, clientFd;
    struct sockaddr_in server, client;

    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[SERVER] Error on socket creation: ");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[SERVER] Error on binding the server: ");
        exit(EXIT_FAILURE);
    }

    if(listen(serverFd, 1) < 0) {
        perror("[SERVER] Error on listen: ");
        exit(EXIT_FAILURE);
    }

    int clientSize = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1) {
        printf("[SERVER] Awaiting connection...\n");
        
        clientFd = accept(serverFd, (struct sockaddr*) &client, &clientSize);
        printf("[SERVER] Client connected!\n");

        int sir1Size = 0, sir2Size = 0;

        recv(clientFd, &sir1Size, sizeof(sir1Size), 0);
        recv(clientFd, &sir2Size, sizeof(sir2Size), 0);

        char sir1[sir1Size + 1], sir2[sir2Size + 1];
        sir1[sir1Size] = '\0'; sir2[sir2Size] = '\0';

        recv(clientFd, sir1, sir1Size * sizeof(char), 0);
        recv(clientFd, sir2, sir2Size * sizeof(char), 0);

        char sirMerged[strlen(sir1) + strlen(sir2) + 1];
        sirMerged[strlen(sir1) + strlen(sir2)] = '\0';

        int i = 0, j = 0, k = 0;

        while(i < strlen(sir1) && j < strlen(sir2)) {
            if(sir1[i] < sir2[j]) {
                sirMerged[k++] = sir1[i++];
            } else {
                sirMerged[k++] = sir2[j++];
            }
        }

        while(i < strlen(sir1)) {
            sirMerged[k++] = sir1[i++];
        }

        while(j < strlen(sir2)) {
            sirMerged[k++] = sir2[j++];
        }

        printf("[SERVER] Sending '%s' to client.\n", sirMerged);
        send(clientFd, sirMerged, strlen(sirMerged) * sizeof(char), 0);

        close(clientFd);
    }
    
    close(serverFd);

    return 0;
}