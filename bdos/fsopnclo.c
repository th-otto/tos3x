/*  fsopnclo.c - open/close/create/delete routines for file system	*/

/*
** NOTE:
**	mods with "SCC.XX.NN" are mods which try to merge fixes to a special
**	post 1.0 / pre 1.1 version.  The notation refers to a DRI internal
**	document (see SCC), which is a change log.  SCC refers to the
**	originator of the fix.  The XX refers to the module in which the
**	fix was originally made, fs.c (FS), sup.c (SUP), etc.  The NN is
**	the fix number to that module as indicated on the change log.  For
**	the most part, these numbers are meaningless, and serve only to 
**	correspond code to particular problems.
**
**  mods
**     date     who mod 		fix/change/note
**  ----------- --  ------------------	-------------------------------
**  06 May 1986 ktb M01.01.SCC.FS.02	ixcreat(): rescan.
**  21 Jul 1986 ktb M01.01.0721.01	ixcreat(): check for bad chars
**  30 Jul 1986 ktb M01.01.0730.01	deleting entries from the sft had
**					problems if there were dup'd entries
**					pointing to the same OFD
**  15 Sep 1986 scc M01.01.0915.01	ixcreat(): disallow creation of subdir
**					if file by same name exists
**  22 Oct 1986 scc M01.01.1022.01	xclose(): range check the handle coming in
**  23 Oct 1986 scc M01.01.1023.01	xclose(): check for closing a standard handle
**					that was already closed
**  23 Oct 1986 scc M01.01.1023.03	sftdel() and sftosrch() erroneously used NULL
**					rather than NULLPTR.
**
**  12 Dec 1986 scc M01.01.1212.01	modified ixcreat(), ixopen(), and xunlink() to
**					check for a negative error return from findit().
**
**  14 Dec 1986 scc M01.01.1214.01	Further modification of M01.01.1212.01 to check
**					for 0 return (indicating BDOS level error).
*/


#include "tos.h"
#include <ostruct.h>
#include <toserrno.h>
#include <fcntl.h>
#include "fs.h"


FTAB *sftsrch PROTO((int field, VOIDPTR ptr));
VOID sftdel PROTO((FTAB *sftp));


/*	
 *  ixcreat - internal routine for creating files
 */

/* 306: 00e163be */
ERROR ixcreat(P(const char *) fname, P(int8_t) attr)
PP(const char *fname;)
PP(int8_t attr;)
{
	register DND *dn;
	register OFD *fd;
	FCB *f;
	const char *s;
	char n[2];
	char a[11];
	int i, f2;
	long pos, rc;
	
	n[0] = ERASE_MARKER;
	n[1] = 0;
	
	/* first find path */
	
	if ((intptr_t) (dn = findit(name, &s, 0)) == 0)
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
	swp68(f->f_td.time);
	f->f_td.date = date;
	swp68(f->f_td.date);
	f->f_clust = 0;
	f->f_fileln = 0;
	ixlseek(fd, pos);
	ixwrite(fd, 11L, a);					/* write name, set dirty flag */
	ixclose(fd, CL_DIR);					/* partial close to flush */
	ixlseek(fd, pos);
	s = (char *) ixread(fd, 32L, NULL);
	f2 = rc = opnfil((FCB *) s, dn, (pf->f_attrib & FA_RDONLY) ? O_RDONLY : O_RDWR);
	
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
ERROR ixopen(P(const char *) fname, P(int16_t) mode)
PP(const char *fname;)
PP(int16_t mode;)
{
	FCB *f;
	DND *dn;
	char *s;

	long pos;

	/* first find path */
	if ((long) (dn = findit(name, &s, 0)) < 0)
		return (dn);
	if (!dn)
		return (EFILNF);

	/* 
	 **  now scan the directory file for a matching filename 
	 */

	pos = 0;
	if (!(f = scan(dn, s, FA_NORM, &pos)))
		return E_FILNF;

	/* Check to see if the file is read only */
	if ((f->f_attrib & 1) && (mod != 0))
		return E_ACCDN;

	return opnfil(f, dn, mod);
}



/*
**  opnfil - does the real work in opening a file
**
**	Error returns
**		ENHNDL
**
**	NOTES:
**		make a pointer to the ith entry of sft 
**		make i a register int.
*/

ERROR opnfil(P(FCB *) f, P(DND *) dn, P(int16_t) mod)
PP(FCB *f;)
PP(DND *dn;)
PP(int16_t mod;)
{
	register int i;
	int h;

	/* find free sft handle */
	for (i = 0; i < OPNFILES; i++)
		if (!sft[i].f_own)
			break;

	if (i == OPNFILES)
		return (ENHNDL);

	sft[i].f_own = run;
	sft[i].f_use = 1;
	h = i + NUMSTD;

	return (makopn(f, dn, h, mod));
}


/*
**  makopn - make an open file for sft handle h 
**
**	Last modified	SCC	8 Apr 85
*/

ERROR makopn(P(FCB *) f, P(DND *) dn, P(FH) h, P(int16_t) mod)
PP(FCB *f;)
PP(DND *dn;)
PP(FH h;)
PP(int mod;)
{
	register OFD *p;
	register OFD *p2;
	DMD *dm;

	dm = dn->d_drv;

	if (!(p = MGET(OFD)))
		return (ENSMEM);

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
		xmovs(12, &p2->o_time, &p->o_time);
		/* not used yet... TBA ******** *//*<<<<<<<<<<<<< */
		p2->o_thread = p;
	} else
	{
		p->o_strtcl = f->f_clust;		/*  1st cluster of file */
		swp68(p->o_strtcl);
		p->o_fileln = f->f_fileln;		/*  init length of file */
		swp68l(p->o_fileln);
		p->o_date = f->f_date;
		p->o_time = f->f_time;
	}

	return (h);
}


