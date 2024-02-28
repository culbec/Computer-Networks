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
    int clientFd;
    struct sockaddr_in server;

    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[CLIENT] Socket creation: ");
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

    u_int16_t number = 0;
    printf("[CLIENT] Introduceti un numar: ");
    fflush(stdout);
    scanf("%hu", &number);

    u_int16_t copyNumber = number;
    u_int16_t divisor = 2, numberOfDivisors = 1;

    while(copyNumber != 1) {
        uint16_t numberOfDivisons = 0;

        while(copyNumber % divisor == 0) {
            copyNumber /= divisor;
            numberOfDivisons++;
        }

        numberOfDivisors *= (numberOfDivisons + 1);
        divisor++;
    }

    numberOfDivisors = htons(numberOfDivisors);
    send(clientFd, &numberOfDivisors, sizeof(numberOfDivisors), 0);

    number = htons(number);
    send(clientFd, &number, sizeof(number), 0);

    numberOfDivisors = ntohs(numberOfDivisors);
    uint16_t divisors[numberOfDivisors];

    recv(clientFd, divisors, numberOfDivisors * sizeof(u_int16_t), 0);

    for(uint16_t i = 0; i < numberOfDivisors; i++) {
        printf("%hu ", divisors[i]);
    }
    fflush(stdout);
    printf("\n");

    close(clientFd);

    return 0;
}