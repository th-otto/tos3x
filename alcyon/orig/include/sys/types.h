#ifndef __SYS_TYPES_H__
#define __SYS_TYPES_H__ 1

typedef int mode_t;
typedef int pid_t;
typedef int uid_t;
typedef int gid_t;

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

#ifndef __WCHAR_TYPE__
#define __WCHAR_TYPE__ unsigned short int
#endif
typedef __WCHAR_TYPE__ wchar_t;

#ifndef __INT8_TYPE__
#define __INT8_TYPE__ signed char
#endif
typedef __INT8_TYPE__ __int8_t;

#ifndef __UINT8_TYPE__
#ifdef __ALCYON__
#define __UINT8_TYPE__ char
#else
#define __UINT8_TYPE__ unsigned char
#endif
#endif
typedef __UINT8_TYPE__ __uint8_t;

#ifndef __INT16_TYPE__
#define __INT16_TYPE__ signed short
#endif
typedef __INT16_TYPE__ __int16_t;

#ifndef __UINT16_TYPE__
#define __UINT16_TYPE__ unsigned short
#endif
typedef __UINT16_TYPE__ __uint16_t;

#ifndef __INT32_TYPE__
#ifdef __MSHORT__
#define __INT32_TYPE__ signed long
#else
#define __INT32_TYPE__ signed int
#endif
#endif
typedef __INT32_TYPE__ __int32_t;

#ifndef __UINT32_TYPE__
#ifdef __ALCYON___
#define __UINT32_TYPE__ long
#else
#ifdef __MSHORT__
#define __UINT32_TYPE__ unsigned long
#else
#define __UINT32_TYPE__ unsigned int
#endif
#endif
#endif
typedef __UINT32_TYPE__ __uint32_t;

typedef long off_t;

#endif
