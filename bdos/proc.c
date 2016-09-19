/*  proc.c - process management routines				*/
/*  xexec, xterm, x0term, xtermres					*/

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include "mem.h"
#include <toserrno.h>
#include "btools.h"
#include "pghdr.h"

#define	DBGPROC	0


/*
 * values of 'mode' for Pexec()
 */
#define PE_LOADGO     0
#define PE_LOAD       3
#define PE_GO         4
#define PE_BASEPAGE   5
#define PE_GOTHENFREE 6
#define PE_BASEPAGEFLAGS 7
#define PE_RELOCATE   50    /* required for NatFeats support only, not in Atari TOS */

#define TPASIZE_QUANTUM (128*1024L)

#define MINSTACK 0x800L


ERROR xpgmld PROTO((PGMHDR01 *hdr, FH fh, PD *p));


/*
 *  run - ptr to PD for current process
 */

PD *run;


/*
 *  supstk - common sup stack for all procs  
 */

int16_t supstk[SUPSIZ];


/*
 *  bakbuf - longjump buffer
 */

xjmp_buf bakbuf;


/*	
 *  xtermres - 
 *	Function 0x31	Ptermres
 */

/* 306de: 00e176d6 */
/* 306us: 00e1767c */
VOID xtermres(P(int32_t) blkln, P(int16_t) rc)
PP(int32_t blkln;)
PP(int16_t rc;)
{
	xsetblk(0, run, blkln);
	xmakeres(run);
	xterm(rc);
}



/*
 *  xterm - terminate a process
 *	terminate the current process and transfer control to the colling
 *	process.  All files opened by the terminating process are closed.
 *
 *	Function 0x4C	Pterm
 */

/* 306de: 00e17704 */
/* 306us: 00e176aa */
VOID xterm(P(uint16_t) rc)
PP(uint16_t rc;)
{
	register PD *r;

#ifdef __ALCYON__
	indcall(trap13(0x00050102L, -1L));	/*  call user term handler */
#else
	indcall(trap13(0x0005, 0x0102, -1L));	/*  call user term handler */
#endif
	xbgettime();
	
	run = (r = run)->p_parent;
	ixterm(r);
	run->p_dreg[0] = rc;
	gouser();
}


/*
 *  x0term - Terminate Current Process
 *	terminates the calling process and returns to the parent process
 *	with a return code of 0 (success).
 *
 *	Function 0x00	Pterm0
 */

/* 306de: 00e17760 */
/* 306us: 00e17706 */
VOID x0term(NOTHING)
{
	xterm(0);
}


/*
 *  ixterm - terminate a process
 *	terminate process with PD 'r'.
 */

/* 306de: 00e1776c */
/* 306us: 00e17712 */
VOID ixterm(P(PD *) r)
PP(PD *r;)									/*  PD of process to terminate      */
{
	register int h;
	register int i;

	/*  check the standard devices in both file tables  */
	for (i = 0; i < NUMSTD; i++)
		if ((h = r->p_uft[i]) > 0)
			xclose(h);

	for (i = 0; i < (OPNFILES - NUMSTD); i++)
		if (r == sft[i].f_own)
			xclose(i + NUMSTD);

	/*  check directory usage */
	for (i = 0; i < NUMCURDIR; i++)
	{
		if ((h = (char)r->p_curdir[i]) /* != 0 */)
			if (diruse[h])
				--diruse[h];
	}

	/* free process memory */
	xmfreall(r);
}


/*  
 *  xexec - execute a new process
 *	Function 0x4B	Pexec
 *
 * 	flag = 0: load&go, 3:load/nogo, 4:justgo, 5:create psp 
 * 	load&go(cmdlin,cmdtail), load/nogo(cmdlin,cmdtail), justgo(psp) 
 * 	create psp - user receives a memory partition 
 *
 */

