#ifndef __VARARGS_H__
#define __VARARGS_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif

#ifdef __STDARG_H__

#undef va_start
#undef va_arg
#undef va_end

#else

#ifndef __VA_LIST
#define __VA_LIST 1
typedef char *va_list;
#endif

#endif


#define va_dcl va_list va_alist;
#define va_start(list) ((list) = (va_list)&va_alist)
#define va_end(list)
#define va_arg(ap, type)    \
    ((sizeof(type) == 1) ? \
    (*(type *)((ap += 2) - 1)) : \
    (*((type *)(ap))++))

#endif /* __VARARGS_H__ */
