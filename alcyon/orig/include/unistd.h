#ifndef __UNISTD_H__
#define __UNISTD_H__

#ifndef __COMPILER_H__
#include <compiler.h>
#endif

/* Values for the second argument to access.
   These may be OR'd together.  */
#ifndef F_OK
#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */
#endif

int isatty PROTO((int fd));
int isdev PROTO((int fd));
char *ttyname PROTO((int fd));
int dup PROTO((int oldfd));
int dup2 PROTO((int oldfd, int newfd));
int access PROTO((const char *fname, int mode));
int chown PROTO((const char *name, uid_t owner, gid_t group));

#endif /* __UNISTD_H__ */
