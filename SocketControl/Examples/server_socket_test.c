// Server side C/C++ program to demonstrate Socket programming 
#include "socket_control.h"

int main(int argc, char const *argv[]) 
{ 
     
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
	int server_fd;
    int new_socket = socket_server_open(-1, &server_fd);
    
	// Open socket on server side to send message.
	int valwrite = socket_send(new_socket, hello, strlen(hello));
    int valread = socket_receive(new_socket, buffer, 1024);
	printf("%s\n",buffer );  
	
	socket_close(new_socket);
	socket_server_close(server_fd);
    return 0; 
} 