#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>

#define PORT 8080

// links to random shit: 
// https://man7.org/linux/man-pages/man2/socket.2.html


int main(int arc, char const* argv[]) {
    
    // sockaddr_in describes a socket connection
    struct sockaddr_in client, server;
    int socket_fd, n, connection_fd;
    char r_buff[100] = "", s_buff[100] = "";

    // creates the endpoint for communication, returns file descriptor refering to it
    // AF_INET = IPv4 Internet protocol
    // SOCK_STREAM = sequenced two way byte stream
    // 0 means use default protocol
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = 2000;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // have to downcast sockaddr_in to generic sockaddr to pass to most functions. 
    // sockaddr is like a parent class that describes sockets in general afaik

    // bind will actualy connect the socket_fd we made to the server socket
    if ( bind(socket_fd, (struct sockaddr *)&server, sizeof server) == -1 ) {
        err(EXIT_FAILURE, "bind");
    }

    // listen marks the socket to be a socket that will be used to accept incoming connection requests
    if ( listen(socket_fd, 1) == -1 ) {
        err(EXIT_FAILURE, "listen");
    }

    n = sizeof client;
    // accept will (surprisingly) accept the first connection request and create/return a new connected socket ass a file descriptor
    connection_fd = accept(socket_fd, (struct sockaddr *)&client, &n);

    while (1) {
        // recv is to receive a message from a socket
        recv(connection_fd, r_buff, sizeof r_buff, 0);
        printf("\n[client] %s", r_buff);

        if ( strcmp(r_buff, "exit") == 0 ) {
            break;
        }
    
        printf("\nserver: ");
        fgets(s_buff, sizeof s_buff, stdin);
        send(connection_fd, s_buff, sizeof s_buff, 0);

        if ( strcmp(s_buff, "exit") == 0 ) {
            break;
        }

        printf("\n");
    }

    close(connection_fd);
    close(socket_fd);

    return 0;
}