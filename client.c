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
#include <poll.h>

#define PORT 8080

int main(int argc, char const* argv[]) {

    struct sockaddr_in server;
    int socket_fd;
    // char r_buff[100] = "", s_buff[100] = "";

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = PORT;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(socket_fd, (struct sockaddr *) &server, sizeof server);

    struct pollfd poll_fds[2] = {
        {
            STDIN_FILENO,
            POLLIN,
            0
        },
        {
            socket_fd,
            POLLIN,
            0
        }
    };

    char buffer[100] = "";

    while (1) {
        
        poll(poll_fds, 2, 60000);

        if ( poll_fds[0].revents & POLLIN ) {
            int n = read(STDIN_FILENO, buffer, sizeof buffer);

            printf("Message: [%s]\nSize: %d", buffer, n);

            send(socket_fd, buffer, n, 0);
        }
        else if ( poll_fds[1].revents & POLLIN ) {
            recv(socket_fd, buffer, sizeof buffer, 0);

            if (strcmp(buffer, "exit\n") == 0) {
                break;
            }

            printf("[server] %s", buffer);
        }

    }

    // while (1) {
    //     printf("\nclient: ");
    //     fgets(s_buff, sizeof s_buff, stdin);
    //     send(socket_fd, s_buff, sizeof s_buff, 0);
        
    //     if ( strcmp(s_buff, "end") == 0 ) {
    //         break;
    //     }

    //     recv(socket_fd, r_buff, sizeof r_buff, 0);
    //     printf("[server] %s", r_buff);

    //     if ( strcmp(r_buff, "end") == 0 ) {
    //         break;
    //     }

    //     printf("\n");
    // }

    close(socket_fd);

    return 0;

}