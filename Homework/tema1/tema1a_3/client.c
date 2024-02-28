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
    int clientFd;
    struct sockaddr_in server;

    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[CLIENT] Error on creating client socket: ");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if(connect(clientFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[CLIENT] Error on connecting: ");
        return 2;
    }

    char buffer[STR_SIZE] = "\0";
    
    printf("Introduceti string-ul: ");
    fflush(stdout);
    fgets(buffer, STR_SIZE, stdin);
    buffer[strlen(buffer) - 1] = '\0';

    printf("[CLIENT] Sending '%s' to server.\n", buffer);
    send(clientFd, buffer, strlen(buffer) * sizeof(char), 0);

    recv(clientFd, buffer, strlen(buffer) * sizeof(char), 0);
    printf("[CLIENT] Received '%s' from server.\n", buffer);

    close(clientFd);

    return 0;
}