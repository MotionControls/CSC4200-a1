/*
Your server must:

- Create a TCP socket using `socket()`.
- Bind the socket to a port using `bind()`.
- Listen for connections using `listen()`.
- Accept incoming client connections using `accept()`.

Once a client connects:

- Receive data from the client.
- Print what was received.
- Send a response based on the data.
- Continue running after the client disconnects.

The server must not exit after handling one connection.

You must check return values from:

- `socket()`
- `bind()`
- `listen()`
- `accept()`
- `recv()`
- `send()`

If a call fails, print an error and handle it properly.
*/

#include <stdio.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define MYPORT	"42"

int main(int argc, char** argv){
	printf("Starting server...\n");
	
	struct addrinfo hints, *res, *p;
	struct sockaddr_storage theirAddr;
	socklen_t theirSize;
	int status, sock, newSock;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if((status = getaddrinfo(NULL, MYPORT, &hints, &res)) != 0){
		printf("getaddrinfo err: %s.\n", gai_strerror(status));
		return 1;
	}
	
	if((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
		printf("socket err.\n");
		return 1;
	}
	
	if(bind(sock, res->ai_addr, res->ai_addrlen) != 0){
		printf("bind err.\n");
		return 1;
	}
	
	if(listen(sock, 10) != 0){
		printf("listen err.\n");
		return 1;
	}
	
	theirSize = sizeof(theirAddr);
	if((newSock = accept(sock, (struct sockaddr*)&theirAddr, &theirSize)) == -1){
		printf("accept err.\n");
		return 1;
	}else{
		
	}
	
	
	return 0;
}