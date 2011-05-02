/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 100

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


/*Vamos alterar da função send para sendto por
  causa do protocolo UDP*/
void sendStr(int sockfd, int numbytes, char str[], struct addrinfo *p){
   if ((numbytes = sendto(sockfd, str, strlen(str), 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
  return;
}


int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	char s[INET6_ADDRSTRLEN];
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;

	if (argc != 2) {
		fprintf(stderr,"usage: talker hostname\n");
		exit(1);
	}
	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}
	
	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}


	for(;;){
    scanf("%s", buf);
    sendStr(sockfd, numbytes, buf, p);


	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
				 p->ai_addr, &(p->ai_addrlen))) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains \"%s\"\n", buf);

	}
	freeaddrinfo(servinfo);
	close(sockfd);

	return 0;
}