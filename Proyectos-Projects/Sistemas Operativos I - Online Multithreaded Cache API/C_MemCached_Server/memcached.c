#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include "sock.h"
#include "common.h"
#include "parser.h"
#include "tablahash.h"
#include "log.h"

int num_cores;

#define MAX_EVENTS 5
#define TEXTSOCK 0
#define BINSOCK 1

// {PUTS, DELS, GETS}
uint64_t stats[3] = {0,0,0};
// Utilizamos spinlock para stats al ser una región crítica compuesta sólo por
// una operación, lo cual resulta más rápido.
pthread_spinlock_t statsSpinlock;

// Tabla hash concurrente.
TablaHash tablaMem;

int epollfd;

int text_sock, bin_sock;
size_t sizeSockTypeArray;
u_int8_t* sockTypeArr;	// Cada entrada será TEXT_SOCK, o BIN_SOCK
pthread_spinlock_t sockTypeArrMutex;

struct conn_data {
	u_int8_t type : 1;
	int fd;
};

#define MIN(x,y) (x > y ? y : x) 

// Read up to n bytes from fd, storing them in buf
int readSock(int fd, char* buf, int n) {
	int rc = read(fd, buf, n);
	if (rc < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return 0;
	if (rc <= 0)
		return -1;
	return rc;
}

// Macro interna
#define WRITE(fd, buf, n) ({						\
	int wc = write(fd, buf, n);					\
	if (wc < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) { \
		perror("write");              \
		exit(EXIT_FAILURE);           \
	}	\
	if (wc <= 0)	 { \
		perror("write");              \
		exit(EXIT_FAILURE);           \
	}	\
	wc; })

// Append no more than n characters from SRC onto DEST[m], returning the length of the resulting string
int stringcat(char* dest, const char* src, size_t __n, size_t __m) {
	int i = __m;
	for(int j = 0; (j < __n) && src[j] != '\0'; i++, j++)
		dest[i] = src[j];
	return i;
}

// Handling de un comando. Modifica la tablahash.
void text_handle(int fd, char* toks[3], int lens[3], int ntok) {
	char valid_commands[4][6] = {"PUT", "GET", "DEL", "STATS"};
	int valid_lens[4] = {3, 3, 3, 5}; 
	bool valid = false;

	for (int i = 0; i < 4; i++) {
		if(lens[0] != valid_lens[i]) continue;
		if (strncmp(toks[0], valid_commands[i], lens[0]) == 0) {
			valid = true; 				// Se hizo un comando válido
			switch(i) {
				case 0:					// PUT
					// Modificando stats
					pthread_spin_lock(&statsSpinlock);
					
					stats[0]++;
					pthread_spin_unlock(&statsSpinlock);
					if (ntok == 3 && lens[1] != 0 && lens[2] != 0) {
						int res = tablahash_insertar(&tablaMem, toks[1], toks[2], lens[1], lens[2], false);
						if (res == 0) {
							// Sólo incrementar la cantidad de KEYs  si se insertó exitosamente.
							
							WRITE(fd, "OK\n", 3);
						} else if (res == 1) {
							WRITE(fd, "OK\n", 3);
						} else WRITE(fd, "EBIG\n", 5);
					} else valid = false;
					break;
				case 1:					// GET
					// Modificando stats
					pthread_spin_lock(&statsSpinlock);
					stats[2]++;
					pthread_spin_unlock(&statsSpinlock);
					if (ntok == 2 && lens[1] != 0) {
						// Puede ser binaria
						size_t lenResp = 0;
						bool isBinary;
						char* respuesta = tablahash_buscar_y_retornar(&tablaMem, toks[1], lens[1], &lenResp, &isBinary);
						if (isBinary) 
							WRITE(fd, "EBINARY\n", 8);
						else if (respuesta == NULL) 
							WRITE(fd, "ENOTFOUND\n", 10);
						else {
							WRITE(fd, "OK ", 3);
							WRITE(fd, respuesta, lenResp);
							WRITE(fd, "\n", 1);
						}
					} else valid = false;
					break;
				case 2:					// DEL
					// Modificando stats
					pthread_spin_lock(&statsSpinlock);
					stats[1]++;
					pthread_spin_unlock(&statsSpinlock);
					if (ntok == 2 && lens[1] != 0) {
						if (!tablahash_eliminar(&tablaMem, toks[1], lens[1]))
							WRITE(fd, "ENOTFOUND\n", 10);
						else {
							WRITE(fd, "OK\n", 3);
							// Sólo decrementar la cantidad de KEYs  si se insertó exitosamente. 
							
						}
					}
					else valid = false;
					break;
				default:					// STATS
					if (ntok == 1) {
						char buf[256];
						char num[16];
						size_t sz = 8;
						// Se construye la salida de STATS
						strcpy(buf, "OK PUTS=");
						sprintf(num, "%ld", stats[0]);
						sz = stringcat(buf, num, 16, sz);
						sz = stringcat(buf, " DELS=", 8, sz);
						sprintf(num, "%ld", stats[1]);
						sz = stringcat(buf, num, 16, sz);
						sz = stringcat(buf, " GETS=", 7, sz);
						sprintf(num, "%ld", stats[2]);
						sz = stringcat(buf, num, 16, sz);
						sz = stringcat(buf, " KEYS=", 7, sz);
						sprintf(num, "%ld", tablaMem->numElems);
						sz = stringcat(buf, num, 16, sz);
						sz = stringcat(buf, "\n", 1, sz);
						WRITE(fd, buf, sz);
					}
					else valid = false;
					break;
			}
			break;
		}
	}
	if (!valid) WRITE(fd, "EINVALID\n", 9);
}

