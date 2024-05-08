#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
//#include <netinet/ip.h>

#define PUERTO 3942
#define MAX_CLIENTES 10

int memfd;

void quit(char* error) { perror(error); exit(EXIT_FAILURE); }
/*void setnonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}*/

int crear_lsock(int puerto) {
    struct sockaddr_in sa;
    int lsock;

    /* Primero, creamos el socket */
    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock == -1)
        quit("socket");
    
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

/* Devuelve la longitud de la linea */
int fd_readline(int fd, char *buf) {
	int rc;
	int i = 0;

	/*
	 * Leemos de a un caracter hasta completar una línea.
	 */
	while ((rc = read(fd, buf + i, 1)) > 0) {
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc == -1)
		quit("read");

	buf[i] = 0;
	return i;
}

void handle_connection(int csock) {
    char buf[200];


    // Atendemos pedidos, uno por linea
    if (fd_readline(csock, buf) == 0) {
        // linea vacia, se cerró la conexión
        close(csock);
        return;
    }
    char instruction[4];
    strncpy(instruction, buf, 3);
    instruction[3] = '\0';
    buf[3] = '\0';
    if (strcmp(buf, "PUT") == 0) {
        write(memfd, buf+4, strlen(buf+4));
        lseek(memfd, 0, SEEK_SET);
        write(csock, "OK", 3);
    } else if (strcmp(buf, "DEL") == 0) {
        
        // Buscamos el token k t lo eliminamos
        lseek(memfd, 0, SEEK_SET);
        write(csock, "OK", 3);
    } else if (strcmp(buf, "GET") == 0) {
        memmove(buf, buf+4, strlen(buf+4));
        int buflen = strlen(buf);
        char membuf[256];
        while (fd_readline(memfd, membuf) != 0)
            if (strncmp(membuf, buf, buflen) == 0) {
                strcpy(buf, membuf+buflen); 
                break;
            }

        lseek(memfd, 0, SEEK_SET);
        char token[256]; strcpy(token, "OK "); strcat(token, buf);
        write(csock, token, strlen(token));
    } 
    else if (strcmp(buf, "BYE") == 0) 
        close(csock);
    else write(csock, "Invalid command", 16);
}

/* wait for clients con epoll */
void wait_clients_epoll(int lsock) {
    struct epoll_event ev, connEvs[MAX_CLIENTES];
    // nFds representa a la cantidad de fds listos para operaciones I/O
    int connSock, nFds, epollfd;

    // Creamos la instancia de epoll
    epollfd = epoll_create1(0);
    if (epollfd == -1) 
        quit("epoll_create1");
    
    // Especificamos la configuración del lsock.
    ev.events = (EPOLLIN | EPOLLOUT);       // Queremos que el lsock pueda leer y escribir en socket.
    ev.data.fd = lsock;                //   Setteamos para que el socket del evento de escucha
    //                                      Sea el lsock
    /*  Añadimos el evento a la instancia epoll
        EPOLL_CTL_ADD decir que estamos añadiendo el fd, en vez de MOD o DEL. 
        El tercer argumento debe coincidir con lev.data.fd, y sirve para 
        verificar que esté correcto. */
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, lsock, &ev) == -1)
        quit("epoll_ctl: lsock");
    
    // Ahora, empezamos a buscar atender clientes.
    while (1) {
        /*  Esperamos a que ocurra algún evento en #epollfd#. Cuando pase, nFds será 
            la cantidadde eventos que ocurrieron.
            debemos especificar que #events# tiene tamaño #MAX_CLIENTES#, y rechazará
            eventos si llegaran a aparecer más de 10 en una única instancia de epoll_wait
            #timeout# = -1 => esperamos indefinidamente */
        nFds = epoll_wait(epollfd, connEvs, MAX_CLIENTES, -1);
        /*  Al terminar epoll_wait, connEvs será modificado y cada evento encontrado 
            tendrá al lsock en su ev.data.fd*/
        for (int i = 0; i < nFds; i++) {
            if (connEvs[i].data.fd == lsock) {  // Encontramos un cliente queriendo conectarse
                connSock = accept(lsock, NULL, NULL);
                if (connSock == -1)
                    quit("accept");
                setnonblocking(connSock);
                /*  Como ya añadimos el evento del lsock a la instancia epoll, podemos reutilizar ev
                    Añadimos el  */
                ev.events = (EPOLLIN | EPOLLOUT);
                ev.data.fd = connSock;
                //  Añadimos el evento a la instancia epoll
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connSock, &ev) == -1)
                    quit("epoll_ctl: connSock");
            } else
                handle_connection(connEvs[i].data.fd); // Manejamos la conexión, con connSock = connEvs[i].data.fd
        }
    }
}

void wait_for_clients(int lsock) {
    int connectionsock;
    /* Esperamos una conexión, de cualquier tipo u origen */
    connectionsock = accept(lsock, NULL, NULL);

    /* Creamos un proceso para manejar la conexión */
    __pid_t pid = fork();
    if (pid == 0) 
        handle_connection(connectionsock);
    else
        wait_for_clients(lsock);
}

int main() {
    int lsock = crear_lsock(PUERTO);
    memfd = open("svmem", O_CREAT | O_RDWR);
    wait_for_clients(lsock);
    return 0;
}