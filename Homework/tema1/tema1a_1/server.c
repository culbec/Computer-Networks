//
// Created by culbec on 10/10/23.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int s; // declaratia  fd server-ului
    struct sockaddr_in server, client; // serverele propriu size
    int c, l; // declaratia fd server si lungimea mesajului

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Nu s-a putut crea socket-ul: ");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    // legam server-ul de ip-ul si port-urile setate

    if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Eroare la bind: ");
        return 3;
    }

    // ascultam clientul
    listen(s, 1);

    l = sizeof(client);
    memset(&client, 0, sizeof(client));

    uint16_t arraySize, numar, suma = 0;

    c = accept(s, (struct sockaddr *) &client, &l);
    printf("[SERVER] Client connected!\n");

    // primim numerele de la client
    recv(c, &arraySize, sizeof(arraySize), 0);
    arraySize = ntohs(arraySize);
    for (int i = 0; i < arraySize; i++) {
        recv(c, &numar, sizeof(numar), MSG_WAITALL);
        numar = ntohs(numar);
        suma += numar;
    }

    // trimitem suma inapoi
    printf("[SERVER] Trimit suma: %hu\n", suma);
    suma = htons(suma);
    send(c, &suma, sizeof(suma), 0);
    close(c);
    close(s);

    return 0;

}
