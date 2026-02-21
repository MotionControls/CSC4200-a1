CC=gcc
CFLAGS=-I.

all: src/server.c src/client.c
	clear
	$(CC) -o appserver src/server.c $(CFLAGS)
	$(CC) -o appclient src/client.c $(CFLAGS)