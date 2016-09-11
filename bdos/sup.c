/*	sup.c - GEMDOS main function dispatcher				*/

#define	DBGOSIF	0


#include "tos.h"
#include "fs.h"
#include "bios.h"
#include <toserrno.h>
#include "mem.h"

/*
 *  local constants
 */

#define LENOSM 4000


int8_t const stddev[NUMSTD] = { H_Null, H_Null, H_Print, H_Aux, H_Null, H_Null };

/*
 *  forward declarations
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
 *  globals
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

int16_t osmlen;

int16_t osmem[LENOSM];



/*
 *  FND - Function Descriptor
 *	each entry in the function table (below) consists of the address of
 *	the function which corresponds to the function number, and a function
 *	type.
 */

typedef ERROR (*gdf) PROTO ((int16_t, ...));

#define FND struct _fnd
FND
{
	gdf fncall;
	int16_t fntyp;
};

/*
 *  funcs - table of os functions, indexed by function number
 *	each entry is for an FND structure.
 *	the function 'ni' is used as the address for functions not implemented.
 */

#ifdef __ALCYON__
#define GDF /* does not casts in initialization */ 
#else
#define GDF (gdf)
#endif

/* 306: 00e3d9f8 */
FND const funcs[0x58] = {

	/* 0x00 */ { GDF x0term, 0 },

	/* 
	 *  console functions 
	 * on these functions, the 0x80 flag indicates std file used 
	 * 0x80 is std in, 0x81 is stdout, 0x82 is stdaux, 0x83 stdprn 
	 */

	/* 0x01 */ { GDF xconin, 0x80 },
	/* 0x02 */ { GDF xtabout, 0x81 },
	/* 0x03 */ { GDF xauxin, 0x82 },
	
	/* 0x04 */ { GDF xauxout, 0x82 },
	/* 0x05 */ { GDF xprtout, 0x83 },
	/* 0x06 */ { GDF rawconio, 0 },
	
	/* 0x07 */ { GDF x7in, 0x80 },
	/* 0x08 */ { GDF x8in, 0x80 },
	/* 0x09 */ { GDF xprt_line, 0x81 },
	
	/* 0x0A */ { GDF readline, 0x80 },
	/* 0x0B */ { GDF xconstat, 0x80 },
	
	/* 
	 * disk functions 
	 * on these functions the 0x80 flag indicates whether a handle 
	 * is required, the low bits represent the parameter ordering, 
	 * as usual. 
	 */

	/* 0x0c */ { GDF ni, 0 },
	/* 0x0d */ { GDF ni, 0 },

	/* 0x0E */ { GDF xsetdrv, 0 },
	/* 0x0f */ { GDF ni, 0 },

	/* 
	 *  extended console functions 
	 * on these functions, the 0x80 flag indicates 
	 * std file used, as above 
	 */

	/* 0x10 */ { GDF xconostat, 0x81 },
	/* 0x11 */ { GDF xprtostat, 0x83 },
	/* 0x12 */ { GDF xauxistat, 0x82 },
	/* 0x13 */ { GDF xauxostat, 0x82 },

	/* 0x14 */ { GDF xmaddalt, 1 },
	/* 0x15 */ { GDF ni, 0 }, /* Srealloc - not implemented */
	/* 0x16 */ { GDF ni, 0 }, /* Slbopen - not implemented */
	/* 0x17 */ { GDF ni, 0 }, /* Slbclose - not implemented */
	/* 0x18 */ { GDF ni, 0 },

	/* 0x19 */ { GDF xgetdrv, 0 },
	/* 0x1A */ { GDF xsetdta, 1 },
	
	/* 0x1b */ { GDF ni, 0 },
	/* 0x1c */ { GDF ni, 0 },
	/* 0x1d */ { GDF ni, 0 },
	/* 0x1e */ { GDF ni, 0 },
	/* 0x1f */ { GDF ni, 0 },

	/* 0x20 */ { GDF ni, 0 }, /* xgsps - Super - handled in dispatcher */
	
	/* 0x21 */ { GDF ni, 0 },
	/* 0x22 */ { GDF ni, 0 },
	/* 0x23 */ { GDF ni, 0 },
	/* 0x24 */ { GDF ni, 0 },

	/* 0x25 */ { GDF ni, 0 }, /* S_SetVec, 1; XBIOS function in GEMDOS */

	/* 0x26 */ { GDF ni, 0 },
	/* 0x27 */ { GDF ni, 0 },
	/* 0x28 */ { GDF ni, 0 },
	/* 0x29 */ { GDF ni, 0 },

	/* 0x2A */ { GDF xgetdate, 0 },
	/* 0x2B */ { GDF xsetdate, 0 },
	/* 0x2C */ { GDF xgettime, 0 },
	
	/* 0x2D */ { GDF xsettime, 0 },
	/* 0x2e */ { GDF ni, 0 },
	/* 0x2F */ { GDF xgetdta, 0 },
	
	/* 0x30 */ { GDF xgetver, 0 },
	/* 0x31 */ { GDF xtermres, 1 },
	/* 0x32 */ { GDF ni, 0 },
	/* 0x33 */ { GDF ni, 0 }, /* Sconfig - not implemented */
	/* 0x34 */ { GDF ni, 0 },

	/* 0x35 */ { GDF ni, 0 },	/* S_GetVec; XBIOS function in GEMDOS */
	/* 0x36 */ { GDF xgetfree, 1 },
	/* 0x37 */ { GDF ni, 0 },
	/* 0x38 */ { GDF ni, 0 },

	/* 0x39 */ { GDF xmkdir, 1 },
	/* 0x3A */ { GDF xrmdir, 1 },
	/* 0x3B */ { GDF xchdir, 1 },
	
	/* xclose will handle its own redirection */

	/* 0x3C */ { GDF xcreat, 1 },
	/* 0x3D */ { GDF xopen, 1 },
	/* 0x3E */ { GDF xclose, 0x0 },
	
	/* 0x3F */ { GDF xread, 0x82 },

	/* 0x40 */ { GDF xwrite, 0x82 },
	/* 0x41 */ { GDF xunlink, 1 },
	/* 0x42 */ { GDF xlseek, 0x81 },

	/* 0x43 */ { GDF xchmod, 1 },
	/* 0x44 */ { GDF xmxalloc, 1 }, /* F_IOCtl in PC-DOS */
	/* 0x45 */ { GDF xdup, 0 },
	/* 0x46 */ { GDF xforce, 0 },
	
	/* 0x47 */ { GDF xgetdir, 1 },
	/* 0x48 */ { GDF xmalloc, 1 },
	/* 0x49 */ { GDF xmfree, 1 },

	/* 0x4A */ { GDF xsetblk, 2 },
	/* 0x4B */ { GDF xexec, 3 },
	/* 0x4C */ { GDF xterm, 0 },
	/* 0x4d */ { GDF ni, 0 },

	/* 0x4E */ { GDF xsfirst, 1 },
	/* 0x4F */ { GDF xsnext, 0 },

	/* 0x50 */ { GDF ni, 0 },
	/* 0x51 */ { GDF ni, 0 },
	/* 0x52 */ { GDF ni, 0 },
	/* 0x53 */ { GDF ni, 0 },
	/* 0x54 */ { GDF ni, 0 },
	/* 0x55 */ { GDF ni, 0 },

	/* 0x56 */ { GDF xrename, 2 },
	/* 0x57 */ { GDF xgsdtof, 1 }
};


