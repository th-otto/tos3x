
#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

int lgetl PROTO((long *lp, FILE *f));
int lgetw PROTO((short *lp, FILE *f));
int getchd PROTO((FILE *fp, struct hdr2 *arptr));
