// custom IRC server
// created by Gezim Saciri, Ben Leipert, Josh Blaz
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>

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
    int listening = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listening == -1)
        printf("socket error %i\n", listening);

    // bind socket to the port we passed in to getaddrinfo()
    // this is so that we can keep listening on this port
    int bindResult = bind(listening, res->ai_addr, res->ai_addrlen);
    if (bindResult == -1)
        printf("bind error %i\n", bindResult);

    // listen on that port
    int listenResult = listen(listening, BACKLOG);
    if (listenResult == -1)
        printf("listen error %i\n", listenResult);

    //==================================================================================
    // SELECT STUFF

    fd_set master;
    FD_ZERO(&master);
    FD_SET(listening, &master);

    bool running = true;
    while(running)
    {
        fd_set copy = master;

        int socketCount = select(0, &copy, NULL, NULL, NULL);

        for (int i = 0; i < socketCount; i++)
        {
            int currentSocket = copy.fd_array[i];
        }   
    }


    // accept incoming connections
    socklen_t addr_size = sizeof(their_addr);
    int new_sockfd = accept(listening, (struct sockaddr *)&their_addr, &addr_size);


    //==================================================================================

    freeaddrinfo(res); // free the linked list

    // return the socket for later calls
    return new_sockfd;
}

int main()
{
    int socket = create_connection();
}