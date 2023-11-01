/*****************************************************************************
*
*                   C P / M   C   H E A D E R   F I L E
*                   -----------------------------------
*       Copyright 1982,83 by Digital Research Inc.  All rights reserved.
*
*       This is the standard include file for the CP/M C Run Time Library.
*
*****************************************************************************/
#ifndef NULLFILE

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#include <portab.h>                     /* Portability Definitions          */
                                        /*                                  */
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

/****************************************************************************
*       Stream I/O File Definitions
*****************************************************************************/
#define BUFSIZ  512                     /*      Standard (ascii) buf size   */
#define MAXFILES        16              /*      Max # open files ( < 32 )   */
struct _iobuf {                         /*                                  */
        WORD _fd;                       /* file descriptor for low level io */
        WORD _flag;                     /* stream info flags                */
        BYTE *_base;                    /* base of buffer                   */
        BYTE *_ptr;                     /* current r/w pointer              */
        WORD _cnt;                      /* # chars to be read/have been wrt */
};                                      /*                                  */
#ifndef FILE                            /* conditionally include:           */
extern struct _iobuf _iob[MAXFILES];    /* an array of this info            */
#define FILE struct _iobuf              /* stream definition                */
#endif                                  /************************************/
#define NULLFILE ((FILE *)0)            /* Null return values               */
                                        /* flag byte definition             */
#define _IOREAD 0x01                    /* readable file                    */
#define _IOWRT  0x02                    /* writeable file                   */
#define _IOABUF 0x04                    /* alloc'd buffer                   */
#define _IONBF  0x08                    /* no buffer                        */
#define _IOFBF  0x00					/* fully buffered                   */
#define _IOERR  0x10                    /* error has occurred               */
#define _IOEOF  0x20                    /* EOF has occurred                 */
#define _IOLBF  0x40                    /* handle as line buffer            */
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

/****************************************************************************/

double atof();
double ceil();
double cos();
double sin();
double exp();
double fabs();
double floor();
double fmod();
double log();
double pow();
double sinh();
double tanh();
double sqrt();
double tan();
double atan();

LONG atol();
LONG ftell();
#ifndef EFGETS
LONG getl();
#endif
LONG lseek();
LONG tell();
LONG putl();

WORD atoi();
WORD brk();

BYTE *sbrk();
VOIDPTR malloc PROTO((unsigned int size));
VOIDPTR realloc PROTO((VOIDPTR ptr, unsigned int size));
VOIDPTR calloc PROTO((unsigned int nmemb, unsigned int size));
BYTE *ftoa();
BYTE *etoa();
BYTE *getpass();
BYTE *gets();
BYTE *fgets();
BYTE *index();
BYTE *rindex();
BYTE *mktemp();
int sprintf PROTO((char *, const char *, ...));
BYTE *strcat();
BYTE *strncat();
BYTE *strcpy();
BYTE *strncpy();
BYTE *ttyname();

FILE *fopen();
FILE *freopen();
FILE *fdopen();
FILE *fopena();
FILE *fopenb();
FILE *freopa();
FILE *freopb();

#endif
