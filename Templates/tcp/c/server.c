// Template Server under TCP protocol

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

void deserveClient(int clientFd) {
    // method for deserving a client
}

int main() {
    /*
        Declaration of:
            - file descriptor of the server socket;
            - socket struct of the server;
    */
   int serverFd;
   struct sockaddr_in server;

   // initialization of the socket file descriptor
   if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket: ");
        exit(EXIT_FAILURE);
   }

   // initialization of the socket struct
   memset(&server, 0, sizeof(server)); // erasing data that could overlap the server
   server.sin_family = AF_INET; // server family
   server.sin_addr.s_addr = inet_addr("127.0.0.1"); // address of choice
   server.sin_port = htons(1234); // port of choice

   // binding the socket to the server
    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server)) == -1) {
        perror("bind: ");
        exit(EXIT_FAILURE);
    }

    // trying to listen on the server
    if(listen(serverFd, 5) == -1) {
        perror("listen: ");
        exit(EXIT_FAILURE);
    }

    // from this point, the server is fully configured and we can start accepting clients

    // declaration of the client socket struct
    struct sockaddr_in client;
    socklen_t clientSize = sizeof(client); // size of the client socket struct
    memset(&client, 0, sizeof(client)); // erasing data that could overlap the client

    while(1) {
        // from here we can accept clients
        printf("[SERVER] Awaiting connection...\n");

        // accepting a client
        int clientFd = accept(serverFd, (struct sockaddr*) &client, &clientSize);

        if(clientFd == -1) {
            perror("accept: ");
            continue;
        }

        // if we want to deserve multiple clients, we may create a individual process for each
        // client that we accept connection from
        int pid = fork();

        if (pid == -1) {
            perror("fork: ");
            close(clientFd);
            continue;
        } else if (pid == 0) {
            deserveClient(clientFd);
            close(clientFd);
        }

        // otherwise, we can deserve the client right here
    }

    close(serverFd); // closing the server file descriptor
    exit(EXIT_SUCCESS);
}