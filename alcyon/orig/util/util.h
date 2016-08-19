#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#include <cout.h>
#include <ar68.h>

int lgetl PROTO((long *lp, FILE *f));
int lgetw PROTO((unsigned short *lp, FILE *f));
int getchd PROTO((FILE *fp, struct hdr2 *arptr));
int getarhd PROTO((FILE *fp, struct libhdr *arptr));
