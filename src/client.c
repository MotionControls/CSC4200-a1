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

#define EXT_ADDR	"localhost"
#define EXT_PORT	"8008"

int main(int argc, char** argv){
	printf("Starting client...\n");
	
	struct addrinfo hints, *res, *walk;
	int status, sock;
	char ipstr[INET6_ADDRSTRLEN];
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if((status = getaddrinfo(EXT_ADDR, EXT_PORT, &hints, &res)) != 0){
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
	
	printf("Connecting to %s:%s...\n", EXT_ADDR, EXT_PORT);
	if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
		perror("connect err.\n");
		return 1;
	}else{
		printf("Connected.\n");
	}
	
	printf("Exiting...\n");
	return 0;
}