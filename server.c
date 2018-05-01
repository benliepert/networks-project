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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // built in struct to get our socket info
    getaddrinfo("mail.denison.edu", "540001", &hints, &res);

    // make a socket
    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == -1)
        printf("Socket Error %i\n", s);

    // connect
	int connectStatus = connect(s, res->ai_addr, res->ai_addrlen);
	if(connectStatus == -1)
		printf("Connect Error %i\n", connectStatus);
	
	// return the socket for later calls
	recvAll(s);

	return s;
}

int main()
{
    socket = create_connection();
}