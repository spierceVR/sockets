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
    const char* hostname = "localhost";
    const uint16_t PORT = 8080;
    init_sockaddr(&myAddr, hostname, PORT);

    // create the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    //open socket
    if (connect(sockfd, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0)
    {
        perror("connection error");
        exit(EXIT_FAILURE);
    }

    // receive message
    char buffer[256];
    if (recv(sockfd, &buffer, sizeof(buffer), 0) < 0){
        perror("recv error");
        exit(EXIT_FAILURE);
    }

    // print message
    printf("Message reeived: %s\n", buffer);

    // close socket
    if (shutdown(sockfd, SHUT_RDWR) < 0) {
        perror("socket close failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}


