#ifndef __STDARG_H__
#define __STDARG_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif

#ifdef __GNUC__
#include_next <stdarg.h>
#else

#ifndef __VA_LIST
#define __VA_LIST 1
typedef char *va_list;
#endif


/* Define the standard macros for the user,
   if this invocation was from the user program.  */
#define va_start(ap, parmN) ((ap) = (va_list)...)
#define va_arg(ap, type)    \
    ((sizeof(type) == 1) ? \
    (*(type *)((ap += 2) - 1)) : \
    (*((type *)(ap))++))
#define va_end(ap)

#endif

#endif
