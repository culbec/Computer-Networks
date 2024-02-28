// Un client trimite unui server un numar. Serverul va returna clientului sirul divizorilor acestui numar.

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/ip.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

int main() {
    int serverFd, clientFd;
    struct sockaddr_in server, client;

    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[SERVER] Socket creation: ");
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
        printf("[SERVER] Waiting for connection...\n");

        clientFd = accept(serverFd, (struct sockaddr *) &client, &clientSize);
        printf("[SERVER] Client connected!\n");

        uint16_t arraySize = 0;
        recv(clientFd, &arraySize, sizeof(arraySize), MSG_WAITALL);
        arraySize = ntohs(arraySize);

        uint16_t divisors[arraySize];

        uint16_t number;
        recv(clientFd, &number, sizeof(number), MSG_WAITALL);
        number = ntohs(number);

        printf("[SERVER] Calculating the divisors of %hu.\n", number);

        u_int16_t index = 0;

        for(uint16_t d = 1; d * d <= number; d++) {
            if(number % d == 0) {
                if(d == number / d) {
                    divisors[index++] = d;
                } else {
                    divisors[index] = d;
                    divisors[arraySize - index - 1] = number / d;
                    index++;
                }
            }
        }

        printf("[SERVER] Sending divisors...\n");
        send(clientFd, divisors, arraySize * sizeof(u_int16_t), 0);

        close(clientFd);
    }

    close(serverFd);


    return 0;
}