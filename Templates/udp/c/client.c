// Client template under UDP protocol

#include <sys/types.h>
#include <sys/socket.h>

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

    // initialization of the client file descriptor
    if((clientFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket: ");
        exit(EXIT_FAILURE);
    }

    // initialization of the server struct
    memset(&server, 0, sizeof(server)); // erasing data that could overlap the server
    server.sin_family = AF_INET; // family of the server
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // address of choice
    server.sin_port = htons(1234); // port of choice

    // sending data to the server
    char buf[] = "hello";
    sendto(clientFd, buf, sizeof(buf), 0, (struct sockaddr*) &server, sizeof(server));
    printf("sent data to %s:%hu\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    // receiving data from the server
    char bufRecv[1024] = "\0";
    socklen_t serverSize = sizeof(server);
    recvfrom(clientFd, bufRecv, sizeof(bufRecv), 0, (struct sockaddr *) &server, &serverSize);
    printf("%s\n", bufRecv);

    close(clientFd);
    exit(EXIT_SUCCESS);
}