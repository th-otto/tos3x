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

extern  char	*getpass PROTO((const char *prompt));	/* get password function	*/
extern	char	*index PROTO((const char *str, int c));	/* locate first occurence	*/
extern	char	*rindex PROTO((const char *str, int c));	/* locate last occurence	*/
extern	char	*mktemp PROTO((char *template));	/* create temp file name	*/
extern	char	*strcat PROTO((char *dst, const char *src));	/* concat whole			*/
extern	char	*strncat PROTO((char *dst, const char *src, int len));	/* concat number specified.	*/
extern	char	*strcpy PROTO((char *dst, const char *src));	/* copy whole string		*/
extern	char	*strncpy PROTO((char *dst, const char *src, int len));	/* copy specified length	*/
extern	int	strlen PROTO((const char *str));	/* return length of string	*/

#define strchr index
#define strrchr rindex

#endif /* __STRING_H__ */
