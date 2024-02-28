// Server template under UDP protocol

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
    int serverFd;
    struct sockaddr_in server, client;

    // initialization of the client file descriptor
    if((serverFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket: ");
        exit(EXIT_FAILURE);
    }

    // initialization of the server struct
    memset(&server, 0, sizeof(server)); // erasing data that could overlap the server
    server.sin_family = AF_INET; // family of the server
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // address of choice
    server.sin_port = htons(1234); // port of choice

    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server)) == -1) {
        perror("bind: ");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    // receiving data from the client
    char buf[1024] = "\0";
    socklen_t clientSize = sizeof(client);
    recvfrom(serverFd, buf, sizeof(buf), 0, (struct sockaddr*) &client, &clientSize);
    printf("received data from %s:%hu", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    printf("%s\n", buf);

    // receiving data from the server
    char bufSend[1024] = "world";
    sendto(serverFd, bufSend, sizeof(bufSend), 0, (struct sockaddr *) &client, sizeof(client));

    close(serverFd);
    exit(EXIT_SUCCESS);
}