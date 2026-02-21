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

#include "shared.h"

int main(int argc, char** argv){
	if(argc > 1 && strcmp(argv[1], "--help") == 0){
		printf("USAGE: appserver [?address] [?port]\n");
		printf("\t?address: Address to host from. Defaults to first available machine address.\n\t?port: Port to host from. Defaults to 8008.\n");
		return 0;
	}
	
	printf("Starting server...\n");
	
	struct addrinfo hints, *res, *walk;
	struct sockaddr_storage theirAddr;
	socklen_t theirSize;
	int status, sock, newSock;
	char ipstr[INET6_ADDRSTRLEN];
	char* port = (argc > 2) ? argv[2] : USED_PORT;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if(argc > 1) hints.ai_flags = AI_PASSIVE;
	
	status = getaddrinfo(
		(argc > 1) ? argv[1] : NULL,
		port,
		&hints, &res);
	if(status != 0){
		printf("getaddrinfo err: %s.\n", gai_strerror(status));
		return 1;
	}
	
	WalkAddrInfo(res);
	sock = CreateSocket(res);
	
	int reuse = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1){
		perror("setsockopt err");
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
	
	while(1){
		theirSize = sizeof(theirAddr);
		
		newSock = accept(sock, (struct sockaddr*)&theirAddr, &theirSize);
		if(newSock == -1){
			perror("accept err");
			continue;
		}
		
		void* addr;
		struct sockaddr* check = (struct sockaddr*)&theirAddr;
		if(check->sa_family == AF_INET){
			addr = &(((struct sockaddr_in*)check)->sin_addr);
		}else{
			addr = &(((struct sockaddr_in6*)check)->sin6_addr);
		}
		
		inet_ntop(theirAddr.ss_family, addr, ipstr, sizeof(ipstr));
		printf("Connected to %s.\nListening for message...\n", ipstr);
		
		char buffer[BUFFER_SIZE];
		int numbytes = recv(newSock, buffer, BUFFER_SIZE-1, 0);		
		if(numbytes <= 0){
			perror("recv err");
			printf("Received %i bytes.\n", numbytes);
			close(newSock);
			continue;
		}
		
		buffer[numbytes] = '\0';
		char* msg = "PONG";
		printf("Received \"%s\".\nResponding with \"%s\".\n", buffer, msg);
		
		numbytes = send(newSock, msg, strlen(msg), 0);
		if(numbytes <= 0){
			perror("send err");
			printf("Sent %i bytes.\n", numbytes);
		}
		
		close(newSock);
		printf("Listening...\n");
	}
	
	printf("Exiting...\n");
	return 0;
}