/*
**  xclose - Close a file.
**
**	Function 0x3E	f_close
**
**	Error returns
**		EIHNDL
**		ixclose()
**
**	SCC:	I have added 'rc' to allow return of status from ixclose().  I 
**		do not yet know whether it is appropriate to perform the 
**		operations inside the 'if' statement following the invocation 
**		of ixclose(), but I am leaving the flow of control intact.
*/

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
			return (E_OK);
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
**  ixclose -
**
**	Error returns
**		EINTRN
**
**	Last modified	SCC	10 Apr 85
**
**	NOTE:	I'm not sure that returning immediatly upon an error from 
**		ixlseek() is the right thing to do.  Some data structures may 
**		not be updated correctly.  Watch out for this!
**		Also, I'm not sure that the EINTRN return is ok.
*/

#define CL_DIR 0x0002					/* this is a directory file, flush, do not free */
#define CL_FULL 0x0004					/* even though its a directory, full close */

ERROR ixclose(P(OFD *) fd, P(int) part)
PP(register OFD *fd;)
PP(int part;)
{
	OFD *p, **q;
	long tmp;
	register int i;
	BCB *b;


	if (fd->o_flag & O_DIRTY)
	{
		ixlseek(fd->o_dirfil, fd->o_dirbyt + 22);

		swp68(fd->o_strtcl);
		swp68l(fd->o_fileln);

		if (part & CL_DIR)
		{
			tmp = fd->o_fileln;			/* [1] */
			fd->o_fileln = 0;
			ixwrite(fd->o_dirfil, 10L, (char *) &fd->o_time);
			fd->o_fileln = tmp;
		} else
			ixwrite(fd->o_dirfil, 10L, (char *) &fd->o_time);

		swp68(fd->o_strtcl);
		swp68l(fd->o_fileln);
	}

	if ((!part) || (part & CL_FULL))
	{
		q = &fd->o_dnode->d_files;

		for (p = *q; p; p = *(q = &p->o_link))
			if (p == fd)
				break;

		/* someone else has this file open **** TBA */

		if (p)
			*q = p->o_link;
		else
			return (EINTRN);			/* some kind of internal error */
	}

	/* only flush to appropriate drive ***** TBA ***** */

	for (i = 0; i < 2; i++)
		for (b = bufl[i]; b; b = b->b_link)
			flush(b);

	return (E_OK);
}


/*
** [1]	We play games here (thanx, Jason).  The ixwrite() call will essentially
**	copy the time, date, cluster, and length fields from the OFD of the
**	(dir) file we are closeing to the FCB for this (dir) file in the 
**	parent dir.  The fileln field of this dir is thus set to 0.  But if 
**	this is a directory we are closing (path & CL_DIR), shouldn't the 
**	fileln be zero anyway?  I give up.
**					- ktb
*/



