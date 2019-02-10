/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) sendc68.h - Jul 26, 1983  REGULUS 4.1
*/

#ifndef __SENDC68_H__
#define __SENDC68_H__ 1

/* format of a symbol entry in the main table */

#ifndef SYNAMLEN
#define SYNAMLEN	8
#define OSTSIZE	14	/* symbol table entry length on object file */
					/* without table link */
#endif

#define STESIZE	18	/* byte length of symbol table entry */

struct symtab {
	char name[SYNAMLEN];		/* symbol name */
	short flags;				/* bit flags */
	long  vl1;					/* symbol value */
#ifdef LINK68
	short ovlnum;				/* overlay number */
	struct symtab *next; 		/* global table link */
#endif
	struct symtab *tlnk;		/* hash table link */
};

/* flags for symbols */
#define SYDF	0x8000		/* defined */
#define SYEQ	0x4000		/* equated */
#define SYGL	0x2000		/* global - entry or external */
#define SYER	0x1000		/* equated register */
#define SYXR	0x0800		/* external reference */
#define SYDA	0x0400		/* DATA based relocatable */
#define SYTX	0x0200		/* TEXT based relocatable */
#define SYBS	0x0100		/* BSS based relocatable */

/*
 * binutil names of the same values
 */
#define A_UNDF   0x0000      /* Undefined.  */
#define A_BSS    0x0100      /* BSS.  */
#define A_TEXT   0x0200      /* Text segment.  */
#define A_DATA   0x0400      /* Data segment.  */
#define A_EXT    0x0800      /* External.  */
#define A_EQREG  0x1000      /* Equated register.  */
#define A_GLOBL  0x2000      /* Global.  */
#define A_EQU    0x4000      /* Equated (absolute with GNU binutils).  */
#define A_DEF    0x8000      /* Defined.  */

#define A_LNAM   0x0048      /* GST compatible long name.  If the a_type
				member of struct asym/xsym && A_LNAM is
				non-zero it is a long name.  A maximum
				of 14 bytes follow a_value.  */

#define A_TFILE 0x0280       /* Text file name (object module).  */
#define A_TFARC 0x02C0       /* Text file archive. This clashes with A_LNAM.  */

struct irts {
	struct symtab *irle;		/* ptr to last entry in chain */
	struct symtab *irfe;		/* ptr to first entry in chain */
};

/* relocation bit definitions: */
#define RBMASK		07		/* tells type of relocation */
#define DABS		0		/* data word absolute */
#define DRELOC		1		/* DATA relocatable */
#define TRELOC		2		/* TEXT relocatable */
#define BRELOC		3		/* BSS relocatable */
#define EXTVAR		4		/* ref to external variable */
#define LUPPER		5		/* upper word of long */
#define EXTREL		6		/* pc-relative mode on external variable */
#define INSABS		7		/* first word of instr -- absolute */

#endif
