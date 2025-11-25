// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for close()
#include <arpa/inet.h>  // for inet_addr, sockaddr_in

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hello";
    
    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    // 2. Define server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "192.168.0.127", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        return -1;
    }

    // 3. Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // 4. Send message
    send(sock, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);

    // 5. Close socket
    close(sock);

    return 0;
}

