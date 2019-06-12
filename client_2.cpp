#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MSG_LEN 100

using namespace std;

typedef struct class 
{
	int id; /**< Client ID*/
	int sockfd;  /**< Socket File Descriptor*/
	char message[MSG_LEN]; /**< Message Sent/Received*/
}ClientStruct;

int main(int argc, char** argv)
{
	/** Setting the arguments to their variables*/
	if(argc < 3) {
		perror("[-] Usage: ./server hostname port\n");
		return -1;
	}


	int port_no = atoi(argv[2]); /**< Port Number*/
	if(port_no <= 0) {
		perror("[-] Invalid Port Number. \n");
		return -2;
	}

	struct hostent *server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		perror("[-] No such host present. \n");
		return -3; 
	}

	cout << "Starting client..." << endl;

	/** Socket Created*/
	ClientStruct client ={INVALID_SOCKET, -1, ""};
	client.sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(client.sockfd < 0)
	{
		perror("[-] Error creating a socket");
		return -4;
	}


	struct sockaddr_in server_addr; /**< Server Address*/
	bzero((char *)&server_addr, sizeof(server_addr)); // Reset
	server_addr.sin_family = AF_INET; 

	bcopy(
			(char *)server->h_addr, 
			(char *)&server_addr.sin_addr.s_addr, 
			server->h_length
		); // Copy the server address

	server_addr.sin_port = htons(port_no); // Setting the port number

	

	return 0;
}