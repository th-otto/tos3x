/*  fsio.c - read/write routines for the file system			*/

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include "mem.h"
#include "btools.h"
#include <toserrno.h>

#undef min
#define min(a,b) (((a) < (b)) ? (a) : (b))

typedef VOID (*xfer) PROTO((int, char *, char *));

ERROR xrw PROTO((int wrtflg, OFD *p, long len, char *ubufr, xfer bufxfr));
VOID addit PROTO((OFD *p, long siz, int flg));
VOID usrio PROTO((int rwflg, int num, int strt, char *ubuf, DMD *dm));


/*
 *  ixread -
 *
 *	Last modified	SCC	26 July 85
 */

/* 306: 00e16236 */
ERROR ixread(P(OFD *)p, P(long) len, P(VOIDPTR) ubufr)
PP(register OFD *p;)
PP(long len;)
PP(VOIDPTR ubufr;)
{
	long maxlen;

	/* Make sure file not opened as write only. */
	if (p->o_mod == WO_MODE)
		return E_ACCDN;

	if (len > (maxlen = p->o_fileln - p->o_bytnum))
		len = maxlen;

	if (len > 0)
		return xrw(0, p, len, ubufr, xfr2usr);

	return 0;						/* zero bytes read for zero requested */
}



/*
 *  xlseek -
 *	seek to byte position n on file with handle h
 *
 *  Function 0x42	Fseek
 *
 *	Error returns
 *		EIHNDL
 *		EINVFN
 *		ixlseek()
 */

/* 306: 00e16cf6 */
ERROR xlseek(P(long) n, P(int16_t) h, P(int16_t) flg)
PP(long n;)
PP(int16_t h;)
PP(int16_t flg;)
{
	OFD *f;

	if (!(f = getofd(h)))
		return E_IHNDL;

	if (flg == 2)
		n += f->o_fileln;
	else if (flg == 1)
		n += f->o_bytnum;
	else if (flg)
		return E_INVFN;

	return ixlseek(f, n);
}


/*
 *  ixlseek -
 *	file position seek
 *
 *	Error returns
 *		ERANGE
 *		EINTRN
 *
 *	Last modified	LTG	31 Jul 85
 *
 *	NOTE:	This function returns ERANGE and EINTRN errors, which are new 
 *		error numbers I just made up (that is, they were not defined 
 *		by the BIOS or by PC DOS).
 */

/* 306: 00e16d58 */
ERROR ixlseek(P(OFD *) p, P(long) n)
PP(register OFD *p;)								/*  file descriptor for file in use */
PP(long n;)									/*  number of bytes to seek     */
{
	int clnum, clx, curnum, i;
	int curflg;
	register DMD *dm;

	if (n > p->o_fileln)
		return E_RANGE;

	if (n < 0)
		return E_RANGE;

	dm = p->o_dmd;
	if (!n)
	{
		clx = 0;
		p->o_curbyt = 0;
		goto fillin;
	}

	/* do we need to start from the beginning ? */

	if (((!p->o_curbyt) || (p->o_curbyt == dm->m_clsizb)) && p->o_bytnum)
		curflg = 1;
	else
		curflg = 0;

	clnum = divmod((int16_t *)&p->o_curbyt, n, dm->m_clblog);

	if (p->o_curcl && (n >= p->o_bytnum))
	{
		curnum = p->o_bytnum >> dm->m_clblog;
		clnum -= curnum;
		clnum += curflg;

		clx = p->o_curcl;

	} else
	{
		clx = p->o_strtcl;
	}
	
	for (i = 1; i < clnum; i++)
		if ((clx = getcl(clx, dm)) == -1)
			return -1;

	/* go one more except on cluster boundary */

	if (p->o_curbyt && clnum)
		clx = getcl(clx, dm);

  fillin:p->o_curcl = clx;
	p->o_currec = cl2rec(clx, dm);
	p->o_bytnum = n;

	return n;
}



/*
 *  ixwrite -
 */

/* 306: 00e16286 */
ERROR ixwrite(P(OFD *) p, P(long) len, P(VOIDPTR) ubufr)
PP(OFD *p;)
PP(long len;)
PP(VOIDPTR ubufr;)
{
	return xrw(1, p, len, ubufr, usr2xfr);
}



/*
 *  makopn - make an open file for sft handle h 
 *
 *	Last modified	SCC	8 Apr 85
 */

/* 306: 00e162ae */
ERROR makopn(P(FCB *) f, P(DND *) dn, P(FH) h, P(int16_t) mod)
PP(FCB *f;)
PP(DND *dn;)
PP(FH h;)
PP(int mod;)
{
	register OFD *p;
	register OFD *p2;
	register DMD *dm;

	dm = dn->d_drv;

	if (!(p = mgetofd()))
		return E_NSMEM;

	p->o_mod = mod;						/*  set mode            */
	p->o_dmd = dm;						/*  link OFD to media       */
	sft[h - NUMSTD].f_ofd = p;
	p->o_usecnt = 0;					/*  init usage          */
	p->o_curcl = 0;						/*  init file pointer info  */
	p->o_curbyt = 0;					/*  "               */
	p->o_dnode = dn;					/*  link to directory       */
	p->o_dirfil = dn->d_ofd;			/*  link to dir's ofd       */
	p->o_dirbyt = dn->d_ofd->o_bytnum - 32;	/*  offset of fcb in dir */

	for (p2 = dn->d_files; p2; p2 = p2->o_link)
		if (p2->o_dirbyt == p->o_dirbyt)
			break;						/* same dir, same dcnt */

	p->o_link = dn->d_files;
	dn->d_files = p;

	if (p2)
	{									/* steal time,date,startcl,fileln */
		xmovs(12, &p2->o_td.time, &p->o_td.time);
		/* not used yet... */
		p2->o_thread = p;
	} else
	{
		p->o_strtcl = f->f_clust;		/*  1st cluster of file */
		swp68(p->o_strtcl);
		p->o_fileln = f->f_fileln;		/*  init length of file */
		swp68l(p->o_fileln);
		p->o_td.date = f->f_td.date;
		p->o_td.time = f->f_td.time;
	}

	return h;
}



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
