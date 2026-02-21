/*
Your client must:

- Create a TCP socket.
- Connect to the server using `connect()`.
- Send a simple test message such as `"HELLO"`.
- Receive the server’s reply.
- Print the reply.
- Close the socket cleanly.
*/

#include "shared.h"

int main(int argc, char** argv){
	if(argc <= 1 || strcmp(argv[1], "--help") == 0){
		printf("USAGE: appclient [address] [?port]\n");
		printf("\taddress: Address to connect to.\n\t?port: Port to connect to. Defaults to 8008.\n");
		return 0;
	}
	
	printf("Starting client...\n");
	
	struct addrinfo hints, *res, *walk;
	int status, sock;
	char ipstr[INET6_ADDRSTRLEN];
	char* port = (argc > 2) ? argv[2] : USED_PORT;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	status = getaddrinfo(argv[1], port, &hints, &res);
	if(status != 0){
		printf("getaddrinfo err: %s.\n", gai_strerror(status));
		return 1;
	}
	
	WalkAddrInfo(res);
	sock = CreateSocket(res);
	
	printf("Connecting to %s:%s...\n", argv[1], port);
	if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
		perror("connect err");
		return 1;
	}else{
		void* addr;
		struct sockaddr* check = (struct sockaddr*)res->ai_addr;
		if(check->sa_family == AF_INET){
			addr = &(((struct sockaddr_in*)check)->sin_addr);
		}else{
			addr = &(((struct sockaddr_in6*)check)->sin6_addr);
		}
		
		inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
		
		char* msg = "PING";
		printf("Connected to %s.\nSending \"%s\".\n", ipstr, msg);
		
		int numbytes = send(sock, msg, strlen(msg), 0);
		if(numbytes <= 0){
			perror("send err");
			printf("Sent %i bytes.\n", numbytes);
			close(sock);
			return 1;
		}
		
		char buffer[BUFFER_SIZE];
		numbytes = recv(sock, buffer, BUFFER_SIZE-1, 0);
		if(numbytes <= 0){
			perror("recv err");
			printf("Received %i bytes.\n", numbytes);
			close(sock);
			return 1;
		}
		
		buffer[numbytes] = '\0';
		printf("Got response \"%s\".\n", buffer);
		close(sock);
	}
	
	printf("Exiting...\n");
	return 0;
}