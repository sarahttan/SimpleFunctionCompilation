#include "socket_control.h"
#define PORT 8080 

int socket_server_open(int port, int *server_fd) {
	// Check if the port is a valid number
	if (port < 0) {
		port = PORT;
	}
	
	// Initialize Socket
	int new_socket; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
	
	// Creating socket file descriptor 
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket_open: socket failed"); 
		return -1;
    } 
       
    // Forcefully attaching socket to the port
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("socket_open: setsockopt"); 
		return -1;
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 
       
    // Forcefully attaching socket to the port 
    if (bind(*server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("socket_open: bind failed"); 
        return -1;
    } 
    if (listen(*server_fd, 3) < 0) 
    { 
        perror("socket_open: listen"); 
        return -1;
    } 
    if ((new_socket = accept(*server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("socket_open: accept"); 
        return -1;
    } 
	
	printf("\nsocket_control: Accepting socket %d. \n", new_socket); 
	return new_socket;
}

int socket_server_close(int server_fd) {
	printf("\nsocket_control: Closing server side socket. \n"); 
	if (server_fd >= 0) {
		close(server_fd);
	} else {
		printf("\n\t Server close socket failed. \n"); 
	}
	
	return 0;
}

int socket_send(int socket, char *buffer, int buffer_length) {
	return send(socket , buffer , buffer_length , 0 ); 
}

int socket_receive(int socket, char *buffer, int buffer_length) {
	return read( socket , buffer, buffer_length); 
}

int socket_client_connect(char *server_address) {
	// Check if the input is a correct address
	char *server_address_internal;
	if (server_address == NULL) {
		server_address_internal = "127.0.0.1";
	} else {
		server_address_internal = server_address;
	}
	
    int sock = 0; 
    struct sockaddr_in serv_addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\nsocket_control:  Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, server_address_internal, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nsocket_control: Invalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nsocket_control: Connection Failed \n"); 
        return -1; 
    } 
	
	printf("\nsocket_control: Connected socket %d. \n", sock); 
    return sock; 
}

int socket_close(int socket) {
	printf("\nsocket_control: Closing socket %d. \n", socket); 
	if (socket >= 0) {
		close(socket);
	} else {
		printf("\n\tClient close failed. \n"); 
	}
	
	return 0;
}
