// Template Client under TCP protocol

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
    /*
        Declaration of:
            - file descriptor of the client socket;
            - socket struct of the server;
    */
    int clientFd;
    struct sockaddr_in server;

    // initialization of the file descriptor of the client socket
    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket: ");
        exit(EXIT_FAILURE);
    }

    // initialization of the socket struct of the server
    memset(&server, 0, sizeof(server)); // erasing data that could overlap the server
    server.sin_family = AF_INET; // family of the sever
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // address of choice
    server.sin_port = htons(1234); // port of choice

    // trying to connect to the server
    if(connect(clientFd, (struct sockaddr*) &server, sizeof(server)) == -1) {
        perror("connect: ");
        close(clientFd);
        exit(EXIT_FAILURE);
    }

    // here we can send/recv data to/from server, as we are connected to it

    close(clientFd);
    exit(EXIT_SUCCESS);
}