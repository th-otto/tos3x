/*  fsopnclo.c - open/close/create/delete routines for file system	*/

#include "tos.h"
#include <ostruct.h>
#include <toserrno.h>
#include "fs.h"
#include "bios.h"
#include "mem.h"
#include "btools.h"


FTAB *sftsrch PROTO((int field, VOIDPTR ptr));


/*
 *  makofd -
 */

/* 306: 00e161de */
OFD *makofd(P(DND *) p)
PP(register DND *p;)
{
	register OFD *f;

	if (!(f = mgetofd()))
		return NULL;

	f->o_strtcl = p->d_strtcl;
	f->o_fileln = 0x7fffffffL;
	f->o_dirfil = p->d_dirfil;
	f->o_dnode = p->d_parent;
	f->o_dirbyt = p->d_dirpos;
	f->o_td.date = p->d_td.date;
	f->o_td.time = p->d_td.time;
	f->o_dmd = p->d_drv;

	return f;
}



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
		swp68(&p->o_strtcl);
		p->o_fileln = f->f_fileln;		/*  init length of file */
		swp68l(&p->o_fileln);
		p->o_td.date = f->f_td.date;
		p->o_td.time = f->f_td.time;
	}

	return h;
}



/*	
 *  ixcreat - internal routine for creating files
 */

/* 306: 00e163be */
ERROR ixcreat(P(const char *) name, P(int8_t) attr)
PP(const char *name;)
PP(int8_t attr;)
{
	register DND *dn;
	register OFD *fd;
	FCB *f;
	const char *s;
	char n[2];
	char a[11];
	int i, f2;
	int32_t pos;
	ERROR rc;
	
	n[0] = 0xe5;
	n[1] = 0;
	
	/* first find path */
	
	if ((dn = findit(name, &s, 0)) == NULL)
		return E_PTHNF;
	
	if (contains_dots(s, 0) != 0)
		return E_PTHNF;
	
	if (ffhndl() < 0)
		return E_NHNDL;
	
	if (!(fd = dn->d_ofd))
		fd = makofd(dn);					/* makofd() also updates dn->d_ofd */
	
	/* is it already there ? */
	pos = 0;
	if ((f = scan(dn, s, -1, &pos)))
	{
		if ((f->f_attrib & (FA_DIREC | FA_RDONLY)) || (attr == FA_DIREC))
			/*  subdir or read only  */
			return E_ACCDN;
		pos -= 32;
		ixdel(dn, f, pos);
	} else
	{
		pos = 0;
	}
	
	/* now scan for empty space */
	while (!(f = scan(dn, n, -1, &pos)))
	{
		/*  not in current dir, need to grow  */
		if ((int) (fd->o_curcl) < 0)
			/*  but can't grow root  */
			return E_ACCDN;
		
		if (nextcl(fd, 1))
			return E_ACCDN;
		
		f = dirinit(dn);
		pos = 0;
	}
	
	builds(s, a);
	pos -= 32;
	f->f_attrib = attr;
	for (i = 0; i < 10; i++)
		f->f_fill[i] = 0;
	f->f_td.time = time;
	swp68(&f->f_td.time);
	f->f_td.date = date;
	swp68(&f->f_td.date);
	f->f_clust = 0;
	f->f_fileln = 0;
	ixlseek(fd, pos);
	ixwrite(fd, 11L, a);					/* write name, set dirty flag */
	ixclose(fd, CL_DIR);					/* partial close to flush */
	ixlseek(fd, pos);
	s = (char *) ixread(fd, 32L, NULL);
	f2 = rc = opnfil((FCB *) s, dn, (f->f_attrib & FA_RDONLY) ? RO_MODE : RW_MODE);
	
	if (rc < 0)
		return rc;
	
	getofd(f2)->o_flag |= O_DIRTY;
	
	return f2;
}


/*
 *  ixopen - open a file 
 *
 *  returns
 *	<0 = error
 *	>0 = file handle
 */

/* 306: 00e16670 */
ERROR ixopen(P(const char *) name, P(int16_t) mode)
PP(const char name;)
PP(int16_t mode;)
{
	FCB *f;
	DND *dn;
	const char *s;
	int32_t pos;

	/* first find path */
	if ((dn = findit(name, &s, 0)) == NULL)
		return E_FILNF;

	/* 
	 *  now scan the directory file for a matching filename 
	 */

	pos = 0;
	if (!(f = scan(dn, s, FA_NORM, &pos)))
		return E_FILNF;

	/* Check to see if the file is read only */
	if ((f->f_attrib & FA_RDONLY) && (mode != RO_MODE))
		return E_ACCDN;

	return opnfil(f, dn, mode);
}



