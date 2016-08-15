#ifndef __STDLIB_H__
#define __STDLIB_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __SYS_TYPES_H__
#include <sys/types.h>
#endif

VOIDPTR malloc PROTO((size_t size));
VOID free PROTO((VOIDPTR));
VOIDPTR realloc PROTO((VOIDPTR ptr, size_t size));
VOIDPTR calloc PROTO((size_t nmemb, size_t size));

int atoi PROTO((const char *s));

extern	int	rand PROTO((NOTHING));		/* random function			*/
extern	VOID srand PROTO((unsigned int seed));	/* seed random generator		*/

VOID abort PROTO((NOTHING));
VOID _exit PROTO((int status)) __attribute__((noreturn));
VOID exit PROTO((int status)) __attribute__((noreturn));

VOIDPTR sbrk PROTO((ptrdiff_t increment));
int brk PROTO((VOIDPTR addr));

#endif /* __STDLIB_H__ */
