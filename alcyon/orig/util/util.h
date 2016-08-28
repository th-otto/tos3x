#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#include <cout.h>
#include <ar68.h>

int lgetl PROTO((long *lp, FILE *f));
int lgetw PROTO((unsigned short *lp, FILE *f));
int lputl PROTO((long *lp, FILE *f));
int lputw PROTO((unsigned short *lp, FILE *f));
int getchd PROTO((FILE *fp, struct hdr2 *arptr));
int putchd PROTO((FILE *fp, struct hdr2 *arptr));
int getarhd PROTO((FILE *fp, struct libhdr *arptr));
int putarhd PROTO((FILE *fp, struct libhdr *arptr));

int efseek PROTO((FILE *stream, long offset, int whence));

