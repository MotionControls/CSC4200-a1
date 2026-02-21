#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define USED_PORT	"8008"
#define BUFFER_SIZE	100

void WalkAddrInfo(struct addrinfo* res){
	char ipstr[INET6_ADDRSTRLEN];
	for(struct addrinfo* walk = res; walk != NULL; walk = walk->ai_next){
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
}

int CreateSocket(struct addrinfo* res){
	printf("Creating socket...\n");
	int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sock == -1){
		perror("socket err");
		exit(1);
	}
	
	return sock;
}

#endif