/************************************************************************/

char *bdosver = "GEMDOS Version 01.KTB";

/************************************************************************/



/*
 *  ni -
 */

/* 306: 00e18af6 */
ERROR ni(NOTHING)
{
	return E_INVFN;
}



/*
 *  xgetver - Function 0x30:  Sversion
 *	return current version number
 */

/* 306: 00e18b00 */
int32_t xgetver(NOTHING)
{
	return ((GEMDOS & 0xff) << 8) | ((GEMDOS >> 8) & 0xff);					/*  minor.major */
}


/*
 *  cinit - C part of osinit().
 */

/* 306: 00e18b0e */
VOID cinit(NOTHING)
{
	register PD *r;
	register int32_t *p;
	register int i;
	
	xminit();
	osmlen = LENOSM;
	r = run = &ospd;
	for (p = (int32_t *)r, i = sizeof(*r) / sizeof(*p); i != 0; i--)
		*p++ = 0;

	/* set up system initial standard handles */
	for (i = 0; i < NUMSTD; i++)
		r->p_uft[0] = stddev[i];

	buptr[0] = beptr[0] = glbkbchar[0];
	buptr[1] = beptr[1] = glbkbchar[1];
	buptr[2] = beptr[2] = glbkbchar[2];

#if !GEMDOS
	date_time(GET_DATE, &date);			/* allow bios to initialise date and */
	date_time(GET_TIME, &time);			/* time from hardware, if supported */
#endif
}


