// Un client trimite unui server doua siruri de numere. Serverul va returna clientului sirul de numere comune celor doua siruri primite.

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/ip.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

int findNumber(int number, int* arr, int arrSize) {
    for(int i = 0; i < arrSize; i++) {
        if (number == arr[i]) {
            return 1;
        }
    }
    return 0;
}

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

        int arr1Size, arr2Size;

        recv(clientFd, &arr1Size, sizeof(arr1Size), MSG_WAITALL);
        recv(clientFd, &arr2Size, sizeof(arr2Size), MSG_WAITALL);

        printf("[SERVER] Received from client: Arr1Size = %d and Arr2Size = %d\n", arr1Size, arr2Size);

        int arr1[arr1Size], arr2[arr2Size];

        recv(clientFd, arr1, arr1Size * sizeof(int), MSG_WAITALL);

        printf("[SERVER] Arr1 contents received from client: ");
        for(int i = 0; i < arr1Size; i++) {
            printf("%d ", arr1[i]);
        }
        printf("\n");

        recv(clientFd, arr2, arr2Size * sizeof(int), MSG_WAITALL);

        printf("[SERVER] Arr2 contents received from client: ");
        for(int i = 0; i < arr2Size; i++) {
            printf("%d ", arr2[i]);
        }
        printf("\n");

        int arrSize = 0, arr[arr1Size];

        for(int i = 0; i < arr1Size; i++) {
            if(findNumber(arr1[i], arr2, arr2Size)) {
                arr[arrSize++] = arr1[i];
            }
        }

        printf("[SERVER] Sending to client: ArrSize = %d\n", arrSize);
        send(clientFd, &arrSize, sizeof(int), 0);

        printf("[SERVER] Arr contents sending to client: ");
        for(int i = 0; i < arrSize; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        send(clientFd, arr, arrSize * sizeof(int), 0);

        close(clientFd);
    }

    close(serverFd);
    return 0;
}