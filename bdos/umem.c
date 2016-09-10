/*  umem.c - user memory management interface routines			*/
/*  xmalloc, xmfreblk, xsetblk						*/


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

#define	STATIUMEM	TRUE

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
	getmpb(&pmd);
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
	MD *p, *q, *p1;								/* free list is composed of MD's */
	BOOLEAN maxflg;
	intptr_t maxval;

#if	STATIUMEM
	++ccffit;
#endif

	if ((q = mp->mp_rover) == 0)		/*  get rotating pointer    */
	{
#if	DBGIUMEM
		kprintf("ffit: null rover\n");
#endif
		return NULL;
	}

	maxval = 0;
	maxflg = (amount == -1 ? TRUE : FALSE);

	p = q->m_link;						/*  start with next MD      */

	do									/* search the list for an MD with enough space */
	{

		if (p == 0)
		{								/*  at end of list, wrap back to start  */
			q = (MD *) & mp->mp_mfl;	/*  q => mfl field  */
			p = q->m_link;				/*  p => 1st MD     */
		}

		if ((!maxflg) && (p->m_length >= amount))
		{
			/*  big enough  */

			if (p->m_length == amount)
				/* take the whole thing */
				q->m_link = p->m_link;
			else
			{							/* break it up - 1st allocate a new
										   MD to describe the remainder */

				/*********** TBD **********
				*  Nicer Handling of This *
				*         Situation       *
				**************************/
				if ((p1 = getmd()) == NULL)
				{
#if	DBGIUMEM
					kprintf("ffit: Null Mget\n");
#endif
					return NULL;
				}

				/*  init new MD  */

				p1->m_length = p->m_length - amount;
				p1->m_start = p->m_start + amount;
				p1->m_link = p->m_link;

				/*  adjust allocated block  */

				p->m_length = amount;
				q->m_link = p1;
			}

			/*  link allocate block into allocated list,
			   mark owner of block, & adjust rover  */

			p->m_link = mp->mp_mal;
			mp->mp_mal = p;

			p->m_own = run;

			mp->mp_rover = (q == (MD *) & mp->mp_mfl ? q->m_link : q);

			return p;					/* got some */
		} else if (p->m_length > maxval)
			maxval = p->m_length;

		p = (q = p)->m_link;

	} while (q != mp->mp_rover);

	/*  return either the max, or 0 (error)  */

#if	DBGIUMEM
	if (!maxflg)
		kprintf("ffit: Not Enough Contiguous Memory\n");
#endif
	return maxflg ? ((MD *) maxval) : ((MD *)0);
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
PP(int32_t len;)								/*  length of block to free */
{
	MD *m, *p;

	UNUSED(n);
	/*
	 * Traverse the list of memory descriptors looking for this block.
	 */

	for (p = pmd.mp_mal; p; p = p->m_link)
		if ((intptr_t) blk == p->m_start)
			break;

	/*
	 * If block address doesn't match any memory descriptor, then abort.
	 */

	if (!p)
		return E_IMBA;

	/*
	 *  If the caller is not shrinking the block size, then abort.
	 */

	if (p->m_length < len)
		return E_GSBF;

	/*
	 *  Always shrink to an even word length.
	 */

	if (len & 1)
		len++;

	/*
	 *  Create a memory descriptor for the freed portion of memory.
	 */

	m = getmd();
	/*
	 * what if 0? *
	 */
#if	DBGUMEM
	if (m == NULL)
		panic("umem.c/xsetblk: Null Return From MGET\n");
#endif

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
PP(MD *m;)
PP(MPB *mp;)
{
	MD *p, *q;

#if	STATIUMEM
	++ccfreeit;
#endif

	q = 0;

	for (p = mp->mp_mfl; p; p = (q = p)->m_link)
		if (m->m_start <= p->m_start)
			break;

	m->m_link = p;

	if (q)
		q->m_link = m;
	else
		mp->mp_mfl = m;

	if (!mp->mp_rover)
		mp->mp_rover = m;

	if (p)
		if (m->m_start + m->m_length == p->m_start)
		{								/* join to higher neighbor */
			m->m_length += p->m_length;
			m->m_link = p->m_link;

			if (p == mp->mp_rover)
				mp->mp_rover = m;

			xmfreblk(p);
		}

	if (q)
		if (q->m_start + q->m_length == m->m_start)
		{								/* join to lower neighbor */
			q->m_length += m->m_length;
			q->m_link = m->m_link;

			if (m == mp->mp_rover)
				mp->mp_rover = q;

			xmfreblk(m);
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
	if (run->p_flags & PF_TTRAMMEM)
		return xmxalloc(amount, MX_PREFTTRAM);
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
	MD *p, **q;

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
	if ((q = getmd()) == NULL)
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
		bconout(2, *str);
#endif
		str++;
	}
}
