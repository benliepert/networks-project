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

struct client
{
    char *name;        // store this once we get NAME command
    char *channel;      // client's channel(s)? TODO maybe make this bigger to allow for larger channel names
    int fd;            // file descriptor
};

client *create_client(int fd)
{
    struct client *new_client = (client *)malloc(32);
    new_client->name = (char *)malloc(25);
    new_client->name = "default";
    new_client->fd = fd;
    return new_client;
}

client *identify_client(int fd, client *client_array[], int fdmax)
{
    printf("1\n");
    for (int k = 0; k <= fdmax; k++)
    {
        printf("2\n");
        if (client_array[k]->fd == fd)
        {
            printf("3\n");
            return client_array[k]; // get current client
            break;
        }
    }
    printf("failed to find established client\n");
    exit(5);
}

void update_client(int fd, client *client_array[], client *current_client, int fdmax)
{
    for (int k = 0; k <= fdmax; k++)
    {
        if (client_array[k]->fd == fd)
        {
            client_array[k] = current_client; // store it again with the updated values
            break;
        }
    }
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int sendall(int s, char *buf, int len)
{
    int total = 0;       // how many bytes we've sent
    int bytesleft = len; // how many we have left to send
    int n;
    while (total < len)
    {
        n = send(s, buf + total, bytesleft, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    len = total;             // return number actually sent here
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
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
    int i, j;
    int client_number = 0;           // keeps track of the how many clients we have
    struct client *client_array[40]; // create our array of clients

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
            //printf("1\n");
            if (FD_ISSET(i, &copy))
            {
                //printf("2\n");
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
                        struct client *new_client;
                        new_client = create_client(newfd);
                        client_array[client_number] = new_client; // append new client to the array
                        client_number = client_number + 1;
                    }
                }
                else
                {
                    //printf("3\n");
                    // handle data from a client
                    char buf[9999]; // buffer for client data
                    int nbytes;
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
                        //printf("4\n");
                        /*
                        for(int y = 0; y < client_number; y++)
                        {
                            printf("Array contains: %i\n", client_array[y]->fd);
                        }
                        */

                        char token_buf[9999];
                        strcpy(token_buf, buf);
                        char *command = strtok(token_buf, " "); //strtok returns first split element

                        // identifies the socket with the correct structure
                        struct client *current_client = identify_client(i, client_array, client_number);
                        printf("client socket = %i \n", current_client->fd);

                        // NAME
                        if (!strcmp(command, "NAME")) // string compare if == 0 -> strings are equal
                        {
                            printf("1 client name = %s \n", current_client->name);
                            //printf("COMMAND: %s \n", command);

                            char *name = strtok(NULL, token_buf); // get to next token, ie the name
                            printf("COMMAND: %s \n", name);
                            current_client->name = name;
                            printf("COMMAND: %s \n", name);
                            char *newChannel = current_client->channel;
                            
                            //current_client->name = name;
                            //memcpy(current_client->name, name,int 5);
                            printf("2 client name = %s \n", current_client->name);
                        }

                        printf("3 client name = %s \n", current_client->name);
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

                        // update the struct in the array
                        update_client(i, client_array, current_client, fdmax);
                        // check if we can send messages by having a valid NAME and CHANNEL
                        // we got some data from a client
                        for (j = 0; j <= fdmax; j++)
                        {
                            // send to everyone...
                            if (FD_ISSET(j, &master))
                            {
                                // except the listening and ourselves
                                if (j != listening && j != i)
                                {
                                    if (sendall(j, buf, nbytes) == -1)
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