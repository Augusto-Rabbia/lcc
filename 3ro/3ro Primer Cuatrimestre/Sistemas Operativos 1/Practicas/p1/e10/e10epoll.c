#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/epoll.h>

#define MAX_CLIENTES 10

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

void handle_connection(int csock) {
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

/* wait for clients con epoll */
void wait_clients_epoll(int lsock) {
    struct epoll_event ev, connEvs[MAX_CLIENTES];
    /* nFds representa a la cantidad de fds listos para operaciones I/O */
    int connSock, nFds, epollfd;

    /* Creamos la instancia de epoll */
    epollfd = epoll_create1(0);
    if (epollfd == -1) 
        quit("epoll_create1");
    
    /* Especificamos la configuración del lsock. */
    ev.events = EPOLLIN;//(EPOLLIN | EPOLLOUT);       // Queremos que el lsock pueda leer y escribir en socket.
    ev.data.fd = lsock;                //   Setteamos para que el socket del evento de escucha
    //                                      Sea el lsock
    /*  Añadimos el evento a la instancia epoll
        EPOLL_CTL_ADD decir que estamos añadiendo el fd, en vez de MOD o DEL. 
        El tercer argumento debe coincidir con lev.data.fd, y sirve para 
        verificar que esté correcto. */
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, lsock, &ev) == -1)
        quit("epoll_ctl: lsock");
    
    /* Ahora, empezamos a buscar atender clientes. */
    while (1) {
        /*  Esperamos a que ocurra algún evento en #epollfd#. Cuando pase, nFds será 
            la cantidadde eventos que ocurrieron.
            debemos especificar que #events# tiene tamaño #MAX_CLIENTES#, y rechazará
            eventos si llegaran a aparecer más de 10 en una única instancia de epoll_wait
            #timeout# = -1 => esperamos indefinidamente */
        nFds = epoll_wait(epollfd, connEvs, MAX_CLIENTES, -1);
		if (nFds == -1)
			quit("epoll_wait");
        /*  Al terminar epoll_wait, connEvs será modificado y cada evento encontrado 
            tendrá al lsock en su ev.data.fd*/
        for (int i = 0; i < nFds; i++) {
            if (connEvs[i].data.fd == lsock) {  // Encontramos un cliente queriendo conectarse
                connSock = accept(lsock, NULL, NULL);
                if (connSock == -1)
                    quit("accept");
                
                /*  Como ya añadimos el evento del lsock a la instancia epoll, podemos reutilizar ev
                    Añadimos el  */
                ev.events = EPOLLIN;//(EPOLLIN | EPOLLOUT);
                ev.data.fd = connSock;
                /*  Añadimos el evento a la instancia epoll */
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connSock, &ev) == -1)
                    quit("epoll_ctl: connSock");
            } else
                handle_connection(connEvs[i].data.fd); // Manejamos la conexión, con connSock = connEvs[i].data.fd
        }
    }
}

/* Crea un socket de escucha en puerto 4040 TCP */
int crear_lsock(int puerto) {
    struct sockaddr_in sa;
    int lsock;
	int yes = 1;

    /* Primero, creamos el socket */
    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock == -1)
        quit("socket");

	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");
    
    /* Creamos el socket address para bindear el lsocket */
    sa.sin_family = AF_INET;           // Para que escuche ipv4
    sa.sin_port = htons(puerto);       // Definimos el puerto
    sa.sin_addr.s_addr = htonl(INADDR_ANY);   // Que se bindee a todas las direcciones posibles en el puerto.

    /* Bindeamos el socket con la configuración que determinamos */
    if (bind(lsock, (struct sockaddr *) &sa, sizeof(sa)) == -1)
        quit("bind");
    
    /* Setteamos el socket en modo escucha, y especificamos que sólo pueden
       haber hasta 10 conexiones pendientes a la vez. Las próximas serán rechazadas*/
    if (listen(lsock, 10) == -1)
        quit("listen");
    
    return lsock;
}

int main() {
	turnos = open("turnos.txt", O_RDWR | O_CREAT);
	write(turnos, "0\n", 2);
	lseek(turnos, 0, SEEK_SET);
	int lsock;
	lsock = crear_lsock(4040);
	wait_clients_epoll(lsock);
}