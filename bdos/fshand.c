/*  fshand.c - file handle routines for the file system			*/



#include	"tos.h"
#include	"fs.h"
#include	"bios.h"
#include	<toserrno.h>




/*
 *  xforce -
 *
 *	Function 0x46	Fforce
 *
 *	Error returns
 *		EIHNDL
 *
 *	Last modified	SCC	5 Apr 85
 */

/* 306: 00e16f5e */
ERROR xforce(P(int16_t) std, P(int16_t) h)
PP(int16_t std;)
PP(int16_t h;)
{
	return ixforce(std, h, run);
}

/*
 *  ixforce - force a std handle to a non-std handle.
 *	if the std handle is for an open non-char device, close it
 */

long ixforce(P(int16_t) std, P(int16_t) h, P(PD *) p)
PP(int16_t std;)								/* std must be a standard handle    */
PP(int16_t h;)									/* h   must NOT be a standard handle    */
PP(PD *p;)
{
	long fh;

	if ((std < 0) || (std >= NUMSTD))
		return E_IHNDL;

	if (p->p_uft[std] > 0)
		xclose(std);

	if (h < 0)
		p->p_uft[std] = h;
	else
	{
		if (h < NUMSTD)
			return E_IHNDL;

		if ((fh = (long) sft[h - NUMSTD].f_ofd) < 0L)
		{
			p->p_uft[std] = fh;
		} else
		{
			p->p_uft[std] = h;
			sft[h - NUMSTD].f_use++;
		}
	}
	return E_OK;
}



/*
 *  syshnd -
 */

/* 306: 00e16e68 */
int16_t syshnd(P(int16_t) h)
PP(register int16_t h;)
{
	if (h >= NUMSTD || (h = run->p_uft[h]) > 0)
		return h - NUMSTD;

	return h;
}



/*
 *  ixdirdup -
 */

VOID ixdirdup(P(int16_t) h, P(int16_t) dn, P(PD *) p)
PP(int16_t h;)									/* file handle              */
PP(int16_t dn;)									/* directory number         */
PP(PD *p;)
{
	p->p_curdir[h] = dn;
	diruse[dn]++;
}


/*
 *  dup -
 *	duplicate a file handle.
 *
 *	Function 0x45	Fdup
 *
 *	Error returns
 *		EIHNDL
 *		ENHNDL
 *
 *	Last modified	SCC	 5 Apr 85
 */

/* 306: 00e16eda */
ERROR xdup(P(int16_t) h)
PP(int16_t h;)									/*+ h must be a standard handle (checked) */
{
	register FH i;
	register FTAB *p;
	
	if (h < 0 || h >= NUMSTD)
		return E_IHNDL;				/* only dup standard */

	if ((i = ffhndl()) < 0)
		return E_NHNDL;
	
	p = &sft[i];
	p->f_own = run;

	if ((h = run->p_uft[h]) > 0)
		p->f_ofd = sft[h - NUMSTD].f_ofd;
	else
		p->f_ofd = (OFD *)(long)h; /* hmpf */

	p->f_use = 1;

	return i + NUMSTD;
}
