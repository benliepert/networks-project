// custom IRC client
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
establish tcp connection
send messages
create a case for ending the connection
*/