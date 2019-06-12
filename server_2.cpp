#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MSG_LEN 100
#define MAX_CLIENTS 5

using namespace std;

/** FLAGS */
const int INVALID_SOCKET = -1;
const int SOCKET_ERROR	 = -1;

struct client_type
{
    int id;
    int sockfd;
};


int main(int argc, char** argv)
{
	if(argc < 2)
	{
		perror("[-] Usage: ./client port_number \n");
		return -1;
	}

	int port_no = atoi(argv[1]); /**< Port Number*/
	if(port_no <=0)
	{
		perror("[-] Invalid Port \n");
		return -1;
	}

	/** Creating a socket */
	cout << "Creating a server socket ..." << endl;
	sockfd = socket(AF_INET, SOCK_STREAM, 0); /**< Socket File Descriptor*/

	if(sockfd < 0)
	{
		perror("[-] ERROR Opening Socket \n");
		return -2; 
	}

	/** Setting up the address structure */
	struct sockaddr_in server_addr;
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family 		= 	AF_INET;
	server_addr.sin_addr.s_addr = 	INADDR_ANY;
	server_addr.sin_port		=	htons(port_no);

	/** Binding */
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("[-] Error while binding \n");
		return -3;
	}


	listen(sockfd, 5); 
	char local_host[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_addr.sin_addr), local_host, INET_ADDRSTRLEN);
    cout << "Listening on " <<  local_host << " : " << port_no << endl;

	std::vector<client_type> client(MAX_CLIENTS); /**< Clients connected to this server*/
    // Initialising the client llist
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		client[i] = (client_type) {-1, INVALID_SOCKET};
	}

	struct sockaddr_in client_addr; /**< Client Address*/
	socklen_t Client_len = sizeof(client_addr);

	while (1)
	{
		int Incoming = INVALID_SOCKET; /**< Accepted flag */
		Incoming = accept(sockfd, (struct sockaddr *)&client_addr, &Client_len);

		if(Incoming == INVALID_SOCKET) {
			continue;
		}
	}
	return 0;
}