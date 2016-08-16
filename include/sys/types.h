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
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned int
#endif
typedef __SIZE_TYPE__ size_t;

#ifndef __PTRDIFF_TYPE__
#define __PTRDIFF_TYPE__ long
#endif
typedef __PTRDIFF_TYPE__ ptrdiff_t;

typedef long off_t;

#endif
