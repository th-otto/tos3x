#ifndef __UNISTD_H__
#define __UNISTD_H__

#ifndef __COMPILER_H__
#include <compiler.h>
#endif

int isatty PROTO((int fd));
int isdev PROTO((int fd));
char *ttyname PROTO((int fd));

#endif /* __UNISTD_H__ */
