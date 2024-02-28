// Un client trimite unui server un sir de caractere si un caracter. Serverul va returna clientului toate pozitiile pe care caracterul primit se regaseste in sir.

#include <sys/socket.h>
#include <sys/types.h>

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
    server.sin_port = htons(1234);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[SERVER] Bind: ");
        exit(EXIT_FAILURE);
    }

    if(listen(serverFd, 1) < 0) {
        perror("[SERVER] Listen: ");
        exit(EXIT_FAILURE);
    }

    int clientSize = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1) {
        printf("[SERVER] Waiting client connection...\n");

        clientFd = accept(serverFd, (struct sockaddr*) &client, &clientSize);
        printf("[SERVER] Client connected!\n");

        uint16_t strSize = 0;
        recv(clientFd, &strSize, sizeof(strSize), MSG_WAITALL);

        char str[strSize + 1];
        memset(str, 0, (strSize + 1) * sizeof(char));
        recv(clientFd, str, (strSize + 1) * sizeof(char), MSG_WAITALL);
        printf("[SERVER] String received from client: %s\n", str);

        char character = '\0';
        recv(clientFd, &character, sizeof(char), MSG_WAITALL);
        printf("[SERVER] Character received from client: %c\n", character);
        
        u_int16_t occurences = 0; char* foundPos = strchr(str, character);

        while(foundPos != NULL) {
            occurences++;
            foundPos = strchr(str + (foundPos - str) + 1, character);
        }

        send(clientFd, &occurences, sizeof(occurences), 0);
    
        u_int16_t positions[occurences];
        memset(positions, 0, occurences * sizeof(u_int16_t));
        uint16_t indexArr = 0, indexCh = 0;
        foundPos = strchr(str, character);

        while(foundPos != NULL) {
            indexCh = foundPos - str;
            positions[indexArr++] = indexCh;
            foundPos = strchr(str + (foundPos - str) + 1, character);
        }

        send(clientFd, positions, occurences * sizeof(uint16_t), 0);

        close(clientFd);
    }

    close(serverFd);
    return 0;
}