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

int main(int argc, char const* argv[]) {

    struct sockaddr_in server;
    int socket_fd;
    char r_buff[100] = "", s_buff[100] = "";

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = 2000;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(socket_fd, (struct sockaddr *) &server, sizeof server);

    while (1) {
        printf("\nclient: ");
        fgets(s_buff, sizeof s_buff, stdin);
        send(socket_fd, s_buff, sizeof s_buff, 0);
        
        if ( strcmp(s_buff, "end") == 0 ) {
            break;
        }

        recv(socket_fd, r_buff, sizeof r_buff, 0);
        printf("[server] %s", r_buff);

        if ( strcmp(r_buff, "end") == 0 ) {
            break;
        }

        printf("\n");
    }

    close(socket_fd);

    return 0;

}