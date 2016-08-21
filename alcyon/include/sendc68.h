/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) sendc68.h - Jul 26, 1983  REGULUS 4.1
*/

/* format of a symbol entry in the main table */

#define OSTSIZE	14	/* symbol table entry length on object file */
					/* without table link */
#define STESIZE	18	/* byte length of symbol table entry */

#define SYNAMLEN	8
struct symtab {
	char name[SYNAMLEN];		/* symbol name */
	short flags;				/* bit flags */
	long  vl1;					/* symbol value */
	char *tlnk;					/* table link */
};

struct symtab *symptr;

/* flags for symbols */
#define SYDF	0x8000		/* defined */
#define SYEQ	0x4000		/* equated */
#define SYGL	0x2000		/* global - entry or external */
#define SYER	0x1000		/* equated register */
#define SYXR	0x0800		/* external reference */
#define SYDA	0x0400		/* DATA based relocatable */
#define SYTX	0x0200		/* TEXT based relocatable */
#define SYBS	0x0100		/* BSS based relocatable */

struct irts {
	char *irle;		/* ptr to last entry in chain */
	char *irfe;		/* ptr to first entry in chain */
};

#ifndef VAX11
struct mlongbytes { short hiword; short loword; };
struct mshortbytes { char lobyte; char hibyte; };
#else
struct mlongbytes { short loword; short hiword; };
struct mshortbytes { char hibyte; char lobyte; };
#endif
union mlong { struct mlongbytes u; long l; };
union mshort { struct mshortbytes u; short l; };

/* relocation bit definitions: */
#define RBMASK		07		/* tells type of relocation */
#define DABS		0		/* data word absolute */
#define DRELOC		1		/* DATA relocatable */
#define TRELOC		2		/* TEXT relocatable */
#define BRELOC		3		/* BSS relocatable */
#define EXTVAR		4		/* ref to external variable */
#define LUPPER		5		/* upper word of long */
#define EXTREL		6		/* relative mode on external variable */
#define INSABS		7		/* first word of instr -- absolute */
