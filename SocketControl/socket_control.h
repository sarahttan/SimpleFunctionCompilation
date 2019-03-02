#ifndef SOCKET_CONTROL_H
#define SOCKET_CONTROL_H
 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

int socket_server_open(int port, int *server_fd); 	// open port at default 8080 if given -1.  Return -1 on failure or socket on success.  Server fd will be stored in passed in variable.
int socket_server_close(int server_fd);	// close port.  Return -1 on failure, 0 on success.
int socket_client_connect(char *server_address); 	// connect from the client to the server at the ip server_address.  Return sock number on success, -1 on failure.
int socket_close(int socket);	// close the socket from the client side.  Call this function on all server/client sockets before closing the server.
int socket_send(int socket, char *buffer, int buffer_length);	// Upon successful completion, return the number of bytes sent. Otherwise, -1 shall be returned and errno set to indicate the error.
int socket_receive(int socket, char *buffer, int buffer_length);	//On success, the number of bytes read is returned.  On error, -1 is returned, and errno is set appropriately.

#ifdef __cplusplus
}
#endif

#endif