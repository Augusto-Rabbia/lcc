#ifndef __TIMING_H
#define __TIMING_H

/*
 * La última versión de este archivo siempre está en:
 * https://raw.githubusercontent.com/mtzguido/timing.h/master/timing.h
 */

#include <time.h>
#include <stdio.h>

static inline float __tdiff(struct timespec t1, struct timespec t2)
{
	return (t2.tv_sec - t1.tv_sec)
		+ 1e-9 * (t2.tv_nsec - t1.tv_nsec);
}

/* Setear a 0 para no ver el parallel factor */
static int time_par = 1;

#define __TIME(s, expr, v) ({						\
		struct timespec __t1p, __t2p;				\
		struct timespec __t1w, __t2w;				\
		typeof(expr) ret;					\
		float *__vv = v;					\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &__t1p);	\
		clock_gettime(CLOCK_REALTIME, &__t1w);			\
		ret = expr;						\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &__t2p);	\
		clock_gettime(CLOCK_REALTIME, &__t2w);			\
		fprintf(stderr, "TIME %-25s \tCPU=%.5fs\twall=%.5fs",	\
				s,					\
				__tdiff(__t1p, __t2p),			\
				__tdiff(__t1w, __t2w));			\
		if (time_par)						\
			fprintf(stderr, "\t parF = %.2f",		\
					__tdiff(__t1p, __t2p)/		\
					__tdiff(__t1w, __t2w));		\
		fprintf(stderr, "\n");					\
		if (__vv)						\
			*__vv = __tdiff(__t1w, __t2w);			\
		ret;})

/*
 * Dada una expresión [expr] y un puntero a float [v],
 * imprime el tiempo que tardó en computar la expresión
 * y lo guarda en v (si v != NULL).
 */
#define TIME(expr, v) __TIME(#expr, expr, v)

/* Similar pero para statements. */
#define TIME_void(stmt, v) __TIME(#stmt, (stmt, 1), v)

#define __TIMEREP(s, n, expr, v)		\
	__TIME(s,				\
		({				\
		int __i;			\
		int __n = n;			\
		typeof(expr) __r;		\
		for (__i = 0; __i < __n; ++__i)	\
			__r = expr;		\
		__r; }), v )

/*
 * Similar a TIME pero repite la computación [n] veces.
 * El tiempo *total* se guarda en [v].
 */
#define TIMEREP(n, expr, v) __TIMEREP(#expr, n, expr, v)

/* Similar a TIMEREP, para statements. */
#define TIMEREP_void(n, stmt, v) __TIMEREP(#stmt, n, (stmt, 1), v)

#endif