/*
 *  xchmod - change/get attrib of path p
 *		if wrt = 1, set; else get
 *
 *	Function 0x43	Fattrib
 *
 *	Error returns
 *		EPTHNF
 *		EFILNF
 *
 */

/* 306: 00e166e6 */
char xchmod(P(const char *) p, P(int16_t) wrt, P(char) mod)
PP(const char *p;)
PP(int16_t wrt;)
PP(char mod;)
{
	OFD *fd;
	DND *dn;
	const char *s;
	int32_t pos;

	if ((ERROR) (dn = findit(p, &s, 0)) < 0)
		return (ERROR)dn;
	if (!dn)
		return E_PTHNF;

	pos = 0;

	if (!scan(dn, s, FA_NORM, &pos))
		return E_FILNF;

	pos -= 21;							/* point at attribute in file */
	fd = dn->d_ofd;
	ixlseek(fd, pos);
	if (!wrt)
		ixread(fd, 1L, &mod);
	else
	{
		ixwrite(fd, 1L, &mod);
		ixclose(fd, CL_DIR);			/* for flush */
	}
	return mod;
}


/*
 *  xgsdtof - get/set date/time of file into of from buffer
 *
 *	Function 0x57	Fdatime
 */

/* 306: 00e167c4 */
ERROR xgsdtof(P(uint16_t *) buf, P(FH) h, P(int16_t) wrt)
PP(uint16_t *buf;)
PP(FH h;)
PP(int16_t wrt;)
{
	register OFD *f;
	register OFD *p;
	uint16_t b[2];

	f = getofd(h);
	if (f == NULL || ((ERROR)f) < 0)
		return E_IHNDL;

	if (!wrt)
	{
		buf[0] = f->o_td.time;
		buf[1] = f->o_td.date;
		return E_OK;
	}

	b[0] = buf[0];
	b[1] = buf[1];
	swp68(&b[0]);
	swp68(&b[1]);

	for (p = f->o_dnode->d_files; p; p = p->o_link)
	{
		p->o_td.time = b[0];
		p->o_td.date = b[1];
		p->o_flag |= O_DIRTY;
	}

	return E_OK;
}



/*	
 *  xunlink - unlink (delete) a file
 *
 *	Function 0x41 	Fdelete
 *
 *  returns
 *	EFILNF	file not found
 *	EACCDN	access denied
 *	ixdel()
 *
 */

/* 306: 00e16876 */
ERROR xunlink(P(const char *) name)
PP(const char *name;)								/*  path name of file to delete     */
{
	register DND *dn;
	register FCB *f;
	const char *s;
	int32_t pos;

	/* first find path */

	if ((ERROR) (dn = findit(name, &s, 0)) < 0)
		return (ERROR)dn;
	if (!dn)
		return E_FILNF;

	/* now scan for filename */

	pos = 0;
	if (!(f = scan(dn, s, FA_NORM, &pos)))
		return E_FILNF;

	if (f->f_attrib & FA_RDONLY)
		return E_ACCDN;

	pos -= 32;

	return ixdel(dn, f, pos);
}


/*
 *  ixdel - internal delete file.
 *
 *  Traverse the list of files open for this directory node.
 *  If a file is found that has the same position in the directory as the one 
 *  we are to delete, then scan the system file table to see if this process is 
 *  then owner.  If so, then close it, otherwise abort.
 *  
 *  NOTE: 	both 'for' loops scan for the entire length of their 
 *		respective data structures, and do not drop out of the loop on 
 *		the first occurence of a match.
 * 	Used by
 * 		ixcreat()
 * 		xunlink()
 * 		xrmdir()
 * 
 */

/* 306: 00e168e6 */
ERROR ixdel(P(DND *) dn, P(FCB *) f, P(long) pos)
PP(DND *dn;)
PP(FCB *f;)
PP(long pos;)
{
	register OFD *fd;
	DMD *dm;
	register CLNO n2;
	CLNO n;
	char c;

	for (fd = dn->d_files; fd; fd = fd->o_link)
		if (fd->o_dirbyt == pos)
			for (n = 0; n < OPNFILES; n++)
				if (sft[n].f_ofd == fd)
				{
					if (sft[n].f_own == run)
						ixclose(fd, 0);
					else
						return E_ACCDN;
				}
	/*
	 * Traverse this file's chain of allocated clusters, freeing them.
	 */

	dm = dn->d_drv;
	n = f->f_clust;
	swp68(&n);

	while (n && (n != -1))
	{
		n2 = getcl(n, dm);
		clfix(n, 0, dm);
		n = n2;
	}

	/*
	 * Mark the directory entry as erased.
	 */

	fd = dn->d_ofd;
	ixlseek(fd, pos);
	c = 0xe5;
	ixwrite(fd, 1L, &c);
	ixclose(fd, CL_DIR);

	/*
	 *	NOTE	that the preceding routines that do physical disk operations 
	 *	will 'longjmp' on failure at the BIOS level, thereby allowing us to 
	 *	simply return with E_OK.
	 */

	return E_OK;
}


