/*  fsio.c - read/write routines for the file system			*/

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include "mem.h"
#include "btools.h"
#include <toserrno.h>

#undef min
#define min(a,b) (((a) < (b)) ? (a) : (b))

VOID addit PROTO((OFD *p, long siz, int flg));
VOID usrio PROTO((int rwflg, int num, int strt, char *ubuf, DMD *dm));


/*
 *  xrw - read/write 'len' bytes from/to the file indicated by the OFD at 'p'.
 *
 *  details
 *	We wish to do the i/o in whole clusters as much as possible.
 *	Therefore, we break the i/o up into 5 sections.  Data which occupies 
 *	part of a logical record (e.g., sector) with data not in the request 
 *	(both at the start and the end of the the request) are handled
 *	separateley and are called header (tail) bytes.  Data which are
 *	contained complete in sectors but share part of a cluster with data not
 *	in the request are called header (tail) records.  These are also
 *	handled separately.  In between handling of header and tail sections,
 *	we do i/o in terms of whole clusters.
 *
 *  returns
 *	nbr of bytes read/written from/to the file.
 */

/* 306: 00e14f10 */
ERROR xrw(P(int) wrtflg, P(OFD *) p, P(long) len, P(char *) ubufr, P(xfer) bufxfr)
PP(int wrtflg;)
PP(OFD *p;)
PP(long len;)
PP(char *ubufr;)
PP(xfer bufxfr;)
{
	register DMD *dm;
	char *bufp;
	int16_t bytn;
	int recn, lenxfr, lentail, num;
	int hdrrec, lsiz;
	int16_t tailrec;
	int last, nrecs, lflg;								/* multi-sector variables */
	long nbyts;
	long rc, bytpos, lenrec, lenmid;

	/*
	 *  determine where we currently are in the filef
	 */

	dm = p->o_dmd;						/*  get drive media descriptor  */

	bytpos = p->o_bytnum;				/*  starting file position  */

	/*
	 *  get logical record number to start i/o with
	 * (bytn will be byte offset into sector # recn)
	 */

	recn = divmod(&bytn, (long) p->o_curbyt, dm->m_rblog);
	recn += p->o_currec;

	/*
	 *  determine "header" of request.
	 */

	if (bytn)							/* do header */
	{									/*
										 *  xfer len is
										 * min( #bytes req'd , #bytes left in current record )
										 */
		lenxfr = min(len, dm->m_recsiz - bytn);
		bufp = getrec(recn, dm, wrtflg);	/*  get desired record  */
		addit(p, (long) lenxfr, 1);		/*  update ofd      */
		len -= lenxfr;					/*  nbr left to do  */
		recn++;							/*    starting w/ next  */

		if (!ubufr)
		{
			rc = (long) (bufp + bytn);	/* ???????????  */
			goto exit;
		}

		(*bufxfr) (lenxfr, bufp + bytn, ubufr);
		ubufr += lenxfr;
	}

	/*
	 *  "header" complete.  See if there is a "tail".  
	 *  After that, see if there is anything left in the middle.
	 */

	lentail = len & dm->m_rbm;

	if ((lenmid = len - lentail) != 0)			/*  Is there a Middle ? */
	{
		hdrrec = recn & dm->m_clrm;

		if (hdrrec)
		{
			/*  if hdrrec != 0, then we do not start on a clus bndy;
			 * so determine the min of (the nbr sects 
			 * remaining in the current cluster) and (the nbr 
			 * of sects remaining in the file).  This will be 
			 * the number of header records to read/write.
			 */

			hdrrec = (dm->m_clsiz - hdrrec);
			if (hdrrec > lenmid >> dm->m_rblog)
				hdrrec = lenmid >> dm->m_rblog;

			usrio(wrtflg, hdrrec, recn, ubufr, dm);
			ubufr += (lsiz = hdrrec << dm->m_rblog);
			lenmid -= lsiz;
			addit(p, (long) lsiz, 1);
		}

		/* 
		 *  do whole clusters 
		 */

		lenrec = lenmid >> dm->m_rblog;	/* nbr of records  */
		num = divmod(&tailrec, lenrec, dm->m_clrlog);	/* nbr of clusters */

		last = nrecs = nbyts = lflg = 0;

		while (num--)					/*  for each whole cluster...   */
		{
			rc = nextcl(p, wrtflg);

			/* 
			 *  if eof or non-contiguous cluster, or last cluster 
			 * of request, 
			 * then finish pending I/O 
			 */

			if ((!rc) && (p->o_currec == last + nrecs))
			{
				nrecs += dm->m_clsiz;
				nbyts += dm->m_clsizb;
				if (!num)
					goto mulio;
			} else
			{
				if (!num)
					lflg = 1;
			  mulio:if (nrecs)
					usrio(wrtflg, nrecs, last, ubufr, dm);
				ubufr += nbyts;
				addit(p, nbyts, 0);
				if (rc)
					goto eof;
				last = p->o_currec;
				nrecs = dm->m_clsiz;
				nbyts = dm->m_clsizb;
				if ((!num) && lflg)
				{
					lflg = 0;
					goto mulio;
				}
			}
		}								/*  end while  */

		/* 
		 *  do "tail" records 
		 */

		if (tailrec)
		{
			if (nextcl(p, wrtflg))
				goto eof;
			lsiz = tailrec << dm->m_rblog;
			addit(p, (long) lsiz, 1);
			usrio(wrtflg, tailrec, p->o_currec, ubufr, dm);
			ubufr += lsiz;
		}
	}

	/* 
	 * do tail bytes within this cluster 
	 */

	if (lentail)
	{
		recn = divmod(&bytn, (long) p->o_curbyt, dm->m_rblog);

		if ((!recn) || (recn == dm->m_clsiz))
		{
			if (nextcl(p, wrtflg))
				goto eof;
			recn = 0;
		}

		bufp = getrec(p->o_currec + recn, dm, wrtflg);
		addit(p, (long) lentail, 1);

		if (!ubufr)
		{
			rc = (long) bufp;
			goto exit;
		}

		(*bufxfr) (lentail, bufp, ubufr /*, wrtflg */);
	}
	/*  end tail bytes  */
eof:
	rc = p->o_bytnum - bytpos;
exit:
 	return rc;
}


