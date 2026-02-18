/*
Your client must:

- Create a TCP socket.
- Connect to the server using `connect()`.
- Send a simple test message such as `"HELLO"`.
- Receive the server’s reply.
- Print the reply.
- Close the socket cleanly.
*/

#include <stdio.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char** argv){
	printf("Starting client...\n");
	
	struct addrinfo hints, *res, *p;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if((int status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0){
		printf("getaddrinfo err: %s.\n", gai_strerror(status));
		return 1;
	}
	
	if((int sock = socket(res->ai_family, res_ai_socktype, res->ai_protocol)) == -1){
		printf("socket err.\n");
		return 1;
	}
	
	if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
		printf("connect err.\n");
		return 1;
	}else{
		
	}
}