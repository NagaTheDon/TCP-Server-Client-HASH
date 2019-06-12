.DEFAULT_GOAL := all

CC=g++
CFLAGS=-Wall -pthread -std=c++11

server:
	$(CC) $(CFLAGS) -o server server.cpp sha256.cpp
client:
	$(CC) $(CFLAGS) -o client client.cpp
all: server client

clean:
	rm -f server client
