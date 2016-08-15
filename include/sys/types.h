#ifndef __SYS_TYPES_H__
#define __SYS_TYPES_H__ 1

typedef int mode_t;

/*
 * not quite right:
 * pointer subtraction yields long result,
 * but unsigned long is not implemented.
 * Also, several library function like malloc() and read()
 * take only unsigned int as argument.
 */
typedef unsigned int size_t;

typedef long ptrdiff_t;

#endif
