#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __SYS_TYPES_H__
#include <sys/types.h>
#endif

int chmod PROTO((const char *path, mode_t mode));

#endif
