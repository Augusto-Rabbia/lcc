#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

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

void quit(char *s) {
	perror(s);
	abort();
}

int turnos;

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

	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(csock);
			return;
		}

		if (strcmp(buf, "NUEVO") == 0) {
			char reply[20];
			lseek(turnos, 0, SEEK_SET);
			read(turnos, reply, 20);
			printf("turnos: %s", reply);
			write(csock, reply, strlen(reply));
			
			int nturno = atoi(reply) + 1;
			sprintf(reply, "%d\n", nturno);

			printf("%s", reply);
			lseek(turnos, 0, SEEK_SET);
			write(turnos, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			close(csock);
			return;
		}
	}
}

void wait_for_clients(int lsock) {
	int csock;
    /* Esperamos una conexión, no nos interesa de donde viene */
    csock = accept(lsock, NULL, NULL);
    if (csock < 0)
        quit("accept");
    __pid_t pid = fork();
    if (pid == 0) {
        /* Atendemos al cliente */
        handle_conn(csock);
    }
	else
	    wait_for_clients(lsock);
    /* Volvemos a esperar conexiones */
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

int main()
{
	turnos = open("turnos.txt", O_RDWR | O_CREAT);
	write(turnos, "0\n", 2);
	lseek(turnos, 0, SEEK_SET);
	int lsock;
	lsock = mk_lsock();
	wait_for_clients(lsock);
}