#ifndef MEM_H
#define MEM_H 1

/*  mem.h - header file for memory and process management routines	*/


/*
 *  conditional compile switches
 */

#define	OSMPANIC	FALSE
#define	OSMLIST		FALSE


/*
 *  externals
 */

extern	MPB	pmd;
extern	int16_t	osmem[];
extern	int16_t	osmlen;
extern	VOIDPTR root[20];
extern	int16_t	osmptr;

MD *ffit PROTO((long amount, MPB *mp));
VOID freeit PROTO((MD *m, MPB *mp));

/*
 *  process management
 */

extern xjmp_buf bakbuf;
extern int16_t supstk[];

#endif
