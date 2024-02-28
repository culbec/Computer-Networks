// Un client trimite unui server doua siruri de caractere. Serverul ii raspunde clientului cu caracterul care se regaseste de cele mai multe ori pe pozitii identice in cele doua siruri si cu numarul de aparitii ale acestui caracter.

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/ip.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

int main() {
    int clientFd;
    struct sockaddr_in server;

    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[CLIENT] Socket: ");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;

    if(connect(clientFd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("[CLIENT] Connect: ");
        exit(EXIT_FAILURE);
    }

    char str1[100], str2[100];

    printf("[CLIENT] Read the first string: ");
    fflush(stdout);
    fgets(str1, sizeof(str1), stdin);
    str1[strlen(str1) - 1] = '\0';

    printf("[CLIENT] Read the second string: ");
    fflush(stdout);
    fgets(str2, sizeof(str2), stdin);
    str2[strlen(str2) - 1] = '\0';

    int str1Size = strlen(str1) + 1;
    int str2Size = strlen(str2) + 1;
    send(clientFd, &str1Size, sizeof(int), 0);
    send(clientFd, &str2Size, sizeof(int), 0);
    send(clientFd, str1, str1Size * sizeof(char), 0);
    send(clientFd, str2, str2Size * sizeof(char), 0);

    char ch; int maxApp;
    recv(clientFd, &ch, sizeof(char), 0);
    recv(clientFd, &maxApp, sizeof(int), 0);

    printf("[CLIENT] Received from server: %c and %d\n", ch, maxApp);

    return 0;
}