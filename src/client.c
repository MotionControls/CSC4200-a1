/*
Your client must:

- Construct a 12-byte header.
- Set `Version = 17`.
- Set `Message Type` to any value you choose for this sprint.
- Set `Message Length` equal to the payload size.
- Use `htonl()` on all 32-bit fields.
- Manually copy each field into a byte buffer.
- Send the header.
- Send a payload that says `"Hello"` (or another short string).

Important:

- Do not send a raw struct.
- Do not rely on compiler memory layout.
- Build the buffer manually using `memcpy()`.
*/

#include "shared.h"

int main(int argc, char** argv){
	// Check if required arguments exist, or user needs usage.
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
	
	// Setup address.
	// https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
	memset(&hints, 0, sizeof(hints));	// Clear memory.
	hints.ai_family = AF_UNSPEC;		// Use IPv4 or IPv6.
	hints.ai_socktype = SOCK_STREAM;	// Use TCP sockets.
	
	status = getaddrinfo(argv[1], port, &hints, &res);
	if(status != 0){
		printf("getaddrinfo err: %s.\n", gai_strerror(status));
		return 1;
	}
	
	// Create socket using given address info.
	sock = CreateSocket(res);
	
	// Connect using socket.
	printf("Connecting to %s:%s...\n", argv[1], port);
	if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
		perror("connect err");
		return 1;
	}else{
		// Convert connected address to char*.
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
		
		// Assemble header.
		uint32_t version = htonl(17);
		uint32_t type = htonl(8008);
		uint32_t length = htonl(strlen(msg));
		uint32_t header[3];
		memcpy(&header[0], &version, sizeof(uint32_t));
		memcpy(&header[1], &type, sizeof(uint32_t));
		memcpy(&header[2], &length, sizeof(uint32_t));
		
		// Send header.
		int numbytes = send(sock, header, HEADER_SIZE, 0);
		if(CheckSend(numbytes, HEADER_SIZE)){
			close(sock);
			return 1;
		}
		
		// Send message.
		int msglen = strlen(msg);
		numbytes = send(sock, msg, msglen, 0);
		if(CheckSend(numbytes, msglen)){
			close(sock);
			return 1;
		}
		
		// Get response header.
		time_t startTime = time(NULL);
		numbytes = GetBuffer(HEADER_SIZE, header, sock, startTime, -1);
		if(CheckRecv(numbytes, HEADER_SIZE, startTime)){
			close(sock);
			return 1;
		}
		
		printf("Got header:\n\tVersion: %i\n\tType: %i\n\tLength: %i\n", ntohl(header[0]), ntohl(header[1]), ntohl(header[2]));
		
		// Get response.
		char buffer[BUFFER_SIZE];
		startTime = time(NULL);
		numbytes = GetBuffer(BUFFER_SIZE, buffer, sock, startTime, ntohl(header[2]));
		if(CheckRecv(numbytes, 1, startTime)){
			close(sock);
			return 1;
		}
		
		buffer[numbytes] = '\0';
		if(strlen(buffer) != ntohl(header[2])){
			printf("recv err: buffer length doesn't match header. Expected %u bytes, got %lu.\n", ntohl(header[2]), strlen(buffer));
			close(sock);
			return 1;
		}
		printf("Got response \"%s\".\n", buffer);
		
		close(sock);
	}
	
	printf("Exiting...\n");
	return 0;
}