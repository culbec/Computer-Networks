// Server concurent
// Un client trimite unui server TCP un sir de caractere. Serverul trimite

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

void clientOp(int clientFd) {
    u_int16_t strSize = 0;

    int bytesRead = recv(clientFd, &strSize, sizeof(strSize), MSG_WAITALL);
    if(bytesRead < 0) {
        perror("recv");
        return;
    }

    strSize = ntohs(strSize);
    printf("[SERVER] Received string size from the client: %hu.\n", strSize);

    char str[strSize + 1];
    memset(str, 0, sizeof(str));

    bytesRead = recv(clientFd, str, strSize * sizeof(char), MSG_WAITALL);
    if(bytesRead < 0) {
        perror("recv");
        return;
    }

    printf("[SERVER] Received string from client: %s\n", str);

    for(int i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }

    printf("[SERVER] Sending to client: %s\n\n", str);

    strSize = htons(strSize);
    int bytesSent = send(clientFd, &strSize, sizeof(strSize), 0);
    if(bytesSent < 0) {
        perror("send");
        return;
    }

    bytesSent = send(clientFd, str, ntohs(strSize) * sizeof(char), 0);
    if(bytesSent < 0) {
        perror("send");
        return;
    }
}

int main() {
    int serverFd, clientFd;
    struct sockaddr_in server, client;

    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[SERVER] Socket: ");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;

    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[SERVER] Bind: ");
        exit(EXIT_FAILURE);
    }

    if(listen(serverFd, 5) < 0) {
        perror("[SERVER] Listen: ");
        exit(EXIT_FAILURE);
    }

    int clientSize = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1) {
        clientFd = accept(serverFd, (struct sockaddr *) &client, &clientSize);
        printf("[SERVER] Client connected from address: %s\n", inet_ntoa(client.sin_addr));

        if(fork() == 0) {
            clientOp(clientFd);
            return 0;
        }
    }

    close(serverFd);

    return 0;
}