#ifndef __FCNTL_H__
#define __FCNTL_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __SYS_TYPES_H__
#include <sys/types.h>
#endif

/* File access modes for `open' and `fcntl'.  */
#ifndef O_RDONLY
#define	O_RDONLY	0x00	/* Open read-only.  */
#define	O_WRONLY	0x01	/* Open write-only.  */
#define	O_RDWR		0x02	/* Open read/write.  */

/* Mask for file access modes.  This is system-dependent in case
   some system ever wants to define some other flavor of access.  */
#define	O_ACCMODE	(O_RDONLY|O_WRONLY|O_RDWR)
#endif

/*
 * non-standard functions
 */
int opena PROTO((const char *, int, ...));
int openb PROTO((const char *, int, ...));
int creata PROTO((const char *pathname, mode_t mode));
int creatb PROTO((const char *pathname, mode_t mode));

#endif /* __FCNTL_H__ */
