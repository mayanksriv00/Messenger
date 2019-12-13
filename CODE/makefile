# Makefile for client and server messenger


CC=g++
CPPFLAGS=-g -Wall -std=c++11


all: messenger_server messenger_client
	

messenger_server: messenger_server.o
	$(CC) messenger_server.o -o messenger_server

messenger_client: messenger_client.o
	$(CC) messenger_client.o -o messenger_client -pthread
	
messenger_client.o: messenger_client.cpp
	$(CC) -c $(CPPFLAGS) -pthread messenger_client.cpp

messenger_server.o: messenger_server.cpp
	$(CC) -c $(CPPFLAGS) messenger_server.cpp



clean:
	rm -f *~ messenger_client messenger_server *.o core
