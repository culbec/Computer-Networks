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
    int clientFd;
    struct sockaddr_in server;

    clientFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientFd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    server.sin_family = AF_INET;

    char buf1[SIZE] = "\0", buf2[SIZE] = "\0";

    printf("[CLIENT] Nr1 = ");
    fflush(stdout);
    fgets(buf1, sizeof(buf1), stdin);

    printf("[CLIENT] Nr2 = ");
    fflush(stdout);
    fgets(buf2, sizeof(buf2), stdin);
    sendto(clientFd, buf1, sizeof(buf1), 0, (struct sockaddr *) &server, sizeof(server));
    sendto(clientFd, buf2, sizeof(buf2), 0, (struct sockaddr *) &server, sizeof(server));

    printf("[CLIENT] Sending to the server...\n");

    u_int16_t sum = 0;
    socklen_t serverLen = sizeof(server);
    recvfrom(clientFd, &sum, sizeof(sum), 0, (struct sockaddr *) &server, &serverLen);

    close(clientFd);
}