/*
 *  ncmps -  compare two text strings, ignoring case.
 */

int ncmps(P(int) n, P(const char *) s, P(const char *) d)
PP(int n;)
PP(const char *s;)
PP(const char *d;)
{
	while (n--)
		if (uc(*s++) != uc(*d++))
			return 0;

	return 1;
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
 *  offree -
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
	ERROR rc;
	long numl;
	const FND *f;

	oscnt = 0;
  restrt:
	oscnt++;
	fn = pw[0];

	if (fn > 0x57)
		return E_INVFN;

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

			b = Getbpb(errdrv);
			if ((ERROR) b <= 0)
			{
				drvsel &= ~(1 << errdrv);
				if ((ERROR) b)
					return (ERROR) b;
				return rc;
			}
			
			if (login(b, errdrv))
				return E_NSMEM;

			rwerr = 0;
			errdrv = 0;
			goto restrt;
		}

		/* else handle as hard error on disk for now */

		for (i = 0; i < 2; i++)
			for (bx = bufl[i]; bx; bx = bx->b_link)
				if (bx->b_bufdrv == errdrv)
					bx->b_bufdrv = -1;
		return rc;
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
				return ctmp;

			case 2:
			case 4:
			case 5:
				/* write the char in the int at pw[1] */
			  rawout:
				return xwrite(h, 1L, ((char *) &pw[1]) + 1);

			case 9:
				pb2 = *((char **) &pw[1]);
				while (*pb2)
					xwrite(h, 1L, pb2++);
#ifdef __ALCYON__
				return; /* BUG: return without value */
#else
				return E_OK;
#endif

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
				return 0;

			case 11:
			case 16:
			case 17:
			case 18:
			case 19:
				return 0xFF;
			}
		}

		if (h == H_Null)
			return 0;

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
				return 0;				/* NUL: always returns 0    */

			/*  check for valid handle  */
			if (num < -6)
				return E_IHNDL;

			pb = (char **) &pw[4];

			/* only do things on read and write */

			if (fn == 0x3f)				/* read */
			{
				if (pw[2])				/* disallow HUGE reads      */
					return 0;

				if (pw[3] == 1)
				{
					**pb = conin(HXFORM(num));
					return 1;
				}

				return cgets(HXFORM(num), pw[3], *pb);
			}

			if (fn == 0x40)				/* write */
			{
				if (pw[2])				/* disallow HUGE writes     */
					return 0;

				pb2 = *pb;				/* char * is buffer address */


				for (i = 0; i < pw[3]; i++)
				{
					if (num == H_Console)
					{
						tabout(HXFORM(num), *pb2++);
					} else
					{
						rc = Bconout(HXFORM(num), *pb2++);
						if (rc < 0)
							return rc;
					}
				}

				return pw[3];
			}

			return 0;
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
	return rc;
}


/******************************************************************************
 *
 * S_SetVec - Function 0x25:  Set exception vector n to address
 *
 *	Last modified	SCC	8 Aug 85
 *
 ******************************************************************************
 */

int32_t S_SetVec(P(int16_t) n, P(int32_t) address)
PP(int16_t n;)
PP(int32_t address;)
{
	if (address == -1L)					/* disallow GET value       */
		return E_INVFN;

	return trap13(5, n, address);		/* pass on to BIOS to set it in */
}

/******************************************************************************
 *
 * S_GetVec - Function 0x35:  Get exception vector n
 *
 *	Last modified	SCC	8 Aug 85
 *
 ******************************************************************************
 */

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
			{
				if ((date & 0x001F) <= 29)
					return;
				else
					goto datok;
			}
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
