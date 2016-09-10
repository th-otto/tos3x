/*  pgmld.c - program load						*/
/*  xpgmld								*/


#include "tos.h"
#include "fs.h"
#include "bios.h"
#include "mem.h"
#include <toserrno.h>
#include "pghdr.h"
#include "btools.h"


ERROR pgmld01 PROTO((FH h, PD *pdptr));
ERROR pgfix01 PROTO((int32_t nrelbytes, PGMINFO *pi));

/*
 *  xpgmld -
 *
 */

ERROR xpgmld(P(const char *) s, P(PD *) p)
PP(const char *s;)								/*  program name            */
PP(PD *p;)									/*  ptr to PD               */
				/*  program space follows PD        */
{
	ERROR r;
	FH h;
	int16_t magic;

	if ((r = xopen(s, 0)) < 0L)			/*  open file for read  */
		return r;

	h = (int) r;						/*  get file handle */

	if ((r = xread(h, 2L, &magic)) < 0L)	/*  read magic nbr  */
		return r;

	/*
	  *  the following switch statement will allow us to call different
	  *  strategies for loading different types of files
	 */

	switch (magic)
	{
	case 0x0601a:
		r = pgmld01(h, p);
		break;
	default:
		r = E_PLFMT;
	}

	xclose(h);
	return r;
}



/*
 *  lastcp - used to keep track of the code ptr betwee pgmld01 and pgfix01
 */

static char *lastcp;


/*
 *  pgmld01 - oldest known gemdos load format - very similar to cp/m 68k
 *	load in the (open) program file with handle 'h' using load file
 *	strategy like cp/m 68k.  Specifically:
 *
 *		read in program header and determine format parameters
 *		seek past the symbol table to the start of the relo info
 *  		read in the first offset (it's different than the rest in that 
 *		  it is a longword instead of a byte).
 *  		make the first adjustment
 *  		until we run out of relocation info or we have an error
 *			read in relocation info into the bss area
 *			call pgfix01() to fix up the code using that info
 *		zero out the bss
 */

ERROR pgmld01(P(FH) h, P(PD *) pdptr)
PP(FH h;)
PP(PD *pdptr;)
{
	register PGMHDR01 *hd;
	register PGMINFO *pi;
	register PD *p;
	PGMHDR01 hdr;
	PGMINFO pinfo;
	char *cp;
	int32_t relst;
	int32_t flen;
	ERROR r;

	hd = &hdr;
	pi = &pinfo;
	p = pdptr;
	relst = 0;

	/*
	 *  read in the program header 
	 */

	if ((r = xread(h, (int32_t) sizeof(PGMHDR01), &hdr)) < 0L)
		return r;

	/*
	 *  calculate program load info
	 */

	flen = (pi->pi_tlen = hd->h01_tlen) + (pi->pi_dlen = hd->h01_dlen);
	pi->pi_blen = hd->h01_blen;
	pi->pi_slen = hd->h01_slen;
	pi->pi_tpalen = p->p_hitpa - p->p_lowtpa - sizeof(PD);
	pi->pi_tbase = (char *) (p + 1);	/*  1st byte after PD   */
	pi->pi_bbase = pi->pi_tbase + flen;
	pi->pi_dbase = pi->pi_tbase + pi->pi_tlen;

	/*
	 *  see if there is enough room to load in the file, then see if
	 *  the requested bss space is larger than the space we have to offer
	 */

	if (flen > pi->pi_tpalen || pi->pi_tpalen - flen < pi->pi_blen)
		return E_NSMEM;

	/*
	 *  initialize PD fields
	 */

	bmove((char *) &pi->pi_tbase, (char *) &p->p_tbase, 6 * sizeof(int32_t));

	/*  
	 *  read in the program file (text and data)
	 *  if there is an error reading in the file or if it is an abs
	 * file, then we are finished  
	 */

	if ((r = xread(h, flen, pi->pi_tbase)) < 0)
		return r;

	if (hd->h01_abs)
		return E_OK;				/*  do we need to clr bss here? */

	/*  
	 *  if not an absolute format, position past the symbols and start the 
	 * reloc pointer  (flen is tlen + dlen).  NOTE that relst is 
	 * init'd to 0, so if the format is absolute, we will not drop
	 * into the fixup code.
	 */

	if (!hd->h01_abs)
	{
		/* should change hard coded 0x1c */
		if ((r = xlseek(flen + pi->pi_slen + 0x1c, h, 0)) < 0L)
			return r;

		if ((r = xread(h, (long) sizeof(relst), &relst)) < 0L)
			return r;
	}

	if (relst != 0)
	{
		cp = pi->pi_tbase + relst;

		/*  make sure we didn't wrap memory or overrun the bss  */

		if (cp < pi->pi_tbase || cp >= pi->pi_bbase)
			return E_PLFMT;

		*((int32_t *) (cp)) += (long) pi->pi_tbase;	/*  1st fixup     */

		lastcp = cp;					/*  for pgfix01() */

		flen = p->p_hitpa - (int32_t)pi->pi_bbase;

		for (;;)
		{								/*  read in more relocation info  */
			if ((r = xread(h, flen, pi->pi_bbase)) <= 0)
				break;
			/*  do fixups using that info  */
			if ((r = pgfix01(r, pi)) <= 0)
				break;
		}

		if (r < 0)
			return r;
	}

	/*  zero out the bss  */

	if (pi->pi_blen != 0)
	{
		*pi->pi_bbase = 0;
		if (pi->pi_blen > 1)
			lbmove(pi->pi_bbase, pi->pi_bbase + 1, pi->pi_blen - 1);
	}

	return E_OK;
}



/*
 *  pgfix01 - do the next set of fixups
 *
 *  returns:
 *	addr of last modified longword in code segment (cp)
 *	0 if error or done
 *	stat01:
 *		>0: all offsets in bss used up, read in more
 *		=0: offset of 0 encountered, no more fixups
 *		<0: EPLFMT (load file format error)
 */


ERROR pgfix01(P(int32_t) nrelbytes, P(PGMINFO *) pi)
PP(int32_t nrelbytes;)							/*  nbr of avail rel values */
PP(PGMINFO *pi;)							/*  program info pointer    */
{
	register char *cp;						/*  code pointer        */
	register char *rp;						/*  relocation info pointer */
	register int32_t n;						/*  nbr of relocation bytes */
	register char *bbase;					/*  base addr of bss segment    */
	register int32_t tbase;					/*  base addr of text segment   */

	cp = lastcp;
	rp = pi->pi_bbase;
	n = nrelbytes;
	tbase = (int32_t) pi->pi_tbase;
	bbase = pi->pi_bbase;

	while (n-- && *rp != 0)
	{
		if (*rp == 1)
			cp += 0xfe;
		else
		{
			/*  get the byte at rp, don't sign ext, add to cp  */
			cp += 0x00ff & (int32_t) (*rp);
			if (cp >= bbase)
			{
				return E_PLFMT;
			}
			*((int32_t *) (cp)) += tbase;

		}
		++rp;
	}

	lastcp = cp;						/*  save code pointer       */
	return ++n == 0 ? TRUE : FALSE;
}
