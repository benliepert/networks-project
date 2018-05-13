#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT "9034"

/*
struct channel
{
    struct channel *next;
    char* name; //channel name
    struct client clients[100];
    //int fd; // file descriptor?
    // linked list of users on the channel
};
*/

struct client
{
    char *name;        // store this once we get NAME command
    char *channels[5]; // client's channel(s)?
    int fd;            // file descriptor
};

client create_client(int fd)
{
    struct client i;
    i.fd = fd;
    return i;
}
/*
client identify_client(int fd, client structArray[], int fdmax)
{
    for (int j = 0; j < fdmax; j++)
    {
        if (structArray[j].fd == fd)
        {
            return structArray[j];
        }
    }
}
*/
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main()
{
    //=================CREATE LISTENING SOCKET==========================================

    // create a struct for our address info
    struct addrinfo hints, *res, *p;
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

    // select and bind to our socket
    int listening;
    for (p = res; p != NULL; p = p->ai_next)
    {
        listening = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listening < 0)
            continue;

        int yes = 1; // for setsockopt()
        // lose the "address already in use" error message
        setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listening, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listening);
            continue;
        }
        break;
    }
    // failed to bind
    if (p == NULL)
    {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(res);

    // listen
    if (listen(listening, 10) == -1)
    {
        perror("listen");
        exit(3);
    }

    fd_set master; // master file descriptor list
    FD_ZERO(&master);
    FD_SET(listening, &master);

    int fdmax = listening; // biggest file descriptor
    int i, j, nbytes;
    int structs = 0;
    client structArray[100]; // create our array of structs
    char buf[1024];           // buffer for client data

    //===================SELECT FOR MULTI I/O===========================================
    for (;;)
    {
        fd_set copy = master;
        if (select(fdmax + 1, &copy, NULL, NULL, NULL) == -1) // setting last val to NULL means no timeout
        {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
        for (i = 0; i <= fdmax; i++)
        {
            printf("1\n");
            if (FD_ISSET(i, &copy))
            {
                printf("2\n");
                // found data to read from connection
                if (i == listening)
                {
                    // handle new connection
                    struct sockaddr_storage remoteaddr; // client address
                    socklen_t addrlen = sizeof remoteaddr;

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
                               "socket %d \n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                        // create a client for this new socket, so that it may be identified again in the future
                        struct client newClient;
                        newClient = create_client(newfd);
                        structArray[structs] = newClient; // append new client to the array of structs
                        printf("client struct array: %u \n", structArray[structs]);
                        structs = structs + 1;
                    }
                }
                else
                {
                    printf("3\n");
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
                    {
                        // got connection closed or eror by client
                        if (nbytes == 0)
                        {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i, &master); // remove from master set
                    }
                    else
                    {
                        printf("4\n");
                        // match i to structure
                        // i is the socket -> need to determine if we have a struct for it yet
                        char *command = strtok(buf, " "); //strtok returns first split element

                        /* identifies the socket with the correct structure, or make one for a new socket*/
                        //struct client currentClient = identify_client(i, structArray, fdmax); // get current client
                        
                        // or do this...
                        struct client currentClient;
                        for (int k = 0; k <= fdmax; k++)
                        {
                            if (structArray[k].fd == i)
                            {
                                currentClient = structArray[k]; // get current client
                            }
                        }
                        
                        // cannot do printf("socketnumber: %i \n", i) this will segfault due to repetition

                        if (!strcmp(command, "NAME")) // NAME command - strcmp = string compare if == 0 -> strings are equal
                        {
                            printf("COMMAND: %s \n", command);
                            command = strtok(NULL, buf); // get to next token, ie the name
                            printf("COMMAND: %s \n", command);
                            
                            currentClient.name = command;
        
                        }
                        printf("client name = %s \n", currentClient.name);
                        /*
                        if (strcmp(command, "JOIN")) // JOIN (channel) command
                        {
                            // check list of channels -> give client list of channels
                            // prompt client to choose one
                        }
                        else if (strcmp(command, "CREATE")) // CREATE (channel) command
                        {
                            // check list of channels against client's channel name
                            // if no match -> create channel
                            // otherwise prompt client for another channel name
                        }
                        else if (strcmp(command[0], "#") && sendMessages) 
                        {
                            // check list of channels against client's channel name
                            // if no match -> create channel
                            // otherwise prompt client for another channel name
                            
                            if(sendMessages){
                            // specify channel? // currentchannel?
                            // send buffer to other clients in channel
                            }
                        }
                        */
                        //messages: temporary buffer: "NAME: concat <old buffer>"
                        // we got some data from a client
                        for (j = 0; j <= fdmax; j++)
                        {
                            // send to everyone...
                            if (FD_ISSET(j, &master))
                            {
                                // except the listening and ourselves
                                if (j != listening && j != i)
                                {
                                    if (send(j, buf, nbytes, 0) == -1)
                                    {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            }     // END got new incoming connection
        }         // END looping through file descriptors
    }             // END for(;;)
    return 0;
}