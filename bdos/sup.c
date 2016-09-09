/*	sup.c - GEMDOS main function dispatcher				*/

#define	DBGOSIF	0


/*
** 
** 
** Originally written by JSL as noted below.
** 
** MODIFICATION HISTORY
** 
** 	10 Mar 85	SCC	Added normal handling of functions 3-5, 7, 8
** 				and redirection of functions 3-8.
** 
** 	11 Mar 85	JSL	Modified calling convention for xexec().
** 
** 	11 Mar 85	SCC	Added handling of extended console functions
** 				16-19.
** 
** 	13 Mar 85	SCC	Added handling of other hard errors from disk.
** 				THIS IS ONLY A ROUGH FIRST CUT!
** 
** 			SCC	Added check of return from getbpb() in hard
** 				error processing.
** 
** 	14 Mar 85	SCC	Added xgetver() function at 0x30.
** 
** 	18 Mar 85	SCC	Added x0term() for function 0x00.
** 
** 	19 Mar 85	SCC	Version 0.3.
** 
** 	21 Mar 85	SCC	Added initialization of stdaux and stdprn to
** 				cinit().
** 
** 	25 Mar 85	SCC	Added get/set date/time routines.
** 
** 	 1 Apr 85	SCC	Version 0.4.
** 				Changed reference to getch() to x7in().
** 
** 	10 Apr 85	SCC	Modified osif() to return EINVFN for functions
** 				greater than 0x57.
** 
** 				Added external definition of setjmp() to return
** 				long.
** 
** 				Modified osif() to return long.
** 
** 				Modified ni() to return EINVFN.
** 
** 			EWF	Added in the typeahead buffer initializations.
** 
** 	11 Apr 85	SCC	Added xfreset() function at 0x0D.
** 
** 				Installed EWF's changes of 10 Apr 85.
** 
** 	12 Apr 85	EWF	Modified typeahead buffer initialization to use
** 				KBBUFSZ.
** 
** 			SCC	Installed EWF's change.
** 
** 				Modified osif() to return EIHNDL.
** 
** 	14 Apr 85	SCC	Backed out modifications of 11 Apr 85 for
** 				xfreset().
** 
** 				The function is removed.  It was not the right
** 				fix for the problem it was intended to solve
** 				(see note in the CLI about the ^C problem.
** 
** 	17 Apr 85	SCC	Version 0.5.
** 
** 	22 Apr 85	SCC	Changed reference to external time and date
** 				variables to unsigned in an attempt to correct
** 				the code in tikfrk().
** 
** 	26 Apr 85	SCC	Version 0.6.
** 
** 	29 Apr 85	SCC	Version 0.7.
** 
** 	 7 May 85	SCC	Version 0.8.
** 
** 				Modified tikfrk()'s date handling.
** 
** 	13 May 85	SCC	Version 0.9.
** 
** 	15 May 85	SCC	Version 0.A.
** 
** 	16 May 85	SCC	Modified osif()'s handling of function 0x0A 
**				when stdin is re-directed to a file.
** 
** 				Version 0.B.
** 
** 	24 May 85	SCC	Version 0.C.
** 
** 	28 May 85	SCC	Version 0.D.
** 
** 	 3 Jun 85	SCC	Modified osif() to return int16_t rather than 
**				int32_t -1, -2 and -3 for f_open()'s on CON:, 
**				AUX:, and PRN:.
** 
** 				Version 0.E.
** 
** 	11 Jun 85	SCC	Modified osif()'s handling of device (negative)
**				handles vs. standard handles.
** 
** 				Version 0.F.
** 
**	29 Jun 85	LTG	Modified osif to return ENSMEM if log fails.
** 
**				Added log() to table of external functions 
**				returning long.
** 
**				Bumped LENOSM from 2000 -> 3000.
**     
**				Version 0.10.
** 
** 	23 Jul 85	SCC	Version 0.11.
** 
** 				LTG previously changed LENOSM from 3000 -> 2000.
** 
** 	24 Jul 85	SCC	Version 0.12.
** 
** 	25 Jul 85	SCC	Version 0.13.
** 
** 	05 Aug 85	KTB	Version 0.14.
** 
** 	06 Aug 85	LTG	Version 0.15.
** 
** 	09 Aug 85	SCC	Moved ncmps() here from MEM.C.  It was only used
** 				here.  Also added use of uc() in compare loop.
** 
** 				Version 0.16.
** 
** 				Added external reference and jump table entries
** 				for F_IOCtl(), S_SetVec(), S_GetVec().
** 
** 				Changed character devices supported by F_Open()
**				to NUL:, PRN:, AUX:, CON:, CLOCK:, MOUSE:.
** 
** 				Changed character devices supported by F_Read()
**				and F_Write().
** 
** 				Modified cinit()'s initialization of standard
** 				handles.
** 
** 				Included S_SetVec() and S_GetVec().
** 
** 				Removed extern definition of errbuf[], since
** 				it is defined in this source.
** 
** 	11 Aug 85	SCC	Added 'extern int bios_dev[];' for HXFORM().
**
**	18 Aug 85	SCC	Modified osif() for NUL: handling.
**
**				VERSION 01.00
**
**				Added version string.
**
**	30 Aug 85	KTB	M01.01.01:  drive logout fix.
**
**	30 Aug 85	KTB	M01.01.02:  version name and number.
**
**	15 Oct 85	KTB	M01.01.03: accomodate split of fs.h into
**				fs.h and bios.h
** 
**	21 Oct 85	KTB	M01.01.04: fs.h now has common external
**				declarations; changes are made here to delete
**				duplicate declarations.
**
**	21 Oct 85	KTB	M01.01.05:  now includes portab.h
**
**	05 Nov 85	KTB	M01.01.06:  cleared diruse entry in freetree.
**
**	05 Nov 85	KTB	debug modifications to osif
**
**	07 Nov 85	KTB	took out & in front of array and function names.
**
**	13 Nov 85	SCC	M01.01.07: console output char call being re-
**				directed to a file.
**
**	22 Jan 86	KTB/SCC	M01.01.08: fix to tabout call.
**
**	28 May 86	KTB	M01.01.0528.01: sup doesn't check for valid
**				std. file handles; needs to as lower routines
**				assume that the file handle is valid and
**				indexes with it.
**
**	 7 Oct 86	scc	M01.01.1007.01:  cast some pointers to long
**				before comparing them with 0.
**
**	24 Oct 86	scc	M01.01.1024.01:  check return of getbpb() for long
**				negative error code (as well as zero) in setjmp section
**				error handling.
**
**	29 Oct 86	scc	M01.01.1029.01:  check return of bconout in F_Write
**				handling for character devices in osif().
**
**	 4 Nov 86	scc	M01.01.1104.01:  bumped up the value of LENOSM from
**				2000 to 4000 to reduce the likelyhood of reaching the
**				limit on DNDs.
**
**	16 Sep 87	ACH	cinit() now calls the new BIOS functions to
**				initialise the internal date and time variables
**				from the real time clock (if supported).
**
** NOTES
** 
** 	osif()
** 
** 		TBA note on 'do real hard error stuff'
** 
** 		Toggle/inquire processor/stack state is being done at a higher
** 		level.  This may be changed in the future.
** 
** 		See note on 13 Mar 85 change above.
** 
** NAMES
** 
** 	EWF	Eric W. Fleischman
** 	JSL	Jason S. Loveman
** 	SCC	Steven C. Cavender
**	LTG	Louis T. Garavaglia
** 	KTB	Karl T. Braun (kral)
**	ACH	Anthony C. Hay (DR UK)
** 
*/

