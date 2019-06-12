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

	/** Attempting to connect*/
	cout << "Connecting to " << (char *)server->h_addr << " : " << port_no << endl; 
	if(connect(client.sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("[-] Error connecting to server");
		return -5;
	}

	cout << "Connected! " << endl;
	std::string sent_message = ""; /**< String with message to send*/
	int ret = 0; /**< Return Send value*/
	recv(client.sockfd, client.received_message, MSG_LEN, 0);
	message = client.received_message;

	if(message != "Server is full")
	{
		client.id = atoi(client.received_message); // First message is the ID

		while(1)
		{
			cout << "Enter a string to HASH: ";
			getline(cin, sent_message);

			ret = send(client.sockfd, sent_message.c_str(), strlen(sent_message.c_str()), 0);

			if(ret <= 0)
			{
				cout << "[-] Send() failed " << endl;
				break;
			}

			if(strcmp(sent_message, ":exit") == 0)
			{
				perror("[-] Disconnected from server \n");
				exit(1);
			}

			if(recv(client.sockfd, client.received_message, MSG_LEN, 0) < 0)
			{
				perror("[-] Error receiving data. \n");
			}
			else
			{
				cout << "Hashed Value: " << client.received_message << endl;
			}

			bzero(client.received_message, MSG_LEN);
			sent_message = "";
		}

	}

	else
	{
		cout << client.received_message << endl;
	}


	/** Socket Closure*/
	cout << "Closing socket ..." << endl;
	ret = shutdown(client.sockfd, SHUT_WR);

	if(ret == SOCKET_ERROR)
	{
		cout << "Shutdown failed " << endl;
		close(client.sockfd);
		return -4;
	}

	close(client.sockfd);

	return 0;
}