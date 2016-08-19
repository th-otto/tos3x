#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#ifndef __GNUC__
# ifndef __PUREC__
#   ifndef __TURBOC__
#     ifndef LATTICE
#       define __ALCYON__ 1
#     endif
#   endif
# endif
#endif

#ifdef __ALCYON__
#define VOID						/* Void function return	   */
#define VOIDPTR char *
#define NOTHING                     /* no parameters */
#define PROTO(p) ()
#define P(t)
#define PP(v) v
#define volatile
#define const
#else
#define NOTHING void                /* no parameters */
#define PROTO(p) p
#define P(t) t
#define PP(v)
#endif

#ifndef VOID
#define VOID	void					/* Void function return	   */
#endif

#ifndef VOIDPTR
#define VOIDPTR	void *
#endif

#include <cout.h>
#include <ar68.h>

int lgetl PROTO((long *lp, FILE *f));
int lgetw PROTO((unsigned short *lp, FILE *f));
int getchd PROTO((FILE *fp, struct hdr2 *arptr));
int getarhd PROTO((FILE *fp, struct libhdr *arptr));