/* 306de: 00e17808 */
/* 306us: 00e177ae */
ERROR xexec(P(int16_t) mode, P(char *) s, P(char *) t, P(char *) v)
PP(int16_t mode;)								/*  0, 3, 4, or 5       */
PP(char *s;)
PP(char *t;)
PP(char *v;)								/* command, tail, environment   */
{
	/*
	 * the setjmp() routine used in GEMDOS does not save any registers,
	 * so no register variables must be used here.
	 */
	PD *volatile p;
	char *volatile b;
	char *volatile e;
	char *volatile env;
	int volatile h;
	int volatile i;
	int volatile cnt;
	ERROR volatile rc;
	int32_t volatile max;
	int32_t *volatile spl;
	int16_t *spw;
	long volatile unused;
	ERROR volatile rc2;
	FH volatile fh;
	PGMHDR01 hdr;
	int volatile allocmode;
	long volatile alt_ram_size;
	long volatile st_ram_size;
	long volatile tpasize;
	
	UNUSED(unused);
	
	p = (PD *)(env = 0);
	fh = 0;
	
	/*
	 *   check validity of mode - 1,2 or >5 is not allowed
	 */

	if (mode != PE_LOADGO && (mode < PE_LOAD || mode > PE_BASEPAGEFLAGS))
		return E_INVFN;

	xmovs(sizeof(errbuf), errbuf, bakbuf);

	if ((rc = xsetjmp(errbuf)) != 0)
	{
		/* Free any memory allocated to this program. */
		if (fh)
			xclose(fh);
		if (p)
			xmfree((intptr_t)p);
		if (env)
			xmfree((intptr_t)env);
		xlongjmp(bakbuf, rc);
	}

	/*
	 *   if we have to load, find the file
	 */

	if (mode == PE_LOADGO || mode == PE_LOAD)
	{
		/*
		 * This block formerly was in a separate function,
		 * but we may have to close the filehandle in case
		 * of critical errors
		 */
		 
		/* open file for read */
		if ((fh = rc2 = xopen(s, RO_MODE)) < 0)
			return rc2;			/*  file not found  */

		/* read file header */
		rc2 = xread(fh, (long)sizeof(hdr), &hdr);
		if (rc2 != sizeof(hdr) || hdr.h01_magic != 0x601a)
		{
			xclose(fh);
			if (rc2 < 0)
				return rc2;
			else
				return E_PLFMT;
		}
		goto donehdr;
	}
	
	if (mode == PE_BASEPAGEFLAGS)
	{
		hdr.h01_ldflags = (long)s;
	} else
	{
		hdr.h01_ldflags = 0;
	}

	hdr.h01_tlen = hdr.h01_dlen = hdr.h01_blen = 0;
	
donehdr:
	
	/* will we need memory and a psp ? */

	if (mode != PE_GO && mode != PE_GOTHENFREE)
	{									/* get largest memory partition available */
		if (!v)
			v = run->p_env;

		/*
		 *  envsize - determine size of env area
		 *	counts bytes starting at 'env' upto and including the terminating
		 *	double null.
		 */
	
		e = v;
		i = 0;
		for (;;)
		{
			if (*e++ == '\0')
			{
				if (*e++ == '\0')
					break;
				i++;
			}
			i++;
		}
	
		i += 2;					/*  count terminating double null  */

		/*
		 *   determine minimum
		 */

		cnt = 0;
		if ((i + cnt) & 1)						/*  must be even    */
			++cnt;
		/*
		 *  allocate environment
		 */
		if (hdr.h01_ldflags & PF_LOADTTRAM)
			h = MX_PTTRAM;
		else
			h = MX_STRAM;
		if (!(e = (env = (char *)xmxalloc((long)(i + cnt + 2), h))))
		{
#if	DBGPROC
			kprintf("xexec: Not Enough Memory!\n");
#endif
			return E_NSMEM;
		}

		/* 
		 * now copy it 
		 */

#if 0 /* that would be too easy... */
		xmovs(i, v, e);
#else
		while (i--)
			*e++ = *v++;
#endif

		/*
		 * allocate the TPA
		 *
		 * we first determine if ST RAM and/or alternate RAM is available for
		 * allocation, based on the flags, the amount of RAM required and
		 * the presence or absence of TT RAM.
		 *
		 * if no types are available (the requested amount is too large), we
		 * return NULL.
		 *
		 * if only one type of RAM is available, we allocate it & return a
		 * pointer to it.
		 *
		 * if both types are available, we normally allocate in alternate RAM
		 * *except* if the amount of ST RAM is greater than the amount of
		 * alternate RAM.  In this case, we use a tiebreaker: bits 31-27
		 * of the flags field plus 1 is taken as a 4-bit number, which is
		 * multiplied by 128K and added to the base amount needed to get a
		 * "would like to have" amount.  If this amount is larger than the
		 * amount of alternate RAM, then we allocate in ST RAM.
		 *
		 * Reference: TT030 TOS Release Notes, Third Edition, 6 September 1991,
		 * pages 29-30.
		 *
		 * returns: ptr to MD of allocated memory (NULL => failed)
		 *          updates 'avail' with the size of allocated memory
		 */
		
		if (hdr.h01_ldflags & PF_LOADTTRAM)
		{
			tpasize = ((hdr.h01_ldflags & 0xf0000000L) >> 28) & 0x0000000fL;
			tpasize = (tpasize + 1) * TPASIZE_QUANTUM;
			tpasize += hdr.h01_tlen + hdr.h01_dlen + hdr.h01_blen;
			alt_ram_size = xmxalloc(-1L, MX_TTRAM);
			st_ram_size = xmxalloc(-1L, MX_STRAM);
			if (alt_ram_size > st_ram_size || alt_ram_size > tpasize)
			{
				max = alt_ram_size;
				allocmode = MX_TTRAM;
			} else
			{
				max = st_ram_size;
				allocmode = MX_STRAM;
			}
		} else
		{
			allocmode = MX_STRAM;
			max = xmxalloc(-1L, MX_STRAM);
		}
		
		/* 
		 *   allocate base page
		 */

		if (max < (sizeof(PD) + hdr.h01_tlen + hdr.h01_dlen + hdr.h01_blen + MINSTACK))
		{								/*  not enough even for PD  */
			xmfree((intptr_t)env);
#if	DBGPROC
			kprintf("xexec: No Room For Base Pg\n");
#endif
			return E_NSMEM; /* BUG: fh still open */
		}

		/*  allocate the base page.  The owner of it is either the
		   new process being created, or the parent  */

		p = (PD *)xmxalloc(max, allocmode);

		/* set owner of environment & bp */
		if (mode == PE_LOADGO)
		{
			xmsetown(env, p);
			xmsetown(p, p);
		}
		/* ...else current process is already owner */
		
		/*
		 *   We know we have at least enough room for the PD (room 
		 *  for the rest of the pgm checked for in pgmld)
		 *   initialize the PD (first, by zero'ing it out)
		 */

		p->p_lowtpa = (int32_t) p;
		p->p_hitpa = (int32_t) p + max;
#if 0 /* that would be too easy... */
		bzero((char *)p + 8, sizeof(PD) - 8);
#else
		for (i = 0, b = (char *)p + 8; i < sizeof(PD); i++)
			*b++ = 0;
#endif
		p->p_xdta = (DTAINFO *)&p->p_cmdlin[0];	/* default p_xdta is p_cmdlin */
		p->p_flags = hdr.h01_ldflags;
		p->p_env = env;

		/* now inherit standard files from me */

		for (i = 0; i < NUMSTD; i++)
		{
			if ((h = run->p_uft[i]) > 0)
				ixforce(i, run->p_uft[i], p);
			else
				p->p_uft[i] = h;
		}

		/* and current directory set */

		for (i = 0; i < NUMCURDIR; i++)
			ixdirdup(i, run->p_curdir[i], p);

		/* and current drive */

		p->p_curdrv = run->p_curdrv;

		/* copy tail */

		b = &p->p_cmdlin[0];
		for (i = 0; i < (PDCLSIZE - 3) && *t; i++)
			*b++ = *t++;

		*b++ = 0;
		t = (char *) p;
	}

	/* 
	 *   for 3 or 0, need to load, supply baspage containing: 
	 *      tpa limits, filled in with start addrs,lens 
	 */

	if (mode == PE_LOADGO || mode == PE_LOAD)
	{
		if ((rc = xpgmld(&hdr, fh, (PD *)t)) != 0)
		{
#if	DBGPROC
			kprintf("cmain: error returned from xpgmld = %lx\n", rc);
#endif
			ixterm((PD *)t);
			return rc;
		}
	}
	
	if (mode == PE_LOADGO || mode == PE_GO || mode == PE_GOTHENFREE)
	{
		p = (PD *) t;
		p->p_parent = run;
		spl = (int32_t *) p->p_hitpa;
		*--spl = (int32_t) p;
		*--spl = 0L;					/* bogus retadd */

		/* 10 regs (40 bytes) of zeroes  */

		for (i = 0; i < 10; i++)
			*--spl = 0L;

		*--spl = p->p_tbase;			/* text start */
		spw = (int16_t *) spl;
		*--spw = 0;						/* startup status reg */
		spl = (int32_t *) spw;
		*--spl = (long) &supstk[SUPSIZ];
		p->p_areg[6 - 3] = p->p_areg[7 - 3] = (long) spl;
		p->p_areg[5 - 3] = p->p_dbase;
		p->p_areg[4 - 3] = p->p_bbase;
		run = p;

		if (mode == PE_GOTHENFREE)
		{
			xmsetown(run, run);
			xmsetown(run->p_env, run);
		}
		
		gouser();
	}

	/* sub-func 3, 5 and 7 return here */

	return (ERROR) t;
}


