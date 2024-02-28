// Un client trimite unui server doua siruri de numere. Serverul va returna clientului sirul de numere care se regaseste in primul sir dar nu se regasesc in al doilea.

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

    int arr1Size, arr2Size;

    do {
        printf("[CLIENT] Arr1 size (> 0, <= 100): ");
        fflush(stdout);
        scanf("%d", &arr1Size);
    } while(arr1Size > 100 || arr1Size < 0);

    do {
        printf("[CLIENT] Arr2 size (> 0, <= 100): ");
        fflush(stdout);
        scanf("%d", &arr2Size);
    } while(arr2Size > 100 || arr2Size < 0);

    int arr1[arr1Size];
    int arr2[arr2Size];

    printf("[CLIENT] Read arr1 numbers: ");
    fflush(stdout);
    for(int i = 0; i < arr1Size; i++) {
        scanf("%d", &arr1[i]);
    }

    printf("[CLIENT] Read arr2 numbers: ");
    fflush(stdout);
    for(int i = 0; i < arr2Size; i++) {
        scanf("%d", &arr2[i]);
    }

    // TODO: Cum facem sa trimitem date care nu sunt considerate fara semn in retea?
    send(clientFd, &arr1Size, sizeof(int), 0);
    send(clientFd, &arr2Size, sizeof(int), 0);

    send(clientFd, arr1, arr1Size * sizeof(int), 0);
    send(clientFd, arr2, arr2Size * sizeof(int), 0);

    int arrSize;
    recv(clientFd, &arrSize, sizeof(int), 0);

    int arr[arrSize];
    recv(clientFd, arr, arrSize * sizeof(int), 0);

    printf("[CLIENT] Arr contents: ");
    for(int i = 0; i < arrSize; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");
    close(clientFd);

    return 0;
}