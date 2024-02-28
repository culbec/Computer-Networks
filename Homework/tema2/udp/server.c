#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#define SIZE 1024
#define PORT 1234
#define ADDRESS "127.0.0.1"

int main() {
    int serverFd, clientFd;
    struct sockaddr_in server, client;

    serverFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverFd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    server.sin_family = AF_INET;

    int retValue = bind(serverFd, (struct sockaddr*) &server, sizeof(server));
    if(retValue == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    char buf1[SIZE] = "\0", buf2[SIZE] = "\0";

    socklen_t clientSize = sizeof(client);
    memset(&client, 0, sizeof(client));

    printf("[SERVER] Waiting for connection...\n");

    // receiving from the client
    // in this step we also ensure that the client sends data to the server
    // and is initialized
    u_int16_t buf1Size = 0;
    recvfrom(serverFd, buf1, sizeof(buf1), MSG_WAITALL, (struct sockaddr *) &client, &clientSize);
    printf("[SERVER] Received packet from %s:%d\n", inet_ntoa(client.sin_addr), client.sin_port);
    printf("[SERVER] %s\n", buf1);

    // receiving from the same client
    // but now the client socket is initialized
    recvfrom(clientFd, buf2, sizeof(buf2), 0, (struct sockaddr *) &client, &clientSize);
    printf("[SERVER] Received packet from %s:%d\n", inet_ntoa(client.sin_addr), htons(client.sin_port));
    printf("[SERVER] %s\n", buf2);

    u_int16_t nr1, nr2;
    nr1 = atoi(buf1);
    nr2 = atoi(buf2);

    u_int16_t sum = nr1 + nr2;
    printf("[SERVER] %hu + %hu = %hu. Sending to client...\n", nr1, nr2, sum);
    sum = htons(sum);
    sendto(clientFd, &sum, sizeof(sum), 0, (struct sockaddr *) &clientSize, clientSize);

    close(clientFd);
    
    close(serverFd);

    exit(EXIT_SUCCESS);
}