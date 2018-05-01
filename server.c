// custom IRC server
// created by Gezim Saciri, Ben Leipert, Josh Blaz

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MYPORT "54001"
#define BACKLOG 10

/*
establish socket connection
bind to the socket
listen to the socket
master file descriptor
wait for messages
*/

// prototypes
int create_connection();

int create_connection()
{
    // dunno about these guys, needed for accept(), look in beej later
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    // create a struct for our address info
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // IPv4 or v6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    // get our socket info
    int status;
    if ((status = getaddrinfo(NULL, MYPORT, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // create a socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
        printf("socket error %i\n", sockfd);

    // bind socket to the port we passed in to getaddrinfo()
    int bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (bind == -1)
        printf("bind error %i\n", bind);

    // listen on that port
    int listen(sockfd, BACKLOG);
    if (listen == -1)
        printf("listen error %i\n", listen);

    // accept incoming connections
    addr_size = sizeof(their_addr);
    int new_sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    //freeaddrinfo(res); // free the linked list

    // return the socket for later calls
    return new_sockfd;
}

int main()
{
    int socket = create_connection();
}