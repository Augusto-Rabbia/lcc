#ifndef __COMMON_H
#define __COMMON_H 1

#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>

#define MB 1048576
#define MEMORY_LIM 150 * MB  // Set memory lim
#define MAX_CLIENTS 65536
enum code {
	PUT = 11,
	DEL = 12,
	GET = 13,

	STATS = 21,

	OK = 101,
	EINVALID = 111,
	ENOTFOUND = 112,
	EBINARY = 113,
	EBIG = 114,
	EUNK = 115,
	EOOM = 116,
};

static const in_port_t mc_lport_text = 8888;
static const in_port_t mc_lport_bin  = 8889;

static inline void quit(char *s) {
	perror(s);
	exit(EXIT_FAILURE);
}

#endif