// 0 todo ok
// -1 error en el read, se bloquea
// -2 comando invalido
int text_consume(int fd) {
	char buf[2049];
	int nread, sobra = 0;
	char *p;
	char *toks[3]= {NULL};
	int lens[3] = {0};
	int ntok, len;
	while ((nread = readSock(fd, buf+sobra, 2048-sobra)) > 0) {
		len = 0;
		char *p0 = buf;
		buf[sobra+nread] = 0;
		// p apunta a la posicion del \n 
		log(3, "Mensaje leido:%s | Fd: %d", buf, fd);
		log(3, "Bytes leidos:%d | Fd: %d", nread, fd);
		while((p = memchr(p0, '\n', nread-len)) != NULL) {
			if (p0 == buf && p == NULL) WRITE(fd, "EBIG\n", 5);
			len = p - buf;
			*p = '\0';
			log(3, "procesando el mensaje %s", p0);
			// text_parser coloca los tokens de buf separados por ' ' en toks,
			// pone la longitud de cada uno en lens, y devuelve la cantidad en ntok
			ntok = text_parser(p0,toks,lens);
			
			if (ntok > 3 || ntok == 0) { 
				WRITE(fd, "EINVALID\n", 9);
				log(3, "Mensaje invalido: %s", p0);
			}
			else text_handle(fd, toks, lens, ntok);
			p0 = p+1;
		}
		sobra = 2048-(p0 - buf);
		memcpy(buf, p0, sobra);
	}
	
	if (nread == -1) return -1;
	return 0;
}

void bin_handle(int fd, uint8_t command, uint32_t sizeKey, u_int8_t* key, uint32_t sizeVal, uint8_t* val) {
	char codigo[1];
	switch(command) {
	case GET: 
			// Modificando stats
			pthread_spin_lock(&statsSpinlock);
			stats[2]++;
			pthread_spin_unlock(&statsSpinlock);
			// Puede ser binaria
			size_t lenResp = 0;
			bool isBinary; 			// Ignorado, utilizado por servidor de texto.
			char* respuesta = tablahash_buscar_y_retornar(&tablaMem, key, sizeKey, &lenResp, &isBinary);

			if (respuesta == NULL) {
				*codigo = ENOTFOUND;
				WRITE(fd, codigo, 1);
			} else {
				*codigo = OK;
				WRITE(fd, codigo, 1);
				WRITE(fd, respuesta, lenResp);
			}	
			break;
	case PUT:
		// Modificando stats
		pthread_spin_lock(&statsSpinlock);
		stats[0]++;
		pthread_spin_unlock(&statsSpinlock);
		int res = tablahash_insertar(&tablaMem, key, val, sizeKey, sizeVal, true);
		if (res == 0) {
			*codigo = OK;
			WRITE(fd, codigo, 1);
		} else if (res == 1) {
			*codigo = OK;
			WRITE(fd, codigo, 1);
		} else {
			*codigo = EBIG;
			WRITE(fd, codigo, 1);
		}
		break;
	case DEL:
		pthread_spin_lock(&statsSpinlock);
		stats[1]++;
		pthread_spin_unlock(&statsSpinlock);
		if (!tablahash_eliminar(&tablaMem, key, sizeKey)) {
			*codigo = ENOTFOUND;
			WRITE(fd, codigo, 1);
		} else {
			*codigo = OK;
			WRITE(fd, codigo, 1);
		}
		break;
	default: ; // command == STATS, asegurado por bin_consume
		char buf[256];
		size_t sz = 0; // Número de bytes escritos
		// Se construye la salida de STATS
		buf[sz++] = OK;
		int ncampos = 4;
		char campos[4][5] = {"PUTS", "DELS", "GETS", "KEYS"};
		int lenCampos[4] = {4,4,4,4};
		uint64_t valores[4] = {stats[0], stats[1], stats[2], tablaMem->numElems};
		uint32_t lenCampo;
		int lenValor;
		for(int i = 0; i < ncampos; i++) {
			lenCampo = lenCampos[i];
			lenValor = sizeof(valores[i]);
			strncpy(buf+sz, (char*)&lenCampo, sizeof(lenCampo));
			sz += sizeof(lenCampo);
			strncpy(buf+sz, campos[i], lenCampo);
			sz += lenCampo;
			strncpy(buf+sz, (char*)&lenValor, sizeof(lenValor));
			sz += sizeof(lenValor);
			strncpy(buf+sz, (char*)&valores[i], lenValor);
			sz += lenValor;
		}
		
		WRITE(fd, buf, sz);
		break;
	}
}

