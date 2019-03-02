#include "socket_control.h"
#include <signal.h>

int new_socket;
int server_fd;

void exit_function(int signal) {
	printf("\nExiting Program");
	socket_close(new_socket);
	socket_server_close(server_fd);
	exit(signal);
}

int main(int argc, char const *argv[]) 
{    
    char buffer[1024] = {0}; 
    new_socket = socket_server_open(-1, &server_fd);
    int valread;
	
	float timestamp;
	float rx_v;
	
	signal(SIGINT, exit_function);	// Capture Ctrl-C
	
	printf("Waiting in server receive for data\n");
	while(1) {
		valread = socket_receive(new_socket, buffer, 1024);
		//printf("%s\n", buffer);  // Uncomment to print only the buffer
		
		sscanf(buffer, "%f, %f\n", &timestamp, &rx_v);
		if ((timestamp >= 0) && (rx_v >= 0)) {
			printf("%02.8f, %02.4f\n", timestamp, rx_v );  // Uncomment to print only the timestamp and rx_v if its valid
		}
	}
	
	exit_function(0);
    return 0; 
} 