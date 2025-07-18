CC = g++
CFLAGS = -Wall -g

client: client.o
	$(CC) client.o -o client

server: server.o
	$(CC) server.o -o server

client.o: client.cpp
	$(CC) -c client.cpp

server.o: server.cpp
	$(CC) -c server.cpp