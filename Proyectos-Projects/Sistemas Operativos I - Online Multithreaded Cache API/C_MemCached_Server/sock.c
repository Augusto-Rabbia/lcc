#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include "common.h"

int mk_tcp_sock(in_port_t port) {
	int s, rc;
	struct sockaddr_in sa;
	int yes = 1;

	/*Completar la llamada a socket y descomentar*/
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
		quit("socket");

	rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (rc != 0)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/*Completar  y descomentar*/
	rc = bind(s, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");
	

	rc = listen(s, MAX_CLIENTS);
	if (rc < 0)
		quit("listen");

	return s;
}

void set_non_blocking(int fd) {
	int flags, s;
	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
    	perror ("fcntl");
      	exit(EXIT_FAILURE);
  	}
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1) {
		perror ("fcntl");
		exit(EXIT_FAILURE);
	}
}
