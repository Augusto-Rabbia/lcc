#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <netinet/ip.h>

#define MAX_CLIENTES 10

int turnos;

/*
 * Para probar, usar netcat. Ej:
 *
 *      $ nc localhost 4040
 *      NUEVO
 *      0
 *      NUEVO
 *      1
 *      CHAU
 */

void quit(char *s)
{
	perror(s);
	abort();
}

int U = 0, hijos = 0;

int fd_readline(int fd, char *buf) {
	int rc;
	int i = 0;

	/*
	 * Leemos de a un caracter (no muy eficiente...) hasta
	 * completar una línea.
	 */
	while ((rc = read(fd, buf + i, 1)) > 0) {
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc < 0)
		return rc;

	buf[i] = 0;
	return i;
}

void handle_conn(int csock) {
	char buf[200];
	int rc;

    /* Atendemos pedidos, uno por linea */
    rc = fd_readline(csock, buf);
    if (rc < 0)
        quit("read... raro");

    if (rc == 0) {
        /* linea vacia, se cerró la conexión */
        close(csock);
        return;
    }

    if (!strcmp(buf, "NUEVO")) {
        char reply[20];
        lseek(turnos, 0, SEEK_SET);
        read(turnos, reply, 10);
        int i = atoi(reply);
        write(csock, reply, strlen(reply));
        i++;
        sprintf(reply, "%d\n", i);
        lseek(turnos, 0, SEEK_SET);
        write(turnos, reply, strlen(reply));
    } else if (!strcmp(buf, "CHAU")) {
        close(csock);
        return;
    }
	
	
}

void wait_for_clients(int lsock) {
    struct epoll_event ev, events[MAX_CLIENTES];
	int conn_sock, nfds, epollfd;
	epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = lsock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, lsock, &ev) == -1) {
        perror("epoll_ctl: lsock");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_CLIENTES, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == lsock) { // Nuevo cliente se intenta conectar
                conn_sock = accept(lsock, NULL, NULL);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                ev.events = EPOLLIN;
                ev.data.fd = conn_sock;
                // Añadimos el fd del cliente nuevo a la lista de interés
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } else {
                handle_conn(events[i].data.fd);
            }
        }
    }

}

/* Crea un socket de escucha en puerto 4040 TCP */
int mk_lsock() {
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(4040);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main() {
	turnos = open("turnos.txt", O_RDWR | O_CREAT);
	write(turnos, "0\n", 2);
	lseek(turnos, 0, SEEK_SET);
	int lsock;
	lsock = mk_lsock();
	wait_for_clients(lsock);
}