#include	"tos.h"
#include	"fs.h"
#include	"bios.h"
#include	<toserror.h>

/*
**  local constants
*/

#define LENOSM 4000



/*
**  forward declarations
*/

VOID cinit PROTO((NOTHING));
ERROR ni PROTO((NOTHING));
int32_t xgetver PROTO((NOTHING));
int32_t S_SetVec PROTO((int16_t n, int32_t address));
int32_t S_GetVec PROTO((int16_t n));

int ncmps PROTO((int n, const char *s, const char *d));
VOID freetree PROTO((DND *d));
VOID offree PROTO((DMD *d));
int32_t osif PROTO((int16_t *));
VOID tikfrk PROTO((int n));


/*
**  globals
*/

int oscnt;

long uptime;

int msec;

xjmp_buf errbuf;

int const nday[] =
/*0 J  F  M  A  M  J  J  A  S  O  N  D  */
{
	0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

MPB pmd;

int osmlen;

int16_t osmem[LENOSM];



/*
**  FND - Function Descriptor
**	each entry in the function table (below) consists of the address of
**	the function which corresponds to the function number, and a function
**	type.
*/

#define FND struct _fnd
FND
{
	int32_t (*fncall) PROTO ((int16_t, ...));
	int16_t fntyp;
};

/*
**  funcs - table of os functions, indexed by function number
**	each entry is for an FND structure.
**	the function 'ni' is used as the address for functions not implemented.
*/

FND funcs[0x58] = {

	/* 0x00 */ { x0term, 0 },

	/* 
	 **  console functions 
	 ** on these functions, the 0x80 flag indicates std file used 
	 ** 0x80 is std in, 0x81 is stdout, 0x82 is stdaux, 0x83 stdprn 
	 */

	/* 0x01 */ { xconin, 0x80 },
	/* 0x02 */ { xtabout, 0x81 },
	/* 0x03 */ { xauxin, 0x82 },
	
	/* 0x04 */ { xauxout, 0x82 },
	/* 0x05 */ { xprtout, 0x83 },
	/* 0x06 */ { rawconio, 0 },
	
	/* 0x07 */ { x7in, 0x80 },
	/* 0x08 */ { x8in, 0x80 },
	/* 0x09 */ { xprt_line, 0x81 },
	
	/* 0x0A */ { readline, 0x80 },
	/* 0x0B */ { xconstat, 0x80 },
	
	/* 
	 * disk functions 
	 * on these functions the 0x80 flag indicates whether a handle 
	 * is required, the low bits represent the parameter ordering, 
	 * as usual. 
	 */

	/* 0x0c */ { ni, 0 },
	/* 0x0d */ { ni, 0 },

	/* 0x0E */ { xsetdrv, 0 },
	/* 0x0f */ { ni, 0 },

	/* 
	 **  extended console functions 
	 ** on these functions, the 0x80 flag indicates 
	 ** std file used, as above 
	 */

	/* 0x10 */ { xconostat, 0x81 },
	/* 0x11 */ { xprtostat, 0x83 },
	/* 0x12 */ { xauxistat, 0x82 },
	/* 0x13 */ { xauxostat, 0x82 },

	/* 0x14 */ { ni, 0 },
	/* 0x15 */ { ni, 0 },
	/* 0x16 */ { ni, 0 },
	/* 0x17 */ { ni, 0 },
	/* 0x18 */ { ni, 0 },

	/* 0x19 */ { xgetdrv, 0 },
	/* 0x1A */ { xsetdta, 1 },
	
	/* 0x1b */ { ni, 0 },
	/* 0x1c */ { ni, 0 },
	/* 0x1d */ { ni, 0 },
	/* 0x1e */ { ni, 0 },
	/* 0x1f */ { ni, 0 },

	/* 0x20 */ { ni, 0 }, /* xgsps */
	
	/* 0x21 */ { ni, 0 },
	/* 0x22 */ { ni, 0 },
	/* 0x23 */ { ni, 0 },
	/* 0x24 */ { ni, 0 },

	/* 0x25 */ { S_SetVec, 1 },

	/* 0x26 */ { ni, 0 },
	/* 0x27 */ { ni, 0 },
	/* 0x28 */ { ni, 0 },
	/* 0x29 */ { ni, 0 },

	/* 0x2A */ { xgetdate, 0 },
	/* 0x2B */ { xsetdate, 0 },
	/* 0x2C */ { xgettime, 0 },
	
	/* 0x2D */ { xsettime, 0 },
	/* 0x2e */ { ni, 0 },
	/* 0x2F */ { xgetdta, 0 },
	
	/* 0x30 */ { xgetver, 0 },
	/* 0x31 */ { xtermres, 1 },
	/* 0x32 */ { ni, 0 },
	/* 0x33 */ { ni, 0 },
	/* 0x34 */ { ni, 0 },

	/* 0x35 */ { S_GetVec, 0 },
	/* 0x36 */ { xgetfree, 1 },
	/* 0x37 */ { ni, 0 },
	/* 0x38 */ { ni, 0 },

	/* 0x39 */ { xmkdir, 1 },
	/* 0x3A */ { xrmdir, 1 },
	/* 0x3B */ { xchdir, 1 },
	
	/* xclose will handle its own redirection */

	/* 0x3C */ { xcreat, 1 },
	/* 0x3D */ { xopen, 1 },
	/* 0x3E */ { xclose, 0x0 },
	
	/* 0x3F */ { xread, 0x82 },

	/* 0x40 */ { xwrite, 0x82 },
	/* 0x41 */ { xunlink, 1 },
	/* 0x42 */ { xlseek, 0x81 },

	/* 0x43 */ { xchmod, 1 },
	/* 0x44 */ { F_IOCtl, 2 },
	/* 0x45 */ { xdup, 0 },
	/* 0x46 */ { xforce, 0 },
	
	/* 0x47 */ { xgetdir, 1 },
	/* 0x48 */ { xmalloc, 1 },
	/* 0x49 */ { xmfree, 1 },

	/* 0x4A */ { xsetblk, 2 },
	/* 0x4B */ { xexec, 3 },
	/* 0x4C */ { xterm, 0 },
	/* 0x4d */ { ni, 0 0,

	/* 0x4E */ { xsfirst, 1 },
	/* 0x4F */ { xsnext, 0 },

	/* 0x50 */ { ni, 0 },
	/* 0x51 */ { ni, 0 },
	/* 0x52 */ { ni, 0 },
	/* 0x53 */ { ni, 0 },
	/* 0x54 */ { ni, 0 },
	/* 0x55 */ { ni, 0 },

	/* 0x56 */ { xrename, 2 },
	/* 0x57 */ { xgsdtof, 1 }
};


/************************************************************************/

char *bdosver = "GEMDOS Version 01.KTB";

/************************************************************************/



/*
**  xgetver -
**	return current version number
*/

in32_t xgetver(NOTHING)
{
	return (0x0101L);					/*  minor.major */
}


/*
**  ni -
*/

ERROR ni(NOTHING)
{
	return (EINVFN);
}




/*
**  cinit - C part of osinit().
*/

VOID cinit(NOTHING)
{
	getmpb(&pmd);
	osmlen = LENOSM;
	run = MGET(PD);

	/* set up system initial standard handles */

	run->p_uft[0] = H_Console;			/* stdin    =   con:    */
	run->p_uft[1] = H_Console;			/* stdout   =   con:    */
	run->p_uft[2] = H_Console;			/* stderr   =   con:    */
	run->p_uft[3] = H_Aux;				/* stdaux   =   aux:    */
	run->p_uft[4] = H_Print;			/* stdprn   =   prn:    */

	add[0] = remove[0] = add[1] = remove[1] = add[2] = remove[2] = 0;

	date_time(GET_DATE, &date);			/* allow bios to initialise date and */
	date_time(GET_TIME, &time);			/* time from hardware, if supported */
}

/*
**  ncmps -  compare two text strings, ignoring case.
*/

int ncmps(P(int) n, P(const char *) s, P(const char *) d)
PP(int n;)
PP(const char *s;)
PP(const char *d;)
{
	while (n--)
		if (uc(*s++) != uc(*d++))
			return (0);

	return (1);
}


/*
 *  freetree -  free the directory node tree
 */

VOID freetree(P(DND *)d)
PP(DND *d;)
{
	int i;

	if (d->d_left)
		freetree(d->d_left);
	if (d->d_right)
		freetree(d->d_right);
	if (d->d_ofd)
	{
		xmfreblk(d->d_ofd);
	}
	for (i = 0; i < NCURDIR; i++)
	{
		if (dirtbl[i] == d)
		{
			dirtbl[i] = 0;
			diruse[i] = 0;
		}
	}
	xmfreblk(d);
}

/*
**  offree -
*/

VOID offree(P(DMD *) d)
PP(DMD *d;)
{
	int i;
	OFD *f;

	for (i = 0; i < OPNFILES; i++)
		if (((long) (f = sft[i].f_ofd)) > 0L)
			if (f->o_dmd == d)
			{
				xmfreblk(f);
				sft[i].f_ofd = 0;
				sft[i].f_own = 0;
				sft[i].f_use = 0;
			}
}


/*
 *  osif -
 */

#if	DBGOSIF
/*
 * if in debug mode, use this 'front end' so we can tell if we exit 
 * from osif
 */

int32_t osif2 PROTO((int16_t *));

long osif(int16_t *pw)
int16_t *pw;
{
	in16_t *p;
	int32_t r;

	p = (int16_t *) &pw;
	osifdmp(p - 2, pw);					/*  pass return addr and pw ptr */

	r = osif2(pw);

	osifret();
	return r;
}
#else
/*
 *  if not in debug mode, go directory to 'osif2'.  Do not pass go, do
 *	not collect $200, and do not spend time on an extra call
 */
#define	osif2	osif

#endif

int32_t osif2(P(int16_t *) pw)
PP(int16_t *pw;)
{
	char **pb, *pb2, *p, ctmp;
	BPB *b;
	BCB *bx;
	DND *dn;
	int typ, h, i, fn;
	int num, max;
	long rc, numl;
	FND *f;

	oscnt = 0;
  restrt:
	oscnt++;
	fn = pw[0];

	if (fn > 0x57)
		return (EINVFN);

	if ((rc = xsetjmp(errbuf)) != 0)
	{
		/* hard error processing */
		/* is this a media change ? */

		if (rc == E_CHNG)
		{								/* first, out with the old stuff */
			dn = drvtbl[errdrv]->m_dtl;
			offree(drvtbl[errdrv]);
			xmfreblk(drvtbl[errdrv]);
			drvtbl[errdrv] = 0;

			if (dn)
				freetree(dn);

			for (i = 0; i < 2; i++)
				for (bx = bufl[i]; bx; bx = bx->b_link)
					if (bx->b_bufdrv == errdrv)
						bx->b_bufdrv = -1;

			/* then, in with the new */

			b = (BPB *) getbpb(errdrv);
			if ((long) b <= 0)
			{
				drvsel &= ~(1 << errdrv);
				if ((long) b)
					return ((long) b);
				return (rc);
			}
			
			if (login(b, errdrv))
				return (ENSMEM);

			rwerr = 0;
			errdrv = 0;
			goto restrt;
		}

		/* else handle as hard error on disk for now */

		for (i = 0; i < 2; i++)
			for (bx = bufl[i]; bx; bx = bx->b_link)
				if (bx->b_bufdrv == errdrv)
					bx->b_bufdrv = -1;
		return (rc);
	}

	f = &funcs[fn];
	typ = f->fntyp;

	if (typ && fn && ((fn < 12) || ((fn >= 16) && (fn <= 19))))	/* std funcs */
	{
		if ((h = run->p_uft[typ & 0x7f]) > 0)
		{								/* do std dev function from a file */
			switch (fn)
			{
			case 6:
				if (pw[1] != 0xFF)
					goto rawout;
			case 1:
			case 3:
			case 7:
			case 8:
				xread(h, 1L, &ctmp);
				return (ctmp);

			case 2:
			case 4:
			case 5:
				/* write the char in the int at pw[1] */
			  rawout:
				xwrite(h, 1L, ((char *) &pw[1]) + 1);
				return;

			case 9:
				pb2 = *((char **) &pw[1]);
				while (*pb2)
					xwrite(h, 1L, pb2++);
				return;

			case 10:
				pb2 = *((char **) &pw[1]);
				max = *pb2++;
				p = pb2 + 1;
				for (i = 0; max--; i++, p++)
				{
					if (xread(h, 1L, p) == 1)
					{
						oscall(0x40, 1, 1L, p);
						if (*p == 0x0d)
						{				/* eat the lf */
							xread(h, 1L, &ctmp);
							break;
						}
					} else
						break;
				}
				*pb2 = i;
				return (0);

			case 11:
			case 16:
			case 17:
			case 18:
			case 19:
				return (0xFF);
			}
		}

		if (h == H_Null)
			return (0);

		if ((fn == 10) || (fn == 9))
			typ = 1;
		else
			typ = 0;
	}

	if (typ & 0x80)
	{
		if (typ == 0x81)
			h = pw[3];
		else
			h = pw[1];

		if (h >= NUMSTD)
			numl = (long) sft[h - NUMSTD].f_ofd;
		else if (h >= 0)
		{
			if ((h = run->p_uft[h]) > 0)
				numl = (long) sft[h - NUMSTD].f_ofd;
			else
				numl = h;
		} else
			numl = h;

		if (!numl)
			return E_IHNDL;			/* invalid handle: media change, etc */

		if (numl < 0)
		{								/* nul, prn, aux, con, clock, mouse */
			/* -1   -2   -3   -4   -5     -6    */

			if ((num = numl) == H_Null)
				return (0);				/* NUL: always returns 0    */

			/*  check for valid handle  */
			if (num < -6)
				return E_IHNDL;

			pb = (char **) &pw[4];

			/* only do things on read and write */

			if (fn == 0x3f)				/* read */
			{
				if (pw[2])				/* disallow HUGE reads      */
					return (0);

				if (pw[3] == 1)
				{
					**pb = conin(HXFORM(num));
					return (1);
				}

				return (cgets(HXFORM(num), pw[3], *pb));
			}

			if (fn == 0x40)				/* write */
			{
				if (pw[2])				/* disallow HUGE writes     */
					return (0);

				pb2 = *pb;				/* char * is buffer address */


				for (i = 0; i < pw[3]; i++)
				{
					if (num == H_Console)
						tabout(HXFORM(num), *pb2++);
					else
					{
						rc = bconout(HXFORM(num), *pb2++);
						if (rc < 0)
							return (rc);
					}
				}

				return (pw[3]);
			}

			return (0);
		}
	}
	rc = 0;
	if (fn == 0x3d || fn == 0x3c)	/* open, create */
	{
		p = *((char **) &pw[1]);
		if (ncmps(5, p, "NUL:"))
			rc = H_Null;
		else if (ncmps(5, p, "PRN:"))
			rc = H_Print;
		else if (ncmps(5, p, "AUX:"))
			rc = H_Aux;
		else if (ncmps(5, p, "CON:"))
			rc = H_Console;
		else if (ncmps(7, p, "CLOCK:"))
			rc = H_Clock;
		else if (ncmps(7, p, "MOUSE:"))
			rc = H_Mouse;
	}
	if (!rc)
	{
		typ &= 0x07f;
		switch (typ)
		{
		case 0:
			rc = (*f->fncall) (pw[1], pw[2]);
			break;

		case 1:
			rc = (*f->fncall) (pw[1], pw[2], pw[3], pw[4]);
			break;

		case 2:
			rc = (*f->fncall) (pw[1], pw[2], pw[3], pw[4], pw[5], pw[6]);
			break;

		case 3:
			rc = (*f->fncall) (pw[1], pw[2], pw[3], pw[4], pw[5], pw[6], pw[7]);
		}
	}
	return (rc);
}


/******************************************************************************
**
** S_SetVec - Function 0x25:  Set exception vector n to address
**
**	Last modified	SCC	8 Aug 85
**
*******************************************************************************
*/

int32_t S_SetVec(P(int16_t) n, P(int32_t) address)
PP(int16_t n;)
PP(int32_t address;)
{
	if (address == -1L)					/* disallow GET value       */
		return (EINVFN);

	return (trap13(5, n, address));		/* pass on to BIOS to set it in */
}

/******************************************************************************
**
** S_GetVec - Function 0x35:  Get exception vector n
**
**	Last modified	SCC	8 Aug 85
**
*******************************************************************************
**/

int32_t S_GetVec(P(int16_t) n)
PP(int16_t n;)
{
	return trap13(5, n, -1L);			/* pass to BIOS to get it   */
}



/*
 *  tikfrk -
 */

VOID tikfrk(P(int) n)
PP(int n;)
{
	int curmo;

	uptime += n;
	msec += n;
	if (msec >= 2000)
	{
		/* update time */

		msec -= 2000;
		time++;

		if ((time & 0x1F) != 30)
			return;

		time &= 0xFFE0;
		time += 0x0020;

		if ((time & 0x7E0) != (60 << 5))
			return;

		time &= 0xF81F;
		time += 0x0800;

		if ((time & 0xF800) != (24 << 11))
			return;

		time = 0;

		/* update date */

		if ((date & 0x001F) == 31)
			goto datok;

		date++;							/* bump day */

		if ((date & 0x001F) <= 28)
			return;

		if ((curmo = (date >> 5) & 0x0F) == 2)
		{
			/* 2100 is the next non-leap year divisible by 4, so OK */
			if (!(date & 0x0600))
				if ((date & 0x001F) <= 29)
					return;
				else
					goto datok;
		}

		if ((date & 0x001F) <= nday[curmo])
			return;

	  datok:
		date &= 0xFFE0;					/* bump month */
		date += 0x0021;

		if ((date & 0x01E0) <= (12 << 5))
			return;

		date &= 0xFE00;					/* bump year */
		date += 0x0221;
	}
}
