/*  umem.c - user memory management interface routines			*/
/*  xmalloc, xsetblk						*/


#ifndef	DBGUMEM
#define	DBGUMEM	0
#endif

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include "mem.h"
#include <toserrno.h>

/*
 * value ORed to the start address for blocks
 * allocated from alt mem
 */
#define M_ALTFLAG 1L


/*
 *  STATIUMEM - cond comp; set to true to count calls to these routines
 */

#define	STATIUMEM	FALSE

#ifndef	DBGIUMEM
#define	DBGIUMEM	0
#endif


#if	STATIUMEM

long ccffit = 0;

long ccfreeit = 0;

#endif

static VOID printstr PROTO((const char *str));


/*
 * xminit - initialize memory descriptors.
 */

/* 306: 00e1800c */
VOID xminit(NOTHING)
{
	register MD *q;
	register MD *unused;
	register intptr_t start;
	register intptr_t am;
	
	UNUSED(unused);
	Getmpb(&pmd);
	for (q = pmd.mp_mfl; q != NULL; q = q->m_link)
	{
		start = q->m_start;
		if ((start &= 0x0)) /* ??? */
		{
			am = 2;
			am -= start;
			q->m_start += am;
			q->m_length -= am;
		}
		q->m_length = q->m_length & ~1L;
		if (q->m_length < 0)
			q->m_length = 0;
	}
}


/* 306: 00e1806c */
static int32_t xmfsize(P(MPB *) mp, P(int16_t) mode)
PP(MPB *mp;)
PP(int16_t mode;)
{
	register int32_t maxval;
	register MD *q;
	
	maxval = 0;
	q = mp->mp_mfl;
	while (q != NULL)
	{
		if ((mode == MX_STRAM) && (q->m_start & M_ALTFLAG))
			break;
		if ((mode != MX_TTRAM) || (q->m_start & M_ALTFLAG))
		{
			if (q->m_length > maxval)
				maxval = q->m_length;
		}
		q = q->m_link;
	}
	return maxval;
}


/* 306: 00e180b8 */
MD *ffit(P(long) amount, P(MPB *) mp, P(int16_t) mode)
PP(long amount;)
PP(MPB *mp;)
PP(int16_t mode;)
{
	register MD *q;
	register MD *p;
	register MD *p1;
	MD *q1;
	register BOOLEAN altflag;

#if	STATIUMEM
	++ccffit;
#endif

	p = (MD *)mp;
	if ((q = ((MPB *)p)->mp_mfl) == NULL)		/* get free list pointer */
	{
#if	DBGIUMEM
		kprintf("ffit: null rover\n");
#endif
		return NULL;
	}
	if (amount <= 0)
		return NULL;
	amount = (amount + 1) & ~1;
	
	altflag = TRUE;
	switch (mode)
	{
	case MX_STRAM:
		altflag = FALSE;
	case MX_TTRAM:
		while (q != NULL && (q->m_length < amount || (q->m_start & M_ALTFLAG) != altflag))
		{
			p = q;
			q = p->m_link;
		}
		if (q == NULL)
			return NULL;
		break;
	case MX_PSTRAM:
	case MX_PTTRAM:
		while (q != NULL && q->m_length < amount)
		{
			p = q;
			q = p->m_link;
		}
		if (q == NULL)
			return NULL;
		if ((q->m_start & M_ALTFLAG) || mode == MX_PSTRAM)
			break;
		q1 = q;
		p1 = p;
		while (q != NULL && !(q->m_start & M_ALTFLAG))
		{
			p = q;
			q = p->m_link;
		}
		while (q != NULL && q->m_length < amount)
		{
			p = q;
			q = p->m_link;
		}
		if (q == NULL)
		{
			q = q1;
			p = p1;
		}
		break;
	}

	if (q->m_length == amount)
	{
		/* take the whole thing */
		p->m_link = q->m_link;
	} else
	{							/* break it up - 1st allocate a new
								   MD to describe the remainder */
		/*********** TBD **********
		*  Nicer Handling of This *
		*         Situation       *
		**************************/
		if ((q1 = mgetmd()) == NULL)
		{
#if	DBGIUMEM
			kprintf("ffit: Null Mget\n");
#endif
			return NULL;
		}

		/*  init new MD  */

		q1->m_length = q->m_length - amount;
		q1->m_start = q->m_start + amount;
		q1->m_link = q->m_link;

		/*  adjust allocated block  */

		q->m_length = amount;
		p->m_link = q1;
	}

	/*  link allocate block into allocated list,
	   mark owner of block, & adjust rover  */

	q->m_link = mp->mp_mal;
	mp->mp_mal = q;

	q->m_own = run;

	return q;					/* got some */
}


