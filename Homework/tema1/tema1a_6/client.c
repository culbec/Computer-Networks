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
    int clientFd;
    struct sockaddr_in server;

    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[CLIENT] Socket: ");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if(connect(clientFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("[CLIENT] Connect: ");
        exit(EXIT_FAILURE);
    }

    char character = '\0';
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    printf("[CLIENT] Introduce a string: ");
    fflush(stdout);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0';

    printf("[CLIENT] Sending the string to the server...\n");

    u_int16_t bufferSize = strlen(buffer);
    send(clientFd, &bufferSize, sizeof(bufferSize), 0);
    send(clientFd, buffer, (bufferSize + 1) * sizeof(char), 0);

    printf("[CLIENT] Introduce a character: ");
    fflush(stdout);
    scanf("%c", &character);

    printf("[CLIENT] Sending the character to the server...\n");
    send(clientFd, &character, sizeof(character), 0);

    u_int16_t occurences = 0;
    recv(clientFd, &occurences, sizeof(occurences), 0);

    u_int16_t positions[occurences];
    memset(positions, 0, sizeof(positions));
    recv(clientFd, positions, occurences * sizeof(u_int16_t), 0);

    printf("[CLIENT] The positions of the character in the string: ");
    for(u_int16_t i = 0; i < occurences; i++) {
        printf("%hu ", positions[i]);
    }
    printf("\n");

    close(clientFd);


    return 0;
}