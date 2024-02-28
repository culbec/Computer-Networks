//
// Created by culbec on 10/10/23.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    /* socket declaration */
    int c; // client descriptor
    struct sockaddr_in server; // server-ul la care se va conecta

    // crearea socket pentru client
    c = socket(AF_INET, SOCK_STREAM, 0);
    // verificam daca s-a creat socket-ul pentru client
    if (c == -1) {
        perror("Nu s-a putut crea socket-ul: ");
        return 1;
    }

    // instantiem server-ul
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    // incercam sa conectam clientul la server
    if (connect(c, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Nu s-a putut conecta la server: ");
        return 2;
    }

    // restul problemei
    // trimitem un sir de numere catre server si vom primi inapoi suma lor

    uint16_t arraySize = 0;

    do {
        printf("[CLIENT] Dati numarul de numere (>0): ");
        scanf("%hu", &arraySize);
    } while(arraySize <= 0);

    uint16_t* array = (uint16_t*)malloc(arraySize * sizeof(uint16_t));

    for(int i = 0; i < arraySize; i++) {
        printf("[CLIENT] Dati numar: ");
        scanf("%hu", &array[i]);
    }

    // trimitem catre server numarul de numere pe care trebuie sa le citeasca
    arraySize = htons(arraySize);
    send(c, &arraySize, sizeof(arraySize), 0);

    arraySize = ntohs(arraySize);
    // trimitem numerele
    for(int i = 0; i < arraySize; i++) {
        array[i] = htons(array[i]);
        send(c, &array[i], sizeof(array[i]), 0);
    }

    uint16_t suma;
    // primim suma de la server
    recv(c, &suma, sizeof(suma), 0);
    printf("[CLIENT] Am primit suma: %hu\n", ntohs(suma));

    // inchidem conexiunea clientului

    close(c);

    // free
    free(array);

    return 0;
}
