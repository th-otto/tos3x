#ifndef __STDLIB_H__
#define __STDLIB_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif

int atoi PROTO((const char *s));

extern	int	rand PROTO((NOTHING));		/* random function			*/
extern	VOID srand PROTO((unsigned int seed));	/* seed random generator		*/

VOID abort PROTO((NOTHING));
VOID _exit PROTO((int status));

#endif /* __STDLIB_H__ */
