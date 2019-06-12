#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "sha256.h"

#define MSG_LEN 100
#define MAX_CLIENTS 20

using namespace std;

/** FLAGS */
const int INVALID_SOCKET = -1;
const int SOCKET_ERROR	 = -1;

struct client_type
{
    int id;
    int sockfd;
};


int ClientProcessor(client_type &new_client, std::vector<client_type> &client_array, std::thread &client_thread)
{
	char charMsg[MSG_LEN] = "";
	std::string msg = "";
  int retries = 10;

	while(1)
	{
		memset(charMsg, 0, MSG_LEN);
    msg = "";
		if(new_client.sockfd != 0)
		{
			int SentFlag = recv(new_client.sockfd, charMsg, MSG_LEN, 0);

			if(SentFlag != SOCKET_ERROR) {
				if(strcmp("", charMsg) != 0)
				{
					// msg = "Client ID: "+ to_string(new_client.id) + " --> " + charMsg ;
          msg = string(charMsg);
          for(int i = 0; i < 10000; i++)
          {msg = sha256(msg);}

          cout << "[*] Client #" << to_string(new_client.id) << ": " << msg << endl;
          SentFlag = send(new_client.sockfd, msg.c_str(), strlen(msg.c_str()), 0);
          continue;
				}

      }
      retries--;
      if(retries <= 0 || SentFlag == SOCKET_ERROR)
      {
        retries = 10;
        cout << "[*] Client #" << to_string(new_client.id) << " disconnected" << endl;
        close(new_client.sockfd);
        close(client_array[new_client.id].sockfd);
        client_array[new_client.id].sockfd = INVALID_SOCKET;
        break;
      }


		}

	}


	client_thread.detach();
	return 0;
}

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
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); /**< Socket File Descriptor*/


// Make it able to reuse the ports
  int yes = 1;
   if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
   {
     printf("[-] Unable to set socket options");
     exit(1);
   }

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
    // Initialising the client list
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		client[i] = (client_type) {-1, INVALID_SOCKET};
	}

	struct sockaddr_in client_addr; /**< Client Address*/
	socklen_t Client_len = sizeof(client_addr);/**< Client Address Length*/

	int NumClients = 0; /**< Total Number of Clients */
	int ID_Assign; /**< ID Assigner*/
	thread client_threads[MAX_CLIENTS]; /**< Threads of the clients */
	string msg = "";

	while (1)
	{
		int Incoming = INVALID_SOCKET; /**< Accepted flag */
		Incoming = accept(sockfd, (struct sockaddr *)&client_addr, &Client_len);

		if(Incoming == INVALID_SOCKET) {
			continue;
		}

		ID_Assign = -1;
    NumClients = -1;


		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			// If a client hasn't been added to the list of clients
			if(client[i].sockfd == INVALID_SOCKET && ID_Assign == -1)
			{
				client[i].sockfd = Incoming;
				client[i].id = i;
				ID_Assign = i;
			}

			if(client[i].sockfd != INVALID_SOCKET)
			{
				NumClients++;
			}
		}

		if(ID_Assign != -1) // If max. clients are not reached
		{
			cout << "[+] Connected! There are " << NumClients + 1 << " clients in this server. MAX: " << MAX_CLIENTS  << endl;

			// Send the ID to the new client
			msg = to_string(client[ID_Assign].id);
			send(client[ID_Assign].sockfd, msg.c_str(), strlen(msg.c_str()), 0);

			client_threads[ID_Assign] = std::thread(
				ClientProcessor, std::ref(client[ID_Assign]),
								 std::ref(client),
								 std::ref(client_threads[ID_Assign]));
		}

		else // Server is full
		{
			msg = "Server is full";
			send(Incoming, msg.c_str(), strlen(msg.c_str()), 0);

			//Get the address of remote host
			char remote_host[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(client_addr.sin_addr), remote_host, INET_ADDRSTRLEN);

			cout << "[-] Connection rejected - Server full! Enter :exit on clients to remove connection. " << endl;
		}

	}

	close(sockfd);

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		client_threads[i].detach();
		close(client[i].sockfd);
	}
	return 0;
}