// A lo largo de la función bin_consume, leemos 1 byte de más en cada READ, esto lo hacemos para
// asegurarnos de que el protocolo sea cumplido correctamente y el mensaje sea válido.
// Devuelve -1 si el fd debe ser cerrado, -2 si el mensaje era inválido y -3 si era inválido y debe limpiarse el buffer.
int bin_consume(int fd) {
	char actionChar[1];
	uint8_t action;
	int nread;
	char sizeKeyString[4], sizeValString[4];
	uint32_t sizeKey, sizeVal;
	char* key = NULL;
	while (true) {
		nread = readSock(fd, actionChar, 1);

		if (nread == 0) return 0;
		if (nread == -1) return -1;
		action = *actionChar;
		if (action != STATS && action != PUT && action != GET && action != DEL) {
			log(2, "Action invalida, %d", actionChar);
			return -3;
		}
		if (action == STATS) {
			bin_handle(fd, action, 0, NULL, 0, NULL);
			continue;	// Leemos el siguiente mensaje.
		}
		// Leemos la longitud de la key
				
		nread = readSock(fd, sizeKeyString, 4);
		if (nread < 4) return -3;
		memcpy(&sizeKey, sizeKeyString, 4);
		sizeKey = ntohl(sizeKey);
		// Chequeamos por errores en la key
		if (sizeKey == 0) return -3;
		if (sizeKey > MEMORY_LIM) {
			char codigo[1];
			codigo[0] = EOOM;
			if (write(fd, codigo, 1) == -1) {
				perror("write\n");
				exit(EXIT_FAILURE);
			}
			return -3;
		}
		// Leemos la key
		key = NULL;
		// Liberar esperar espacio hasta que entre la clave
		if ((key = increase_size((void *) key, sizeKey, &tablaMem)) == NULL) {
			char codigo[1];
			codigo[0] = EOOM;
			if (write(fd, codigo, 1) == -1) {
				perror("write\n");
				exit(EXIT_FAILURE);
			}
			return 0;
		}
		nread = 0;
		int nreadaux = 0;
		// Se lee hasta leer todos los bytes esperados
		while (nread < sizeKey) {
			nreadaux = readSock(fd, key+nread, MIN(2048, sizeKey-nread));
			if (nreadaux > 0) nread += nreadaux;
			else { free(key); return -2; }
		}
		
		if(action == GET || action == DEL) {
			bin_handle(fd, action, sizeKey, key, 0, NULL);
			continue;	// Leemos el siguiente mensaje.
		}
		
		nread = readSock(fd, sizeValString, 4);
		if (nread == -1) return -2;
		if (nread < 4) return -3;
		memcpy(&sizeVal, sizeValString, 4);
		sizeVal = ntohl(sizeVal);
		if(sizeVal == 0) return -3;
		if (sizeVal > MEMORY_LIM) {
			char codigo[1];
			codigo[0] = EOOM;
			if (write(fd, codigo, 1) == -1) {
				perror("write\n");
				exit(EXIT_FAILURE);
			}
			return -3;
		}
		char* val = NULL;
		// Liberar esperar espacio hasta que entre el valor
		if ((val = increase_size((void *) val, sizeVal, &tablaMem)) == NULL) {
			char codigo[1];
			codigo[0] = EOOM;
			if (write(fd, codigo, 1) == -1) {
				perror("write\n");
				exit(EXIT_FAILURE);
			}
			free(key);
			return -3;
		}
		nreadaux = 0;
		nread = 0;
		// Se lee hasta leer todos los bytes esperados
		while (nread < sizeVal) { 
			// Se intent leer un byte extra para comprobar que el mensaje está bien formado
			nreadaux = readSock(fd, val+nread, MIN(2048, sizeVal-nread)); 
			
			if (nreadaux > 0) nread += nreadaux;
			else {
				free(key);
				free(val);
				return -2;
			}
			
		}
		log(3, "Binary PUT received, key size: %u, value size: %u", sizeKey, sizeVal);
		bin_handle(fd, action, sizeKey, key, sizeVal, val);
	}
}

