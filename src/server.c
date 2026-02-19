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
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define MY_PORT	"8008"

int main(int argc, char** argv){
	printf("Starting server...\n");
	
	struct addrinfo hints, *res, *walk;
	struct sockaddr_storage theirAddr;
	socklen_t theirSize;
	int status, sock, newSock;
	char ipstr[INET6_ADDRSTRLEN];
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if((status = getaddrinfo(NULL, MY_PORT, &hints, &res)) != 0){
		printf("getaddrinfo err: %s.\n", gai_strerror(status));
		return 1;
	}
	
	for(walk = res; walk != NULL; walk = walk->ai_next){
		void *addr;
		char *ipver;
		struct sockaddr_in *ipv4;
		struct sockaddr_in6 *ipv6;
		
		if(walk->ai_family == AF_INET){
			ipv4 = (struct sockaddr_in*)walk->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}else{
			ipv6 = (struct sockaddr_in6*)walk->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		
		inet_ntop(walk->ai_family, addr, ipstr, sizeof(ipstr));		
		printf("Using %s IP: %s.\n", ipver, ipstr);
	}
	
	printf("Creating socket...\n");
	if((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
		perror("socket err");
		return 1;
	}
	
	printf("Binding socket...\n");
	if(bind(sock, res->ai_addr, res->ai_addrlen) != 0){
		perror("bind err");
		return 1;
	}
	
	printf("Listening...\n");
	if(listen(sock, 10) != 0){
		perror("listen err");
		return 1;
	}
	
	theirSize = sizeof(theirAddr);
	if((newSock = accept(sock, (struct sockaddr*)&theirAddr, &theirSize)) == -1){
		perror("accept err");
		return 1;
	}else{
		printf("Connected.\n");
	}
	
	printf("Exiting...\n");
	return 0;
}