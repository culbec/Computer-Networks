#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 1234

const char* BOMBARDAT = "DA\n";
const char* NEBOMBARDAT = "NU\n";

void razboi(int clientFd) {
    printf("[SERVER] Waiting size...\n");

    // receiving the size of the matrix as a string
    char nString[2] = "\0";
    recv(clientFd, nString, sizeof(nString), 0);
    nString[1] = '\0';

    // converting the n to a number
    int n = atoi(nString);
    printf("[CLIENT] Received %u\n", n);

    // reserving memory for the matrix
    int** teren = (int**)malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++) {
        teren[i] = (int*)malloc(n * sizeof(int));
        memset(teren[i], 0, sizeof(teren[i]));
    }

    // number of mistakes and unbombed planes
    int greseli = 0, numarAvioaneNebombardate = n;

    // generating n pairs of (line, column) that will be initialized to 1
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        int line = rand() % n;
        int col = rand() % n;

        teren[line][col] = 1;
    }

    // telling the client that the game started.
    char start[] = "Jocul a inceput!\n";
    send(clientFd, start, sizeof(start), 0);

    // the game will continue until there are 5 mistakes or no unbombed planes
    while(greseli < 5 && numarAvioaneNebombardate > 0) {
        // receiving position from the client
        char lineBuf[10], colBuf[10];
        recv(clientFd, &lineBuf, sizeof(lineBuf), MSG_WAITALL);
        recv(clientFd, &colBuf, sizeof(colBuf), MSG_WAITALL);

        int line = atoi(lineBuf);
        int col = atoi(colBuf);

        printf("[SERVER] Pozitie primita de la client: (%u, %u). Verificare...\n", line, col);

        // if the line/column is not in [4, 9] -> error
        if((line < 4 || line > 9) || (col < 4 || col > 9)) {
            greseli++;
            int greseliBuf = htonl(greseli);
            int numarAvioaneBuf = htonl(numarAvioaneNebombardate);

            send(clientFd, NEBOMBARDAT, sizeof(NEBOMBARDAT), 0);
            send(clientFd, &greseliBuf, sizeof(greseli), 0);
            send(clientFd, &numarAvioaneBuf, sizeof(numarAvioaneBuf), 0);

            continue;
        }

        if (teren[line][col] == 1) {
            teren[line][col] = 0;
            numarAvioaneNebombardate--;
            // sending confirmation of bombardment
            int greseliBuf = htonl(greseli);
            int numarAvioaneBuf = htonl(numarAvioaneBuf);

            send(clientFd, BOMBARDAT, sizeof(BOMBARDAT), 0);
            send(clientFd, &greseliBuf, sizeof(greseli), 0);
            send(clientFd, &numarAvioaneBuf, sizeof(numarAvioaneBuf), 0);
        } else {
            // updating the errors
            greseli++;
            // sending failure of bombardment
            int greseliBuf = htonl(greseli);
            int numarAvioaneBuf = htonl(numarAvioaneBuf);

            send(clientFd, NEBOMBARDAT, sizeof(NEBOMBARDAT), 0);
            send(clientFd, &greseliBuf, sizeof(greseli), 0);
            send(clientFd, &numarAvioaneBuf, sizeof(numarAvioaneBuf), 0);
        }
    }

    if (greseli == 5) {
        // trimitere game lost
        char buf[] = "Joc pierdut!\n";
        int greseliBuf = htonl(greseli);
        int numarAvioaneBuf = htonl(numarAvioaneNebombardate);

        send(clientFd, buf, sizeof(buf), 0);
        send(clientFd, &greseliBuf, sizeof(greseliBuf), 0);
        send(clientFd, &numarAvioaneBuf, sizeof(numarAvioaneBuf), 0);
    } else if (numarAvioaneNebombardate == 0) {
        // trimitere game won
        char buf[] = "Joc castigat!\n";
        int greseliBuf = htonl(greseli);
        int numarAvioaneBuf = htonl(numarAvioaneNebombardate);

        send(clientFd, buf, sizeof(buf), 0);
        send(clientFd, &greseliBuf, sizeof(greseliBuf), 0);
        send(clientFd, &numarAvioaneBuf, sizeof(numarAvioaneBuf), 0);
    }

    for(int i = 0; i < n; i++) {
        free(teren[i]);
    }
    free(teren);
}

int main() {
    int serverFd;
    struct sockaddr_in server;

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket: ");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server.sin_port = htons(SERVER_PORT);

    if (bind(serverFd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("bind: ");
        goto exit_failure;
    }

    if (listen(serverFd, 5) < 0) {
        perror("listen: ");
        goto exit_failure;        
    }

    printf("[SERVER] Awaiting connection...\n");

    while(1) {
        int clientFd;
        struct sockaddr_in client;

        memset(&client, 0, sizeof(client));
        socklen_t clientSize = sizeof(client);   

        if((clientFd = accept(serverFd, (struct sockaddr*) &client, &clientSize)) < 0) {
            perror("accept: ");
            continue;
        }

        printf("\n[SERVER] Client connected: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        int pid = fork();

        if (pid == -1) {
            perror("fork: ");
            continue;
        } else if (pid == 0) {
            printf("[SERVER] Serving client.\n");
            razboi(clientFd);
            printf("[SERVER] Done!\n");
            close(clientFd);
        }
    }

    close(serverFd);
    exit(EXIT_SUCCESS);

    exit_failure:
        close(serverFd);
        exit(EXIT_FAILURE);
}