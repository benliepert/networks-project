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
#include <stdlib.h>

#define MYPORT "54001"
#define BACKLOG 10

// prototypes
int create_connection();
typedef struct addrinfo addrinfo;

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
    int status; // need to store it for error reporting
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

    int yes = 1;
    struct addrinfo *p;
    for (p = res; p != NULL; p = p->ai_next)
    {
        listening = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listening < 0)
        {
            continue;
        }
        // lose the pesky "address already in use" error message
        setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listening, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listening);
            continue;
        }
        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL)
    {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }
    freeaddrinfo(res); // all done with this
    // listen
    if (listen(listening, 10) == -1)
    {
        perror("listen");
        exit(3);
    }

    //==================================================================================
    // SELECT STUFF

    fd_set master;
    FD_ZERO(&master);
    FD_SET(listening, &master);

    int fdmax = listening;
    int i, j;

    for (;;)
    {
        fd_set copy = master;

        if (select(fdmax + 1, &copy, NULL, NULL, NULL) = -1) //setting last val to NULL means it will never timeout
        {
            perror("select");
            exit(4);
        }

        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &copy))
            {
                if (i == listening)
                {
                    //accept new connection
                    struct sockaddr_storage remoteaddr;
                    socklet_t addrlen = sizeof remoteaddr;
                    int newfd = accept(listening, (struct sockaddr *)&remoteaddr, &addrlen);
                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax)
                        {
                            fdmax = newfd;
                        }
                        char remoteIP[INET6_ADDRSTRLEN];
                        printf("selectserver: new connection from %s on "
                               "socked %d \n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                    }
                }
                else
                {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
                    {
                        // got error or connection closed by client
                        if (nbytes == 0)
                        {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(i);           // bye!
                        FD_CLR(i, &master); // remove from master set
                    }
                    else
                    {
                        // we got some data from a client
                        for (j = 0; j <= fdmax; j++)
                        {
                            // send to everyone!
                            if (FD_ISSET(j, &master))
                            {
                                // except the listener and ourselves
                                if (j != listener && j != i)
                                {
                                    if (send(j, buf, nbytes, 0) == -1)
                                    {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // accept incoming connections
    socklen_t addr_size = sizeof(their_addr);
    int new_sockfd = accept(listening, (struct sockaddr *)&their_addr, &addr_size);

    //==================================================================================

    // return the socket for later calls ?
    return new_sockfd;
}

int main()
{
    int socket = create_connection();
}