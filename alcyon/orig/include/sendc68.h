/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) sendc68.h - Jul 26, 1983  REGULUS 4.1
*/

/* format of a symbol entry in the main table */

#define SEEKREL	 1	/* relative seek flag */
#define OSTSIZE	14	/* symbol table entry length on object file */
					/* without table link */
#define STESIZE	18	/*byte length of symbol table entry */

struct symtab {
	char name[8];		/*symbol name */
	short flags;		/*bit flags */
	long  vl1;			/*symbol value */
	char *tlnk;			/*table link */
};

struct symtab *symptr;

/* flags for symbols */
# define SYDF	0100000		/*defined */
# define SYEQ	0040000		/*equated */
# define SYGL	0020000		/*global - entry or external */
# define SYER	0010000		/*equated register */
# define SYXR	0004000		/*external reference */
# define SYDA	0002000		/*DATA based relocatable */
# define SYTX	0001000		/*TEXT based relocatable */
# define SYBS	0000400		/*BSS based relocatable */

struct irts {
	char *irle;		/*ptr to last entry in chain */
	char *irfe;		/*ptr to first entry in chain */
};

#ifndef VAX11
struct mlong { int hiword; int loword; };
struct mshort { char lobyte; char hibyte; };
#else
struct mlong { short loword; short hiword; };
struct mshort { char hibyte; char lobyte; };
#endif

/* relocation bit definitions: */
#define RBMASK		07		/*tells type of relocation */
#define INSABS		7		/*first word of instr -- absolute */
#define DABS		0		/*data word absolute */
#define TRELOC		2		/* TEXT relocatable */
#define DRELOC		1		/* DATA relocatable */
#define BRELOC		3		/* BSS relocatable */
#define EXTVAR		4		/* ref to external variable */
#define LUPPER		5		/* upper word of long */

struct buf{
	int fildes;
	int nunused;
	char *xfree;
	char buff[512];
};

struct buf ibuf;
const char *ifilname;