/*
 *  xrename - rename a file,
 *	oldpath p1, new path p2
 *
 *	Function 0x56	Frename
 *
 *	Error returns
 *		EPTHNF
 *
 */

/* 306: 00e16a06 */
ERROR xrename(P(int16_t) n, P(const char *) p1, P(const char *)p2)	/*+ rename file, old path p1, new path p2 */
PP(int16_t n;)									/*  not used                */
PP(const char *p1;)
PP(const char *p2;)
{
	register OFD *fd2;
	OFD *f1, *fd;
	FCB *f;
	DND *dn1, *dn2;
	const char *s1;
	const char *s2;
	char buf[11];
	int hnew, att;
	ERROR rc, h1;

	UNUSED(n);

	if (!ixsfirst(p2, 0, (DTAINFO *) 0L))
		return E_ACCDN;

	if ((dn1 = findit(p1, &s1, 0)) == NULL)
		return E_PTHNF;

	if ((ERROR) (dn2 = findit(p2, &s2, 0)) < 0)
		return (ERROR)dn2;
	if (!dn2)
		return E_PTHNF;

	if ((h1 = xopen(p1, 2)) < 0L)
		return h1;

	f1 = getofd((int) h1);

	fd = f1->o_dirfil;
	buf[0] = 0xe5;
	ixlseek(fd, f1->o_dirbyt);

	if (dn1 != dn2)
	{
		/* get old attribute */
		f = (FCB *) ixread(fd, 32L, NULL);
		att = f->f_attrib;
		/* erase (0xe5) old file */
		ixlseek(fd, f1->o_dirbyt);
		ixwrite(fd, 1L, buf);

		/* copy time/date/clust, etc. */

		ixlseek(fd, f1->o_dirbyt + 22);
		ixread(fd, 10L, buf);
		hnew = xcreat(p2, att);
		fd2 = getofd(hnew);
		ixlseek(fd2->o_dirfil, fd2->o_dirbyt + 22);
		ixwrite(fd2->o_dirfil, 10L, buf);
		fd2->o_flag &= ~O_DIRTY;
		xclose(hnew);
		ixclose(fd2->o_dirfil, CL_DIR);
	} else
	{
		builds(s2, buf);
		ixwrite(fd, 11L, buf);
	}

	if ((rc = xclose((int) h1)) < 0L)
		return rc;

	return ixclose(fd, CL_DIR);
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
 *  match1 - check for bad chars in path name
 *	check thru test string to see if any character in the ref str is found
 *	(utility routine for ixcreat())
 *	by scc
 */

BOOLEAN match1(P(const char *) ref, P(const char *) test)
PP(register const char *ref;)
PP(const char *test;)
{
	register const char *t;

	while (*ref)
	{
		for (t = test; *t; t++)
			if (*t == *ref)
				return TRUE;
		ref++;
	}

	return FALSE;
}


/*
 *  used in calls to sftsrch to distinguish which field we are matching on
 */

#define	SFTOFD		0
#define	SFTOWNER	1

/*
 *  SFTOFDSRCH - search sft for entry with matching OFD ptr
 *	call sftsrch with correct parms
 */

#define	SFTOFDSRCH(o)	sftsrch( SFTOFD , (char *) o )

/*
 *  SFTOWNSRCH - search sft for entry with matching PD
 *	call sftsrch with correct parms
 */

#define	SFTOWNSRCH(p)	sftsrch( SFTOWN , (char *) p )


/*
 *  sftsrch - search the sft for an entry with the specified OFD
 *  returns:
 *	ptr to the matching sft, or
 *	NULL
 */

FTAB *sftsrch(P(int) field, P(VOIDPTR) ptr)
PP(int field;)								/*  which field to match on     */
PP(VOIDPTR ptr;)								/*  ptr to match on         */
{
	register FTAB *sftp;						/*  scan ptr for sft            */
	register int i;
	register OFD *ofd;
	register PD *pd;

	switch (field)
	{
	case SFTOFD:
		for (i = 0, sftp = sft, ofd = (OFD *) ptr; i < OPNFILES && sftp->f_ofd != ofd; ++i, ++sftp)
			;
		break;
	case SFTOWNER:
		for (i = 0, sftp = sft, pd = (PD *) ptr; i < OPNFILES && sftp->f_own != pd; ++i, ++sftp)
			;
		break;
	default:
		i = OPNFILES;					/* setup for null return  */
	}
	return i >= OPNFILES ? (FTAB *) NULL : sftp;
}
