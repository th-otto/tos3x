/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

/* reduce long relative branches to short if possible */

#include "as68.h"
#include <fcntl.h>

VOID fixsyadr PROTO((int al));


VOID pass1a(NOTHING)
{
	register long reduced;
	register int writfn, i, wsize;

	pitix = &itbuf[ITBSZ];
	reduced = itoffset = 0L;
	stbuf[0].itrl = 0;
	wsize = 3 * sizeof(struct it);				/* don't calculate many times */
	close(itfn);
	LASTCHTFN = itfnc;
	itfn = openfi(tfilname, O_RDONLY, 1);		/* open it for reading */
	writfn = open(tfilname, O_WRONLY, 1);		/* may need to rewrite some of it */
	if (writfn < 0)
		asabort();
	while (1)
	{
		ristb();						/* read it for one statement */
		opcpt = stbuf[2].itop.ptrw2;	/* ptr to opcode entry in main tab */
		if (!(opcpt->flags & OPDR))
		{								/* not a directive */
			format = opcpt->flags & OPFF;
			p1inlen = stbuf[1].itrl;	/* pass 1 instr length guess */
			if (((format == 6 && p1inlen == 4) || opcpt == jsrptr) && (rlflg = stbuf[3].itrl) == TEXT)
			{
				nite = stbuf[0].itrl & 0xff;	/* # of it entries */
				pnite = &stbuf[nite];	/* ptr to end of stmt */
				modelen = stbuf[2].itrl;	/* instr mode length */
				opdix = ITOP1;			/* first operand */
				pitw = &stbuf[ITOP1];	/* ptr to first operand */
				loctr = stbuf[3].itop.l - reduced;
				expr(p2gi);
				ival.l -= loctr + 2L;
				if (itype == ITCN && !extflg && reloc != ABS)
				{
					if (format == 9)
					{
						/* jsr */
						i = (ival.l >= -128 && ival.l <= 127) ? p1inlen - 2 :
							(ival.l >= -32768 && ival.l <= 32767) ? p1inlen - 4 : 0;
						if (!i)
							continue;
						stbuf[2].itop.ptrw2 = bsrptr;	/* chng to bsr */
					} else if (ival.l >= -128 && ival.l <= 127)
					{
						if (ival.l == 2)	/* branch to next instr - delete */
							i = 4;
						else			/* reduce long branch to short */
							i = 2;
					} else
						continue;
					fixsyadr(i);
					reduced += i;
					stbuf[1].itrl -= i;	/* reduced instr lenght somewhat */
					if (!stbuf[1].itrl)
						stbuf[1].itrl = -1;	/* ignore flag */
					if (lseek(writfn, itoffset, SEEK_SET) == -1L)
					{
						rpterr("seek error on intermediate file\n");
						asabort();
					}
					if (write(writfn, &stbuf[0], wsize) != wsize)
					{
						rpterr("write error on it file\n");
						asabort();
					}
				}
			}
		} else if (opcpt == endptr)
		{
			savelc[TEXT] -= reduced;
			close(writfn);
			return;
		}
	}
}


/*
 * fix all symbol addresses that are text based and greater than loctr
 * fix means subtract al from them
 */
VOID fixsyadr(P(int) al)
PP(int al;)
{
	register struct symtab **sx1, **sx2;
	register short l;

	l = al;
	/* loop thru symbol initial reference table */
	for (sx1 = sirt; sx1 < &sirt[SZIRT - 1]; sx1 += 2)
	{
		if (*(sx2 = sx1 + 1) == 0)		/* this chain is empty */
			continue;

		/* symbols on one chain */
		sx2 = (struct symtab **)*sx2;						/* first entry on this chain */
		while (1)
		{
			if (((((struct symtab *)sx2)->flags & SYDF) || (((struct symtab *)sx2)->flags & SYPC)) &&
				(((struct symtab *)sx2)->flags & SYRO) &&
				((struct symtab *)sx2)->vl1.l > loctr)
			{
				((struct symtab *)sx2)->vl1.l -= l;
			}
			if (((struct symtab *)sx2) == *sx1)			/* end of chain */
				break;
			sx2 = (struct symtab **)(((struct symtab *)sx2)->tlnk);			/* next entry in chain */
		}
	}
}
