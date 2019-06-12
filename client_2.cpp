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

int main(int argc, char** argv)
{
	/** Setting the arguments to their variables*/
	if(argc < 3) {
		perror("Usage: ./server hostname port\n");
	}


	int port_no = atoi(argv[2]); /**< Port Number*/
	if(port_no <= 0) {

	}

	return 0;
}