#ifndef MEM_H
#define MEM_H 1

/*  mem.h - header file for memory and process management routines	*/


/* allowed values for Mxalloc mode: */
#define MX_STRAM 0
#define MX_TTRAM 1
#define MX_PREFSTRAM 2
#define MX_PREFTTRAM 3

/*
 *  externals
 */

extern MPB pmd;
extern int16_t osmem[];
extern int16_t osmlen;
extern VOIDPTR root[20];
extern int16_t osmptr;
extern PD ospd;

VOID xminit PROTO((NOTHING));
VOID osminit PROTO((NOTHING));
VOID ofdadd PROTO((FOFD *buf, long len));
MD *ffit PROTO((long amount, MPB *mp, int16_t mode));
VOID freeit PROTO((MD *m, MPB *mp));
VOID xmakeres PROTO((PD *p));
VOID xmfreall PROTO((PD *r));
VOID xmsetown PROTO((VOIDPTR addr, PD *own));
MD *mgetmd PROTO((NOTHING));
OFD *mgetofd PROTO((NOTHING));
VOID foldermsg PROTO((NOTHING));
OFD *oftdel PROTO((OFD *ofd));
VOIDPTR xmdfree PROTO((MD *m));
DND *fsgetofd PROTO((NOTHING));

/*
 *  process management
 */

extern xjmp_buf bakbuf;
extern int16_t supstk[];

#endif
