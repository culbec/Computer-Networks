// Un client trimite unui server un sir de caractere. Serverul va returna clientului acest sir oglindit (caracterele sirului in ordine inversa).

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/ip.h>
#include <netinet/in.h>

#include <stdio.h>
#include <string.h>

#include <unistd.h>

#define STR_SIZE 100

int main() {
    // NOTE: Server can work both as client and as server.
    int serverFd, clientFd;
    struct sockaddr_in server, client;

    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[SERVER] Error on server socket creation: ");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[SERVER] Error on server binding: ");
        return 2;
    }

    if(listen(serverFd, 1) < 0) {
        perror("[SERVER] Error on listen: ");
        return 3;
    }

    int clientSize = sizeof(client);
    memset(&client, 0, sizeof(client));

    char buffer[STR_SIZE] = "\0";

    while(1) {
        printf("[SERVER] Waiting connection...\n");

        clientFd = accept(serverFd, (struct sockaddr*) &client, &clientSize);

        printf("[SERVER] Client connected!\n");

        recv(clientFd, buffer, STR_SIZE * sizeof(char), 0);

        printf("[SERVER] Received from client: %s\n", buffer);

        for(int i = 0, j = strlen(buffer) - 1; i < j; i++, j--) {
            char temp = buffer[i];
            buffer[i] = buffer[j];
            buffer[j] = temp;
        }

        printf("[SERVER] Sending to client: %s\n", buffer);
        send(clientFd, buffer, strlen(buffer) * sizeof(char), 0);
        close(clientFd);
    }

    close(serverFd);

    return 0;
}