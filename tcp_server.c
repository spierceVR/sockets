#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void init_sockaddr(struct sockaddr_in *name, const char *hostname, uint16_t port)
{
    struct hostent *hostinfo;

    name->sin_family = AF_INET;
    name->sin_port = htons(port);
    hostinfo = gethostbyname(hostname);
    if (hostinfo == NULL)
    {
        fprintf(stderr, "Unknown host %s.\n", hostname);
        exit(EXIT_FAILURE);
    }
    name->sin_addr = *(struct in_addr *)hostinfo->h_addr;
}

int main(int argc, char const *argv[])
{
    // create socket address var
    struct sockaddr_in myAddr;
    const char *hostname = "localhost";
    const uint16_t PORT = 8080;
    init_sockaddr(&myAddr, hostname, PORT);

    // create the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to address
    if (bind(sockfd, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    // open socket
    if (listen(sockfd, 5) < 0)
    {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    // accept connection
    int client_socket = accept(sockfd, NULL, NULL);
    if (client_socket < 0 ){
        perror("accept error");
        exit(EXIT_FAILURE);
    } 

    // send message
    const char message[256] = "Hello Server!";
    send(client_socket, message, sizeof(message), 0);


    // close socket
    if (shutdown(sockfd, SHUT_RDWR) < 0)
    {
        perror("socket close failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}