void handle_conn_text(int fd, struct epoll_event* ev) {
	/* Enable TCP_CORK option on 'sockfd' - subsequent TCP output is corked
	until this option is disabled. */
	int optval = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) != 0)
		quit("setsockopt: TCP_CORK");
	int retval = text_consume(fd);
	optval = 0;
	if (setsockopt(fd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) != 0)
		quit("setsockopt: TCP_CORK");
	if (retval == -1) {
		close(fd);
		free(ev->data.ptr);
	}
	else {
		ev->events = EPOLLIN | EPOLLONESHOT | EPOLLET;
		((struct conn_data*) (ev->data.ptr))->fd = fd;
		epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, ev);
	}
}

void handle_conn_bin(int fd, struct epoll_event* ev) {
	/* Enable TCP_CORK option on 'sockfd' - subsequent TCP output is corked
	until this option is disabled. */
	int optval = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) != 0)
		quit("setsockopt: TCP_CORK");
	int retval = bin_consume(fd);
	/* Disable TCP_CORK option on 'sockfd' - corked output is now transmitted
	in a single TCP segment. */
	optval = 0;
	if (setsockopt(fd, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval)) != 0)
		quit("setsockopt: TCP_CORK");
	if (retval == -1) {
		close(fd);
		free(ev->data.ptr);
	}
	else {
		// Se bloqueó el READ, mensaje inválido o incompleto.
		if (retval == -2) {
			char msg[1];
			msg[0] = EINVALID;
			WRITE(fd, msg, 1);
		}
		else if (retval == -3) {
			char msg[1];
			msg[0] = EINVALID;
			WRITE(fd, msg, 1);
			//limpiamos el fd;
			char buffer[2048];
			int nread = 1;
			while (nread > 0)
			nread = readSock(fd, buffer, 2048);
		}

		ev->events = EPOLLIN | EPOLLONESHOT | EPOLLET;
		epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, ev);
	}
	
}

// Limita la memoria del servidor.
void limit_mem() {
	if (MEMORY_LIM < 70 * MB) {
		puts("WARNING: Memory Limit set unadvisably low, server may malfunction.\n\
At least 70 MB are recommended for optimal operation.");
	}
	struct rlimit limit;

    // Establecer un nuevo límite
    limit.rlim_cur = MEMORY_LIM;  // Establecer el soft limit
    limit.rlim_max = MEMORY_LIM;  // Establecer el hard limit

	if (setrlimit(RLIMIT_DATA , &limit) == -1) {
        perror("setrlimit");
        exit(1);
    }
}

// Captura y maneja las señales de tipo SIGINT
void handle_sigint(int signal) {
	puts("Terminating...");
	tablahash_destruir(tablaMem);
	pthread_spin_destroy(&statsSpinlock);
	pthread_spin_destroy(&sockTypeArrMutex);
	free(sockTypeArr);
	log_free();
	close(epollfd);
	exit(EXIT_SUCCESS);
	// Es posible que quede memoria sin liberar al momento de terminar el programa, alocada en bin_consume.
	// Sin embargo, asegurarse de que no hayan leaks aumentaría demasiado la complejidad del código,
	// y el sistema operativo puede hacerse cargo de hacerlo cuando el proceso muera, por lo que no es considerado.
}

