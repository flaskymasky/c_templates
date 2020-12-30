#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv)
{
	struct addrinfo hints, *res, *p;
	char ip[INET6_ADDRSTRLEN];
	int status;

	hints.ai_family = AF_UNSPEC; // ipv4 and 6
	hints.ai_socktype = SOCK_STREAM; // tcp

	// 			    name,   port,  how,   list with ips
	if ( (status = getaddrinfo(argv[1], NULL, &hints, &res)) == -1 ) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	printf("Ip Addresses of : %s\n\n", argv[1]);

	for ( p = res; p != NULL; p = p->ai_next)
	{
		void *addr;

		// IPV4
		if ( p->ai_family == AF_INET )
		{
			struct sockaddr_in *sa4 = ( struct sockaddr_in *) p->ai_addr;
			addr = &(sa4->sin_addr);
		}
		// IPV6
		else
		{
			struct sockaddr_in6 *sa6 = ( struct sockaddr_in6 *) p->ai_addr;
			addr = &(sa6->sin6_addr);
		}

		// make printable
		if ( inet_ntop(p->ai_family, addr, ip, sizeof(ip)) == NULL ) {
			perror("");
			exit(1);
		}


		printf("IP: %s\n", ip);

		memset(ip, 0, sizeof(ip));

	}


	freeaddrinfo(res);

	return 0;
}
	

