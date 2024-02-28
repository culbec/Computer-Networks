/*
    - pp. ca primim de la client un nume de calculator cu addr: 1789567989;
    - trebuie translatat intr-un sir de 4 octeti separati prin punct;
    
        x.y.z.t -> little endian -> t.z.y.x
*/

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// using union
// data is overlapped in the same memory section

union {
    u_int32_t ip;
    u_int8_t byte[4];
} addrBytes;

void my_inet_ntoa(uint32_t host) {
    addrBytes.ip = host; // metoda 1
    printf("Converted using union: %hhu.%hhu.%hhu.%hhu\n", 
    addrBytes.byte[0], addrBytes.byte[1], addrBytes.byte[2], addrBytes.byte[3]);

    uint8_t *bytePtr = (u_int8_t*) &host; // metoda 2
    printf("Converted using pointers: %hhu.%hhu.%hhu.%hhu\n", 
    *(bytePtr), *(bytePtr + sizeof(*bytePtr)), *(bytePtr + 2*sizeof(*bytePtr)), *(bytePtr + 3*sizeof(*bytePtr)));

    char addr[16] = "\0";
    sprintf(addr, "%hhu.%hhu.%hhu.%hhu", addrBytes.byte[0], addrBytes.byte[1], addrBytes.byte[2], addrBytes.byte[3]);
    in_addr_t addrConverted = inet_addr(addr);
    printf("Test? %u =? %u\n", host, addrConverted);

    printf("\n");
}

void my_htonl(u_int32_t number) {
    u_int32_t rez = 0;

    u_int8_t r1 = number % 256, r2 = *((u_int8_t*)&number);
    if (r1 == r2) {
        /*

            number: |x|y|z|t| -> nu stim arhitectura -> |t|z|y|x|

            1. |x|y|z|t| -> |_|_|_|x|
            2. |x|y|z|t| -> |_|_|y|_|
            3. |x|y|z|t| -> |_|z|_|_|
            4. |x|y|z|t| -> |t|_|_|_|

            => |t|z|y|x|
        */
        rez = (number >> 24) | ((number & 0x00FF0000) >> 8) | ((number & 0x0000FF00) << 8) | (number << 24);
        printf("Converted: %u\n", rez);
    }  else {
        printf("No conversion!\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if(argc == 1) {
        printf("Usage: ./trans number1 number2 ...\n");
        return 1;
    }

    for(int i = 1; i < argc; ++i) {
        u_int32_t number = atoi(argv[i]);
        my_inet_ntoa(number);
        my_htonl(number);
    }

    return 0;
}