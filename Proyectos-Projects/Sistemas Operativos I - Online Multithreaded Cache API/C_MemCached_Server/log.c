#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sys/syscall.h>
#define gettid() (syscall(SYS_gettid))

#include "log.h"

int __loglevel = 3;

static FILE *logFp = NULL;
static pthread_mutex_t logLk = PTHREAD_MUTEX_INITIALIZER;

void log_f(char *file, int line, char *msg, ...)
{
	va_list l;
	char fstr[256];
	time_t t_st;
	char timebuf[26];
	
	pthread_mutex_lock(&logLk);
	if (logFp == NULL) {
		char fname[100];
		mkdir("logs", S_IRWXU | S_IRWXG | S_IRWXO);
		sprintf(fname, "logs/log_file_%i", getpid());
		logFp = fopen(fname, "a");
		fprintf(logFp, "------------------------------------------------\n");
	}
	va_start(l, msg);
	
	if (time(&t_st) != -1) {
		ctime_r(&t_st, timebuf);
		timebuf[strlen(timebuf)-1] = 0; /* sacar \n */
	} else {
		strcpy(timebuf, "????????");
	}
	sprintf(fstr, "%s - Th:%li - %s:%i - %s\n", timebuf, gettid()-getpid(), file, line, msg);
	vfprintf(logFp, fstr, l);
	fflush(logFp);
	
	pthread_mutex_unlock(&logLk);

	va_end(l);
}

void log_free() { fclose(logFp); }