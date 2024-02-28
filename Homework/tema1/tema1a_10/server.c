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
    int serverFd, clientFd;
    struct sockaddr_in server, client;

    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[SERVER] Socket: ");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;

    if(bind(serverFd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("[SERVER] Bind: ");
        exit(EXIT_FAILURE);
    }

    if(listen(serverFd, 1) < 0) {
        perror("[SERVER] Listen: ");
        exit(EXIT_FAILURE);
    }

    memset(&client, 0, sizeof(client));
    int clientSize = sizeof(client);
    
    while(1) {
        printf("[SERVER] Waiting connection...\n");

        clientFd = accept(serverFd, (struct sockaddr *) &client, &clientSize);
        printf("[SERVER] Client connected!\n");

        int str1Size, str2Size;

        recv(clientFd, &str1Size, sizeof(int), MSG_WAITALL);
        printf("[SERVER] Received from client: %d\n", str1Size);

        recv(clientFd, &str2Size, sizeof(int), MSG_WAITALL);
        printf("[SERVER] Received from client: %d\n", str2Size);

        char str1[str1Size], str2[str2Size];
        int apps[128] = {0};

        recv(clientFd, str1, str1Size * sizeof(char), MSG_WAITALL);
        printf("[SERVER] Received from client: %s\n", str1);

        recv(clientFd, str2, str2Size * sizeof(char), MSG_WAITALL);
        printf("[SERVER] Received from client: %s\n", str2);

        int strSize = 0;

        if(str1Size < str2Size)  {
            strSize = str1Size;
        } else {
            strSize = str2Size;
        }

        for(int i = 0; i < strSize; i++) {
            if(str1[i] == str2[i]) {
                apps[str1[i]]++;
            }
        }

        char ch = '\0'; int maxApp = 0 ;
        for(int i = 0; i < 128; i++) {
            if(apps[i] > maxApp) {
                ch = (char) i;
                maxApp = apps[i];
            }
        }

        printf("[Server] Sending to client: %c and %d\n", ch, maxApp);
        send(clientFd, &ch, sizeof(char), 0);
        send(clientFd, &maxApp, sizeof(int), 0);

        close(clientFd);
    }

    close(serverFd);
    return 0;
}