/*
 *  addit -
 *	update the OFD for the file to reflect the fact that 'siz' bytes
 *	have been written to it.
 */

VOID addit(P(OFD *) p, P(long) siz, P(int) flg)
PP(register OFD *p;)
PP(register long siz;)
PP(int flg;)								/* update curbyt ? (yes if less than 1 cluster transferred) */
{
	p->o_bytnum += siz;

	if (flg)
		p->o_curbyt += siz;

	if (p->o_bytnum > p->o_fileln)
	{
		p->o_fileln = p->o_bytnum;
		p->o_flag |= O_DIRTY;
	}
}


/*
 *  usrio -
 *
 *	Last modified	SCC	10 Apr 85
 *
 *	NOTE:	rwabs() is a macro that includes a longjmp() which is executed 
 *		if the BIOS returns an error, therefore usrio() does not need 
 *		to return any error codes.
 */

VOID usrio(P(int) rwflg, P(int) num, P(int) strt, P(char *) ubuf, P(DMD *) dm)
PP(int rwflg;)
PP(int num;)
PP(int strt;)
PP(char *ubuf;)
PP(register DMD *dm;)
{
	register BCB *b;

	for (b = bufl[1]; b; b = b->b_link)
		if ((b->b_bufdrv == dm->m_drvnum) && (b->b_bufrec >= strt) && (b->b_bufrec < strt + num))
			flush(b);

	rwabs(rwflg, ubuf, num, strt + dm->m_recoff[2], dm->m_drvnum);
}
