/************************************************************************/
/*									*/
/*	string.h	written by John Feagans		16 Apr 86	*/
/*	copyright 1986 by Atari Corp.					*/
/*									*/
/*	for use with Alcyon C as distributed in ST developers package.	*/
/*									*/
/************************************************************************/

#ifndef __STRING_H__
#define __STRING_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __SYS_TYPES_H__
#include <sys/types.h>
#endif

char *index PROTO((const char *str, int c));	/* locate first occurence	*/
char *rindex PROTO((const char *str, int c));	/* locate last occurence	*/
char *mktemp PROTO((char *template));	/* create temp file name	*/
char *strcat PROTO((char *dst, const char *src));	/* concat whole			*/
char *strncat PROTO((char *dst, const char *src, size_t len));	/* concat number specified.	*/
char *strcpy PROTO((char *dst, const char *src));	/* copy whole string		*/
char *strncpy PROTO((char *dst, const char *src, size_t len));	/* copy specified length	*/
int strlen PROTO((const char *str));	/* return length of string	*/
int strcmp PROTO((const char *, const char *));
int strcasecmp PROTO((const char *, const char *));

#define strchr index
#define strrchr rindex

char *strlwr PROTO ((char *str));
char *strupr PROTO ((char *str));

VOIDPTR memset PROTO((VOIDPTR ptr, int c, size_t bytes));

#endif /* __STRING_H__ */
