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