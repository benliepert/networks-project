// custom IRC server
// created by Gezim Saciri, Ben Leipert, Josh Blaz

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
establish socket connection
bind to the socket
listen to the socket
master file descriptor
wait for messages
*/

// Prototypes
int create_connection();

int create_connection()
{
    // create a struct for our address info
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     //IPv4 or v6 is ok
    hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     //fill in my IP for me

    // built in struct to get our socket info
    int status;
    if ((status = getaddrinfo("mail.denison.edu", "540001", &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // === DO EVERYTHING BELOW ======================================================

    // make a socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
        printf("Socket Error %i\n", sockfd);

    // bind it to the port we passed in to getaddrinfo():
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    // connect
    int connectStatus = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if (connectStatus == -1)
        printf("Connect Error %i\n", connectStatus);

    // return the socket for later calls
    //recvAll(s);

    //===============================================================================

    freeaddrinfo(res); // free the linked list

    return sockfd;
}

int main()
{
    int socket = create_connection();
}