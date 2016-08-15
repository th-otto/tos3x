/*****************************************************************************
*
*                   A T A R I  " C"   H E A D E R   F I L E
*                   ---------------------------------------
*       Copyright 1985 by ATARI CORP.  All rights reserved.
*
*       This is the standard include file for the CP/M C Run Time Library.
*
*****************************************************************************/

#ifndef __STDIO_H__
#define __STDIO_H__ 1

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __SYS_TYPES_H__
#include <sys/types.h>
#endif

#ifdef __ALCYON__
#include <varargs.h>
#else
#include <stdarg.h>
#endif

/****************************************************************************
*       Stream I/O File Definitions
*****************************************************************************/
#define BUFSIZ          512             /*      Standard (ascii) buf size   */
#define MAXFILES        16              /*      Max # open files ( < 32 )   */
struct _iobuf {                         /*                                  */
         int _fd;                       /* file descriptor for low level io */
         int _flag;                     /* stream info flags                */
        char *_base;                    /* base of buffer                   */
        char *_ptr;                     /* current r/w pointer              */
         int _cnt;                      /* # chars to be read/have been wrt */
};                                      /*                                  */
#ifndef FILE                            /* conditionally include:           */
extern struct _iobuf _iob[MAXFILES];    /* an array of this info            */
typedef struct _iobuf FILE;             /* stream definition                */
#endif                                  /************************************/
                                        /* flag byte definition             */
#define _IOREAD 0x01                    /* readable file                    */
#define _IOWRT  0x02                    /* writeable file                   */
#define _IOABUF 0x04                    /* alloc'd buffer                   */
#define _IONBUF 0x08                    /* no buffer                        */
#define _IOERR  0x10                    /* error has occurred               */
#define _IOEOF  0x20                    /* EOF has occurred                 */
#define _IOLBUF 0x40                    /* handle as line buffer            */
#define _IOSTRI 0x80                    /* this stream is really a string   */
#define _IOASCI 0x100                   /* this was opened as an ascii file */
                                        /************************************/
#define stdin  (&_iob[0])               /* standard input stream            */
#define stdout (&_iob[1])               /*    "     output  "               */
#define stderr (&_iob[2])               /*    "     error   "               */
                                        /************************************/
#define clearerr(p) ((p)->_flag &= ~_IOERR) /* clear error flag             */
#define feof(p) ((p)->_flag & _IOEOF)   /* EOF encountered on stream        */
#define ferror(p) ((p)->_flag & _IOERR) /* error encountered on stream      */
#define fileno(p) ((p)->_fd)            /* get stream's file descriptor     */
#define getchar() getc(stdin)           /* get char from stdin              */
#define putchar(c) putc(c,stdout)       /* put char to stdout               */
#define putc fputc
#define getc fgetc


/****************************************************************************/
/*                                                                          */
/*                              M A C R O S                                 */
/*                              -----------                                 */
/*                                                                          */
/*      Define some stuff as macros ....                                    */
/*                                                                          */
/****************************************************************************/

#define abs(x)  ((x) < 0 ? -(x) : (x))  /*      Absolute value function     */

#define MAX(x,y)   (((x) > (y)) ? (x) :  (y))   /* Max function             */
#define MIN(x,y)   (((x) < (y)) ? (x) :  (y))   /* Min function             */
#define max(x,y)   (((x) > (y)) ? (x) :  (y))   /* Max function             */
#define min(x,y)   (((x) < (y)) ? (x) :  (y))   /* Min function             */

#define EOF (-1)		/* End of File marker */

#define	NULL	((VOIDPTR)0)			/*	Null pointer value	    */

int printf PROTO((const char *fmt, ...));
int fprintf PROTO((FILE *fp, const char *fmt, ...));
int sprintf PROTO((char *str, const char *fmt, ...));

int vprintf PROTO((const char *fmt, va_list args));
int vfprintf PROTO((FILE *fp, const char *fmt, va_list args));
int vsprintf PROTO((char *str, const char *fmt, va_list args));

int fputc PROTO((int c, FILE *stream));
int fgetc PROTO((FILE *stream));

int fflush PROTO((FILE *stream));

FILE *fopen PROTO((const char * name, const char * mode));

FILE *fopena PROTO((const char * name, const char *mode));
FILE *fopenb PROTO((const char * name, const char *mode));

int open PROTO((const char *pathname, int flags, ...));
int creat PROTO((const char *pathname, mode_t mode));
int close PROTO((int fd));
long lseek PROTO((int fd, long offs, int whence));

long getl PROTO((FILE * sp));
int getw PROTO((FILE * sp));


/*************************** end of stdio.h *********************************/

#endif /* __STDIO_H__ */