// Espera a las conexiones de clientes
void* server_threads(void* nprocArg) {
	int nproc = nprocArg - (void *)0;
	int eventFd;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	int nEvents = 0;
	while (1) {
		nEvents = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nEvents == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for(int i = 0; i < nEvents; i++) {
			eventFd = events[i].data.fd;//Y esto?? pq ya no tenemos más esta cosa, tenemos la struct en el ptr...
			if (eventFd == text_sock || eventFd == bin_sock) { // Nuevo cliente se intenta conectar
				log(3, "nuevo cliente, protocolo: %d", eventFd);
				int conn_sock = accept(eventFd, NULL, NULL);
				if (conn_sock == -1) {
					perror("accept");
					exit(EXIT_FAILURE);
				}
				ev.events = EPOLLIN | EPOLLONESHOT;
				ev.data.fd = eventFd;
				epoll_ctl(epollfd, EPOLL_CTL_MOD, eventFd, &ev);

				// Haciendo el fd no bloqueante
				set_non_blocking(conn_sock);
				ev.events = EPOLLIN | EPOLLONESHOT | EPOLLET;

				struct conn_data* evData = NULL;
				if ((evData = increase_size((void *) evData, sizeof (struct conn_data), &tablaMem)) == NULL) {
					if (eventFd == text_sock) {
						if (write(conn_sock, "EOOM", 1) == -1) {
							perror("write\n");
							exit(EXIT_FAILURE);
						}
					}
					if (eventFd == bin_sock) {
						char codigo[1];
						codigo[0] = EOOM;
						if (write(conn_sock, codigo, 1) == -1) {
							perror("write\n");
							exit(EXIT_FAILURE);
						}
					}
					close(conn_sock);
				}
				evData->fd = conn_sock;
				if (eventFd == text_sock) evData->type = TEXTSOCK;
				else 					  evData->type = BINSOCK; 
				ev.data.ptr = evData;
				// Añadimos el fd del cliente nuevo a la lista de interés
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
					perror("epoll_ctl: conn_sock");
					exit(EXIT_FAILURE);
				}
 				
			} else {
				struct conn_data* evData = events[i].data.ptr;
				log(3, "nuevo pedido, FD: %d", evData->fd);

				if (evData->type == TEXTSOCK) {
					handle_conn_text(evData->fd, &events[i]); 	 // HANDLECON PARA TEXTO
				}
				else if (evData->type == BINSOCK) {
					handle_conn_bin(evData->fd, &events[i]); 	   // HANDLECON PARA BINARIOS
				}
			}
		}
	}
	return NULL;
}

// Inicia el servidor.
void server() {
	struct epoll_event ev;

	epollfd = epoll_create1(0);
	if (epollfd == -1) {
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN | EPOLLONESHOT;
	ev.data.fd = text_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, text_sock, &ev) == -1) {
		perror("epoll_ctl: text_sock");
		exit(EXIT_FAILURE);
	}

	ev.data.fd = bin_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, bin_sock, &ev) == -1) {
		perror("epoll_ctl: bin_sock");
		exit(EXIT_FAILURE);
	}

	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);

	// Set thread inheritance to PTHREAD_EXPLICIT_SCHED
	pthread_attr_setinheritsched(&threadAttr, PTHREAD_EXPLICIT_SCHED);

	pthread_t threads[num_cores-1]; 
	for (long int i = 1; i < num_cores; i++) {
		int ret = pthread_create(&(threads[i-1]), 0, server_threads, (void *) i);
		if (ret == 0)
			log(1, "Thread %d creado", i);
		else {
			log(1, "Thread %d error número %d al crear", i, ret);
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}
	printf("Created %d threads, server ready.\n", num_cores);
	server_threads((void *) 0);
}

int main(int argc, char **argv) {
	// Initializing logging
	__loglevel = 4;

	puts("Initializing server...");
	num_cores = sysconf(_SC_NPROCESSORS_ONLN);

	sizeSockTypeArray = 256;
	sockTypeArr = malloc(sizeSockTypeArray * sizeof(u_int8_t));
	
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, handle_sigint);

	/*Función que limita la memoria*/
	limit_mem();

	text_sock = mk_tcp_sock(mc_lport_text);
	if (text_sock < 0)
		quit("mk_tcp_sock.text");

	bin_sock = mk_tcp_sock(mc_lport_bin);
	if (bin_sock < 0)
		quit("mk_tcp_sock.bin");

	puts("Listening sockets successfully created.");

	size_t capacidadTabla = 1000000;
	int cantLocks = capacidadTabla / 10000;
	tablaMem = tablahash_crear(capacidadTabla, cantLocks);
	puts("Key searching interface created, starting server up...");

	pthread_spin_init(&statsSpinlock, 0);
	pthread_spin_init(&(sockTypeArrMutex), 0);

	server();

	return 0;
}
