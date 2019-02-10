#ifndef __ALCYON__
#ifndef __GNUC__
# ifndef __PUREC__
#   ifndef __TURBOC__
#     ifndef LATTICE
#       define __ALCYON__ 1
#     endif
#   endif
# endif
#endif
#endif

#ifndef PROTO
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
#endif

#ifndef UNUSED
#  ifdef __ALCYON__
#    define UNUSED(x)
#  else
#    define UNUSED(x) ((void)(x))
#  endif
#endif

#ifndef VOID
#define VOID	void					/* Void function return	   */
#endif

#ifndef VOIDPTR
#define VOIDPTR	void *
#endif

#include <cout.h>
#include <ar68.h>

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

int lgetl PROTO((int32_t *lp, FILE *f));
int lgetw PROTO((unsigned short *lp, FILE *f));
int getchd PROTO((FILE *fp, struct hdr2 *arptr));
int putchd PROTO((FILE *fp, struct hdr2 *arptr));
int getarhd PROTO((FILE *fp, struct libhdr *arptr));
int putarhd PROTO((FILE *fp, struct libhdr *arptr));
int lputw PROTO((unsigned short *wp, FILE *f));
int lputl PROTO((int32_t *lp, FILE *f));