/*
 *  xsetblk - 
 *
 *	Function 0x4A	Mshrink
 *
 *	Last modified	SCC	19 Apr 85
 */

/* 306: 00e1821c */
ERROR xsetblk(P(int16_t) n, P(VOIDPTR) blk, P(int32_t) len)
PP(int16_t n;)									/*  dummy, not used         */
PP(VOIDPTR blk;)								/*  addr of block to free   */
PP(register int32_t len;)								/*  length of block to free */
{
	register MD *m, *p;

	UNUSED(n);
	/*
	 * Traverse the list of memory descriptors looking for this block.
	 */

	for (p = pmd.mp_mal; p; p = p->m_link)
		if ((intptr_t) blk == (p->m_start & ~M_ALTFLAG))
			goto found;

	/*
	 * If block address doesn't match any memory descriptor, then abort.
	 */

	return E_IMBA;

found:
	/*
	 *  Always shrink to an even word length.
	 */
	len++;
	len &= ~1L;

	/*
	 *  If the caller is not shrinking the block size, then abort.
	 */

	if (p->m_length < len)
		return E_GSBF;
	if (p->m_length == len)
		return E_OK;

	if (len == 0)
	{
		freeit(p, &pmd);
		return E_OK;
	}
	
	/*
	 *  Create a memory descriptor for the freed portion of memory.
	 */

	m = mgetmd();
	/*
	 * what if 0? *
	 */
	if (m == NULL)
	{
#if	DBGUMEM
		panic("umem.c/xsetblk: Null Return From MGET\n");
#endif
		return ERR;
	}
	m->m_start = p->m_start + len;
	m->m_length = p->m_length - len;
	p->m_length = len;
	freeit(m, &pmd);

	return E_OK;
}


/*
 *  freeit - Free up a memory descriptor
 */

/* 306: 00e182bc */
VOID freeit(P(MD *) m, P(MPB *) mp)
PP(register MD *m;)
PP(MPB *mp;)
{
	register MD *p, *q;
	register intptr_t s;
	register intptr_t addr;
	MD *p1;
	
#if	STATIUMEM
	++ccfreeit;
#endif

	p1 = NULL;

	addr = m->m_start;
	q = (MD *)mp;
	p = ((MPB *)q)->mp_mfl;
	for (; p; p = (q = p)->m_link)
	{
		s = p->m_start;
		if (s > 0 && addr > 0 && addr < s)
			break;
		if (s < 0 && addr > 0) 
			break;
		if (s < 0 && addr < 0 && addr < s)
			break;
		p1 = p;
	}
	
	m->m_link = p;
	q->m_link = m;

	if (p)
	{
		if (m->m_start + m->m_length == p->m_start)
		{								/* join to higher neighbor */
			m->m_length += p->m_length;
			m->m_link = p->m_link;

			xmdfree(p);
		}
	}
	
	if (p1)
	{
		if (p1->m_start + p1->m_length == m->m_start)
		{								/* join to lower neighbor */
			p1->m_length += m->m_length;
			p1->m_link = m->m_link;

			xmdfree(m);
		}
	}
}


/*
 *  xmalloc - 
 *
 *	Function 0x44	Mxalloc
 *
 *	Last modified	SCC	3 Jun 85
 */