/*	
**  xunlink - unlink (delete) a file
**
**	Function 0x41 	f_delete
**
**  returns
**	EFILNF	file not found
**	EACCDN	access denied
**	ixdel()
**
*/

ERROR xunlink(P(const char *) name)
PP(const char *name;)								/*  path name of file to delete     */
{
	register DND *dn;
	register FCB *f;
	char *s;
	long pos;

	/* first find path */

	if ((long) (dn = findit(name, &s, 0)) < 0)
		return (dn);
	if (!dn)
		return E_FILNF;

	/* now scan for filename */

	pos = 0;
	if (!(f = scan(dn, s, FA_NORM, &pos)))
		return E_FILNF;

	if (f->f_attrib & FA_RDONLY)
		return E_ACCDN;

	pos -= 32;

	return (ixdel(dn, f, pos));
}


/*
**  ixdel - internal delete file.
**
**  Traverse the list of files open for this directory node.
**  If a file is found that has the same position in the directory as the one 
**  we are to delete, then scan the system file table to see if this process is 
**  then owner.  If so, then close it, otherwise abort.
**  
**  NOTE: 	both 'for' loops scan for the entire length of their 
**		respective data structures, and do not drop out of the loop on 
**		the first occurence of a match.
** 	Used by
** 		ixcreat()
** 		xunlink()
** 		xrmdir()
** 
*/

ERROR ixdel(P(DND *) dn, P(FCB *) f, P(long) pos)
PP(DND *dn;)
PP(FCB *f;)
PP(long pos;)
{
	register OFD *fd;
	DMD *dm;
	register n2;
	int n;
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
? Traverse this file's chain of allocated clusters, freeing them.
*/

	dm = dn->d_drv;
	n = f->f_clust;
	swp68(n);

	while (n && (n != -1))
	{
		n2 = getcl(n, dm);
		clfix(n, 0, dm);
		n = n2;
	}

/*
? Mark the directory entry as erased.
*/

	fd = dn->d_ofd;
	ixlseek(fd, pos);
	c = 0xe5;
	ixwrite(fd, 1L, &c);
	ixclose(fd, CL_DIR);

/*
**	NOTE	that the preceding routines that do physical disk operations 
**	will 'longjmp' on failure at the BIOS level, thereby allowing us to 
**	simply return with E_OK.
*/

	return E_OK;
}


/*
**  match1 - check for bad chars in path name
**	check thru test string to see if any character in the ref str is found
**	(utility routine for ixcreat())
**	by scc
*/

BOOLEAN match1(P(const char *) ref, P(const char *) test)
PP(register const char *ref;)
PP(const char *test;)
{
	register char *t;

	while (*ref)
	{
		for (t = test; *t; t++)
			if (*t == *ref)
				return (TRUE);
		ref++;
	}

	return (FALSE);
}


/*
**  used in calls to sftsrch to distinguish which field we are matching on
*/

#define	SFTOFD		0
#define	SFTOWNER	1

/*
**  SFTOFDSRCH - search sft for entry with matching OFD ptr
**	call sftsrch with correct parms
*/

#define	SFTOFDSRCH(o)	sftsrch( SFTOFD , (char *) o )

/*
**  SFTOWNSRCH - search sft for entry with matching PD
**	call sftsrch with correct parms
*/

#define	SFTOWNSRCH(p)	sftsrch( SFTOWN , (char *) p )


/*
**  sftosrch - search the sft for an entry with the specified OFD
**  returns:
**	ptr to the matching sft, or
**	NULL
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
	return (i >= OPNFILES ? (FTAB *) NULL : sftp);
}


/*
**  sftdel - delete an entry from the sft
**	delete the entry from the sft.  If no other entries in the sft
**	have the same ofd, free up the OFD, also.
*/

VOID sftdel(P(FTAB *) sftp)
PP(FTAB *sftp;)
{
	register FTAB *s;
	register OFD *ofd;

	/*  clear out the entry  */

	ofd = (s = sftp)->f_ofd;

	s->f_ofd = 0;
	s->f_own = 0;
	s->f_use = 0;

	/*  if no other sft entries with same OFD, delete ofd  */

	if (SFTOFDSRCH(ofd) == (FTAB *) NULL)
		xmfreblk(ofd);
}
