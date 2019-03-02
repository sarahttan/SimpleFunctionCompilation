// Server side C/C++ program to demonstrate Socket programming 
#include "socket_control.h"
#include <unistd.h>
#include <signal.h>

int new_socket;
FILE *ADC_data;
char * line = NULL;

void exit_function(int signal) {
	// free structs and close all files
	if (line)
		free(line);
	socket_close(new_socket);
	fclose(ADC_data);
	exit(signal);
}

int main(int argc, char const *argv[]) 
{ 
	printf("Usage: ./client_fftw_ADC_data [filename to read]\n");
	if (argc < 2) {
		printf("\t Incorrect number of arguments\n");
		return 1;
	} 
	
	ADC_data = fopen(argv[1], "r");
	if (ADC_data == NULL) {
		printf("\t Unable to open file at %s\n", argv[1]);
		return 1;
	}
	
	// Connect to the socket
	new_socket = socket_client_connect(NULL);
	
	signal(SIGINT, exit_function);
	
	// read the lines and send them across the socket to fftw
    size_t len = 0;
	int valwrite = 0;
	ssize_t read;
	
	//while (1) {
		while ((read = getline(&line, &len, ADC_data)) != -1) {
			//printf("Retrieved line of length %zu:\n", read);
			printf("%s", line);
			valwrite = socket_send(new_socket, line, read);
			sleep(0.5);
		}
	//	fclose(ADC_data);
	//	ADC_data = fopen(argv[1], "r");
	//	if (ADC_data == NULL) {
	//		printf("\t Unable to open file at %s\n", argv[1]);
	//		return 1;
	//	}
	//}
	exit_function(0);
    return 0; 
} 