/* 306: 00e18372 */
int32_t xmxalloc(P(int32_t) amount, P(int16_t) mode)
PP(int32_t amount;)
PP(int16_t mode;)
{
	register MD *m;

	if (amount == -1L)
	{
		return xmfsize(&pmd, mode);
	} else
	{
		/*
		 *  Pass the request on to the internal routine.  If it was not able 
		 *  to grant the request, then abort.
		 */
	
		if ((m = ffit(amount, &pmd, mode)))
			return m->m_start & ~M_ALTFLAG;
		else
			return 0;
	}
}


/*
 *  xmalloc - 
 *
 *	Function 0x48	Malloc
 *
 *	Last modified	SCC	3 Jun 85
 */

/* 306: 00e183cc */
int32_t xmalloc(P(int32_t) amount)
PP(int32_t amount;)
{
	if (run->p_flags & PF_ALLOCTTRAM)
		return xmxalloc(amount, MX_PTTRAM);
	else
		return xmxalloc(amount, MX_STRAM);
}


/*
 *  xmfree - 
 *
 *	Function 0x49	Mfree
 */

/* 306: 00e183fe */
ERROR xmfree(P(int32_t) addr)
PP(int32_t addr;)
{
	register MD *p, **q;

	for (p = *(q = &pmd.mp_mal); p; p = *(q = &p->m_link))
		if (addr == (p->m_start & ~M_ALTFLAG))
			goto found;

	return E_IMBA;

found:
	*q = p->m_link;
	freeit(p, &pmd);

	return E_OK;
}


/* 306: 00e183fe */
VOID xmakeres(P(PD *) p)
PP(PD *p;)
{
	register MD *m, **q;

	for (m = *(q = &pmd.mp_mal); m; )
	{
		if (m->m_own == p)
		{
			m = m->m_link;
			*q = m;
		} else
		{
			q = &m->m_link;
			m = *q;
		}
	}
}


/*
 *   for each item in the allocated list that is owned by 'r', 
 *  free it
 */

/* 306: 00e1847c */
VOID xmfreall(P(PD *) r)
PP(PD *r;)
{
	register MD *m, **q;

	for (m = *(q = &pmd.mp_mal); m; )
	{
		if (m->m_own == r)
		{
			*q = m->m_link;
			freeit(m, &pmd);
			m = *q;
		} else
		{
			q = &m->m_link;
			m = *q;
		}
	}
}


/* 306: 00e184be */
VOID xmsetown(P(VOIDPTR) paddr, P(PD *) own)
PP(VOIDPTR paddr;)
PP(PD *own;)
{
	register MD *q;
	register VOIDPTR addr;
	
	addr = paddr;
	for (q = pmd.mp_mal; q != NULL; q = q->m_link)
	{
		if ((intptr_t)addr == (q->m_start & ~M_ALTFLAG))
		{
			q->m_own = own;
			return;
		}
	}
}


/* 306: 00e184f8 */
ERROR xmaddalt(P(char *) start, P(int32_t) len)
PP(char *start;)
PP(int32_t len;)
{
	register intptr_t s;
	register MD *q;
	
	s = (intptr_t)start;
	if ((s &= M_ALTFLAG))
	{
		s = 2 - s;
		start += s;
		len -= s;
	}
	len &= ~1L;
	if (len <= 0)
		return E_OK;
	if ((q = mgetmd()) == NULL)
		return ERR;
	q->m_start = (intptr_t)start | M_ALTFLAG;
	q->m_length = len;
	q->m_link = NULL;
	q->m_own = NULL;
	freeit(q, &pmd);
	
	return E_OK;
}


/* 306: 00e18570 */
VOID foldermsg(NOTHING)
{
	printstr("\033H*** OUT OF INTERNAL MEMORY:\033K\r\n*** USE FOLDR100.PRG TO GET MORE.\033K\r\n\033K\n\033K\n*** SYSTEM HALTED ***\033K");
	for (;;) ;
}


static VOID printstr(P(const char *) str)
PP(const char *str;)
{
	while (*str)
	{
#ifdef __ALCYON__
		trap13(0x00030002L, *str);
#else
		Bconout(2, *str);
#endif
		str++;
	}
}
