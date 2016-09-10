/*  proc.c - process management routines				*/
/*  xexec, xterm, x0term, xtermres					*/

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include "mem.h"
#include <toserrno.h>
#include "btools.h"

#define	DBGPROC	0



int envsize PROTO((const char *env));



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
 *  xexec - execute a new process
 *	Function 0x4B	Pexec
 *
 * 	flag = 0: load&go, 3:load/nogo, 4:justgo, 5:create psp 
 * 	load&go(cmdlin,cmdtail), load/nogo(cmdlin,cmdtail), justgo(psp) 
 * 	create psp - user receives a memory partition 
 *
 */


ERROR xexec(P(int16_t) flg, P(char *) s, P(char *) t, P(char *) v)
PP(int16_t flg;)								/*  0, 3, 4, or 5       */
PP(char *s;)
PP(char *t;)
PP(char *v;)								/* command, tail, environment   */
{
	PD *p;
	char *b, *e;
	int i, h;
	int cnt;
	ERROR rc;
	int32_t max;
	MD *m, *env;
	int32_t *spl;

	UNUSED(cnt);
	
	m = env = 0L;

	/*
	 *   check validity of flg - 1,2 or >5 is not allowed
	 */

	if (flg && (flg < 3 || flg > 5))
		return E_INVFN;

	/*
	 *   if we have to load, find the file
	 */

	if ((flg == 0) || (flg == 3))
		if (ixsfirst(s, 0, 0L))
			return E_FILNF;			/*  file not found  */

	xmovs(sizeof(errbuf), errbuf, bakbuf);

	if ((rc = xsetjmp(errbuf)) != 0)
	{
#if 0
/* This is the way it was. ACH */

		if (rc != E_CHNG)
			xlongjmp(bakbuf, rc);

		/* we allocated memory in a previous incarnation */

		if (flg != 4)
		{
			freeit(m, &pmd);
			freeit(env, &pmd);
		}

		xlongjmp(bakbuf, rc);
#else
		/* Free any memory allocated to this program. */
		if (flg != 4)					/* did we allocate any memory? */
			ixterm((PD *)t);					/*  yes - free it */

		xlongjmp(bakbuf, rc);
#endif
	}

	/* will we need memory and a psp ? */

	if (flg != 4)
	{									/* get largest memory partition available */
		if (!v)
			v = run->p_env;

		/*
		 *   determine minimum
		 */

		i = envsize(v);
		if (i & 1)						/*  must be even    */
			++i;
		/*
		 *  allocate environment
		 */

		if (!(env = ffit((long) i, &pmd, 0)))
		{
#if	DBGPROC
			kprintf("xexec: Not Enough Memory!\n\r");
#endif
			return E_NSMEM;
		}

		e = (char *) env->m_start;

		/* 
		 * now copy it 
		 */

		bmove(v, e, i);

		/* 
		 *   allocate base page
		 */

		max = (int32_t) ffit(-1L, &pmd, 0);	/*  amount left */

		if (max < sizeof(PD))
		{								/*  not enoufg even for PD  */
			freeit(env, &pmd);
#if	DBGPROC
			kprintf("xexec: No Room For Base Pg\n\r");
#endif
			return E_NSMEM;
		}

		/*  allocate the base page.  The owner of it is either the
		   new process being created, or the parent  */

		m = ffit(max, &pmd, 0);

		p = (PD *) m->m_start;			/*  PD is first in bp   */

		/* set owner of environment & bp */
		env->m_own = flg == 0 ? p : run;
		m->m_own = env->m_own;
		max = m->m_length;				/*  length of tpa   */

		/*
		 *   We know we have at least enough room for the PD (room 
		 *  for the rest of the pgm checked for in pgmld)
		 *   initialize the PD (first, by zero'ing it out)
		 */

		bzero(p, sizeof(PD));

		p->p_lowtpa = (int32_t) p;
		p->p_hitpa = (int32_t) p + max;
		p->p_xdta = (DTA *)&p->p_cmdlin[0];	/* default p_xdta is p_cmdlin */
		p->p_env = (char *) env->m_start;


		/* now inherit standard files from me */

		for (i = 0; i < NUMSTD; i++)
		{
			if ((h = run->p_uft[i]) > 0)
				ixforce(i, run->p_uft[i], p);
			else
				p->p_uft[i] = h;
		}

		/* and current directory set */

		for (i = 0; i < 16; i++)
			ixdirdup(i, run->p_curdir[i], p);

		/* and current drive */

		p->p_curdrv = run->p_curdrv;

		/* copy tail */

		b = &p->p_cmdlin[0];
		for (i = 0; (i < PDCLSIZE) && (*t); i++)
			*b++ = *t++;

		*b++ = 0;
		t = (char *) p;
	}

	/* 
	 *   for 3 or 0, need to load, supply baspage containing: 
	 *      tpa limits, filled in with start addrs,lens 
	 */

	if (flg == 0 || flg == 3)
	{
		if ((rc = xpgmld(s, (PD *)t)) != 0)
		{
#if	DBGPROC
			kprintf("cmain: error returned from xpgmld = %lx\n\r", rc);
#endif
			ixterm((PD *)t);
			return rc;
		}
	}
	
	if (flg == 0 || flg == 4)
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
#ifndef __ALCYON__
		{
			int16_t *spw;
			spw = (int16_t *) spl;
			*--spw = 0;						/* startup status reg */
			spl = (int32_t *) spw;
		}
#else
		*--((int16_t *) spl) = 0;						/* startup status reg */
#endif
		*--spl = (long) &supstk[SUPSIZ];
		p->p_areg[6 - 3] = p->p_areg[7 - 3] = (long) spl;
		p->p_areg[5 - 3] = p->p_dbase;
		p->p_areg[4 - 3] = p->p_bbase;
		run = (PD *) p;

		gouser();
	}

	/* sub-func 3 and 5 return here */

	return (ERROR) t;
}

/*
 * [1]	The limit on this loop should probably be changed to use sizeof(PD)
 */


/*
 *  envsize - determine size of env area
 *	counts bytes starting at 'env' upto and including the terminating
 *	double null.
 */

int envsize(P(const char *) env)
PP(const char *env;)
{
	register const char *e;
	register int cnt;

	for (e = env, cnt = 0; !(*e == '\0' && *(e + 1) == '\0'); ++e, ++cnt)
		;

	return cnt + 2;					/*  count terminating double null  */
}


/*
 *  x0term - Terminate Current Process
 *	terminates the calling process and returns to the parent process
 *	with a return code of 0 (success).
 *
 *	Function 0x00	Pterm0
 */

/* 306: 00e17760 */
VOID x0term(NOTHING)
{
	xterm(0);
}


/*
 *  xterm - terminate a process
 *	terminate the current process and transfer control to the colling
 *	process.  All files opened by the terminating process are closed.
 *
 *	Function 0x4C	Pterm
 */

/* 306: 00e17704 */
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
 *  xtermres - 
 *	Function 0x31	Ptermres
 */

/* 306: 00e176d6 */
VOID xtermres(P(int32_t) blkln, P(int16_t) rc)
PP(int32_t blkln;)
PP(int16_t rc;)
{
	xsetblk(0, run, blkln);
	xmakeres(run);
	xterm(rc);
}



/*
 *  ixterm - terminate a process
 *	terminate process with PD 'r'.
 */

/* 306: 00e1776c */
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
		if ((h = r->p_curdir[i]) != 0)
			diruse[h]--;
	}

	/* free process memory */
	xmfreall(r);
}