/*
 *  xpgmld - oldest known gemdos load format - very similar to cp/m 68k
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
 *			fix up the code using that info
 *		zero out the bss
 */

/* 306de: 00e17dca */
/* 306us: 00e17d70 */
ERROR xpgmld(P(PGMHDR01 *) hdr, P(FH) fh, P(PD *) pdptr)
PP(PGMHDR01 *hdr;)
PP(FH fh;)
PP(PD *pdptr;)
{
	register PD *p;
	register char *rp;				/*  relocation info pointer */
	register char *relocstart;
	int32_t *cp;					/*  code pointer        */
	intptr_t tbase;					/*  base addr of text segment   */
	int32_t addr;
	int32_t flen;
	int32_t unused;
	int32_t fixbase;
	int32_t relst;
	int32_t rlen;					/*  nbr of relocation bytes */
	register short c;
	int32_t blen;
	int32_t tpalen;
	register int32_t fixp;
	register int32_t n;

	UNUSED(unused);
	
	p = pdptr;
	/* relst = 0; */ /* BUG: seems to be removed */

	/*
	 *  calculate program load info
	 */

	flen = hdr->h01_tlen + hdr->h01_dlen;
	cp = &p->p_tbase;
	tpalen = p->p_hitpa - p->p_lowtpa - sizeof(PD);
	p++;	/*  1st byte after PD   */
	tbase = addr = (intptr_t)p;
	fixbase = tbase;
	relocstart = (char *)(tbase + flen);
	blen = tpalen - flen;
	
	/*
	 *  see if there is enough room to load in the file, then see if
	 *  the requested bss space is larger than the space we have to offer
	 */

	if (blen < hdr->h01_blen)
	{
		xclose(fh);
		return E_NSMEM;
	}
	
	/*
	 *  initialize PD fields
	 */
	*cp++ = addr; /* sets p->p_tbase */
	*cp++ = hdr->h01_tlen;
	addr += hdr->h01_tlen;
	*cp++ = addr; /* sets p->p_dbase */
	*cp++ = hdr->h01_dlen;
	addr += hdr->h01_dlen;
	*cp++ = addr; /* sets p->p_bbase */
	*cp++ = hdr->h01_blen;
	addr += hdr->h01_blen;
	
	/*  
	 *  read in the program file (text and data)
	 *  if there is an error reading in the file or if it is an abs
	 * file, then we are finished
	 * Remember that the I/O routines will jump back via longjmp()
	 * on hard errors.
	 */
	xread(fh, flen, (VOIDPTR)tbase);

	if (!hdr->h01_abs)		/*  do we need to clr bss here? */
	{
		/*  
		 *  if not an absolute format, position past the symbols and start the 
		 * reloc pointer  (flen is tlen + dlen).  NOTE that relst is 
		 * init'd to 0, so if the format is absolute, we will not drop
		 * into the fixup code.
		 */
		xlseek(flen + hdr->h01_slen + sizeof(*hdr), fh, SEEK_SET);

		xread(fh, (long) sizeof(relst), &relst);

		if (relst != 0)
		{
			fixp = fixbase + relst;

			/*  make sure we didn't wrap memory or overrun the bss  */

			if (fixp < fixbase || (char *)fixp >= relocstart)
			{
				xclose(fh);
				return E_PLFMT;
			}
			
			*((int32_t *) (fixp)) += fixbase;	/*  1st fixup     */
			
			do {
				if (fixp < fixbase || (char *)fixp >= relocstart)
				{
					xclose(fh);
					return E_PLFMT;
				}
				
				/*  read in relocation info  */
				rlen = xread(fh, blen, relocstart);
				n = rlen;
				
				rp = relocstart;
				/* do fixups using that info */
				while (n)
				{
					if (!(c = *rp++))
						goto done;
					c &= 0xff;
					if (c == 1)
					{
						fixp += 0xfe;
					} else
					{
						/*  get the byte at rp, don't sign ext, add to cp  */
						fixp += c;
						*((int32_t *) (fixp)) += fixbase;
					}
					n--;
				}
			} while (rlen == blen);
		}
		done:;
	}
	
	/* zero out the bss */
	if (hdr->h01_ldflags & PF_FASTLOAD)
	{
		zeromem(relocstart, relocstart + hdr->h01_blen);
	} else
	{
		zeromem(relocstart, relocstart + blen);
	}
	
	xclose(fh);
	return E_OK;
}
