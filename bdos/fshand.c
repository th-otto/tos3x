/*  fshand.c - file handle routines for the file system			*/

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include <toserrno.h>




/*
 *  syshnd -
 */

/* 306: 00e16e68 */
FH syshnd(P(FH) h)
PP(register FH h;)
{
	if (h >= NUMSTD || (h = run->p_uft[h]) > 0)
		return h - NUMSTD;

	return h;
}



/*
 *  getofd -
 */
/* 306: 00e16e98 */
OFD *getofd(P(FH) h)
PP(FH h;)
{
	if (h < 0 || h >= OPNFILES)
		return NULL;
	h = syshnd(h);
	if (h < 0)
		return NULL;
	return sft[h].f_ofd;
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

/* 306: 00e16f74 */
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
 *  xclose - Close a file.
 *
 *	Function 0x3E	Fclose
 *
 *	Error returns
 *		EIHNDL
 *		ixclose()
 *
 *	SCC:	I have added 'rc' to allow return of status from ixclose().  I 
 *		do not yet know whether it is appropriate to perform the 
 *		operations inside the 'if' statement following the invocation 
 *		of ixclose(), but I am leaving the flow of control intact.
 */

/* 306: 00e17024 */
ERROR xclose(P(FH) h)
PP(int h;)
{
	int h0;
	OFD *fd;
	ERROR rc;

	if (h < 0)
		return E_OK;					/* always a good close on a character device */

	if (h > (OPNFILES + NUMSTD - 1))
		return E_IHNDL;

	if ((h0 = h) < NUMSTD)
	{
		h = run->p_uft[h];
		run->p_uft[h0] = 0;				/* mark std dev as not in use */
		if (h <= 0)
			return E_OK;
	} else if (((long) sft[h - NUMSTD].f_ofd) < 0L)
	{
		if (!(--sft[h - NUMSTD].f_use))
		{
			sft[h - NUMSTD].f_ofd = 0;
			sft[h - NUMSTD].f_own = 0;
		}
		return E_OK;
	}

	if (!(fd = getofd(h)))
		return E_IHNDL;

	rc = ixclose(fd, 0);

	if (!(--sft[h - NUMSTD].f_use))
		sftdel(&sft[h - NUMSTD]);

	return rc;
}


/*
 * [1]	We play games here (thanx, Jason).  The ixwrite() call will essentially
 *	copy the time, date, cluster, and length fields from the OFD of the
 *	(dir) file we are closeing to the FCB for this (dir) file in the 
 *	parent dir.  The fileln field of this dir is thus set to 0.  But if 
 *	this is a directory we are closing (path & CL_DIR), shouldn't the 
 *	fileln be zero anyway?  I give up.
 *					- ktb
 */



/*
 *  xread -
 *	read 'len' bytes  from handle 'h'
 *
 *	Function 0x3F	Fread
 *
 *	Error returns
 *		EIHNDL
 *		bios()
 *
 *	Last modified	SCC	8 Apr 85
 */

/* 306: 00e170f0 */
ERROR xread(P(int16_t) h, P(long) len, P(VOIDPTR) ubufr)
PP(int16_t h;)
PP(long len;)
PP(VOIDPTR ubufr;)
{
	register OFD *p;

	if ((p = getofd(h)) != NULL)
	{
		if (len == 0)
			return 0;
		return ixread(p, len, ubufr);
	}
	
	return E_IHNDL;
}


/*
 *  xwrite -
 *	write 'len' bytes to handle 'h'.
 *
 *	Function 0x40	Fwrite
 *
 *	Error returns
 *		EIHNDL
 *		bios()
 *
 *	Last modified	SCC	10 Apr 85
 */

/* 306: 00e17130 */
ERROR xwrite(P(FH) h, P(long) len, P(VOIDPTR) ubufr)
PP(FH h;)
PP(long len;)
PP(VOIDPTR ubufr;)
{
	register OFD *p;

	if ((p = getofd(h)) != NULL)
	{
		if (len == 0)
			return 0;
		p->o_flag |= O_DIRTY;
#if 0
		/* WTF? why has this been disabled? */
		/* Make sure not read only. */
		if (p->o_mod == RO_MODE)
			return E_ACCDN;
#endif
		return ixwrite(p, len, ubufr);
	}

	return E_IHNDL;
}


/*
 *  opnfil - does the real work in opening a file
 *
 *	Error returns
 *		ENHNDL
 *
 *	NOTES:
 *		make a pointer to the ith entry of sft 
 *		make i a register int.
 */

/* 306: 00e17176 */
ERROR opnfil(P(const FCB *) f, P(DND *) dn, P(int16_t) mod)
PP(const FCB *f;)
PP(DND *dn;)
PP(int16_t mod;)
{
	register int i;
	int h;

	/* find free sft handle */
	if ((i = ffhndl()) < 0)
		return E_NHNDL;

	sft[i].f_own = run;
	sft[i].f_use = 1;
	h = i + NUMSTD;

	return makopn(f, dn, h, mod);
}



/* 306: 00e171d8 */
FH ffhndl(NOTHING)
{
	register int i;
	register FTAB *p;
	
	for (i = 0, p = sft; i < (OPNFILES - NUMSTD); i++, p++)
		if (p->f_own == NULL)
			return i;
	return -1;
}
