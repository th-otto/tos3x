/*  fsdir - directory routines for the file system			*/

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include <toserrno.h>
#include "btools.h"


/*
 *  local macros
 */

#define dirscan(a,c) ((DND *) scan(a,c,0x10,&negone))

/*
 *  local structures
 */

/*
 *  local forward declarations
 */

DND *GetDnd PROTO((const char *n, DND *d));

/*
 *  dots -, dots2  -
 */

static char dots[22] = { ".          " };
static char dots2[22] = { "..         " };

/*
 *  negone - for use as parameter
 */

static int32_t negone = -1L;



/*
 *  scan - scan a directory for an entry with the desired name.
 *	scans a directory indicated by a DND.  attributes figure in matching
 *	as well as the entry's name.  posp is an indicator as to where to start
 *	searching.  A posp of -1 means to use the scan pointer in the dnd, and
 *	return the pointer to the DND, not the FCB.
 */

/* 306: 00e15580 */
FCB *scan(P(DND *) dnd, P(const char *) n, P(int16_t) att, P(int32_t *) posp)
PP(register DND *dnd;)
PP(const char *n;)
PP(int att;)
PP(int32_t *posp;)
{
	char name[12];
	register FCB *fcb;
	OFD *fd;
	DND *dnd1;
	BOOLEAN m;								/*  T: found a matching FCB     */

	m = 0;								/*  have_match = false          */
	builds(n, name);					/*  format name into dir format     */
	name[11] = att;

	/*
	 *  if there is no open file descr for this directory, make one
	 */

	if (!(fd = dnd->d_ofd))
	{
		if (!(dnd->d_ofd = (fd = makofd(dnd))))
		{
			return (FCB *)0;
		}
	}

	/*
	 *  seek to desired starting position.  If posp == -1, then start at
	 * the beginning.
	 */

	ixlseek(fd, (*posp == -1) ? 0L : *posp);

	/*
	 *  scan thru the directory file, looking for a match
	 */

	while ((fcb = (FCB *) ixread(fd, 32L, NULL)) && (fcb->f_name[0]))
	{
		/*
		 *  Add New DND.
		 *  ( iff after scan ptr && complete flag not set && not a .
		 *  or .. && subdirectory && not deleted )
		 */

		if ((fcb->f_attrib & FA_DIREC) && (fcb->f_name[0] != '.') && (fcb->f_name[0] != 0xE5)
			)
		{								/*  see if we already have it  */
			dnd1 = GetDnd(&fcb->f_name[0], dnd);
			if (!dnd1)
				if (!(dnd1 = makdnd(dnd, fcb)))
					return NULL;
		}

		if ((m = match(name, fcb->f_name)) != 0)
			break;
	}

	/* restore directory scanning pointer */

	if (*posp != -1L)
		*posp = fd->o_bytnum;

	/*
	 *  if there was no match, but we were looking for a deleted entry,
	 *  then return a pointer to a deleted fcb.  Otherwise, if there was
	 *  no match, return a null pointer
	 */

	if (!m)
	{									/*  assumes that (*n != 0xe5) (if posp == -1)  */
		if (fcb && (*n == 0xe5))
			return fcb;
		return (FCB *)0;
	}

	if (*posp == -1)
	{									/*  seek to position of found entry  */
		ixlseek(fd, fd->o_bytnum - 32);
		return (FCB *)dnd1;
	}

	return fcb;
}


/*
 *  dcrack - parse out start of 1st path element, get DND
 *	if needed, logs in the drive specified (explicitly or implicitly) in
 *	the path spec pointed to by 'np', parses out the first path element
 *	in that path spec, and adjusts 'np' to point to the first char in that
 *	path element.
 *
 *  returns
 *	ptr to DND for 1st element in path, or
 *	error
 */

/* 306: 00e156b4 */
static DND *dcrack(P(const char **) np)
PP(const char **np;)
{
	register const char *n;
	DND *p;
	register int d;
	int32_t l;

	/*
	 *  get drive spec (or default) and make sure drive is logged in
	 */

	n = *np;							/*  get ptr to name     */
	if (n[1] == ':')					/*  if we start with drive spec */
	{
		d = uc(n[0]) - 'A';				/*    compute drive number  */
		n += 2;							/*    bump past drive number    */
	} else								/*  otherwise           */
		d = run->p_curdrv;				/*    assume default        */

	if ((l = ckdrv(d)) < 0)				/*  check for valid drive & log */
		return (DND *)l;				/*    in.  abort if error   */

	/*
	 *  if the pathspec begins with SLASH, then the first element is
	 *  the root.  Otherwise, it is the current default directory.  Get
	 *  the proper DND for this element
	 */

	if (*n == SLASH)
	{									/* [D:]\path */
		p = drvtbl[d]->m_dtl;			/*  get root dir for log drive  */
		n++;							/*  skip over slash     */
	} else
		p = dirtbl[run->p_curdir[d]];	/*  else use curr dir   */

	/* whew ! *//*  <= thankyou, Jason, for that wonderful comment */

	*np = n;
	return p;
}


/*
 *  findit - find a file/dir entry
 */

/* 306: 00e15750 */
DND *findit(P(const char *) name, P(const char **) sp, P(int) dflag)
PP(const char *name;)								/*  name of file/dir            */
PP(const char **sp;)
PP(int dflag;)								/*  T: name is for a directory      */
{
	register DND *p;
	const char *n;
	DND *pp, *newp;
	int i;
	char s[11];

	/* crack directory and drive */

	n = name;

	if ((long) (p = dcrack(&n)) <= 0)
		return p;

	/*
	 *  Force scan() to read from the beginning of the directory again,
	 *  since we have gone to a scheme of keeping fewer DNDs in memory.
	 */

	do
	{
		if (!(i = getpath(n, s, dflag)))
			break;

		if (i < 0)
		{								/*  path is '.' or '..'  */
			if (i == -2)				/*  go to parent (..)  */
				p = p->d_parent;

			i = -i;						/*  num chars is 1 or 2  */
			goto scanxt;
		}

		/*
		 * go down a level in the path...
		 * save a pointer to the current DND, which will
		 * become the parent, and get the node on the left,
		 * which is the first child.
		 */

		pp = p;							/*  save ptr to parent dnd  */

		if (!(newp = p->d_left))
		{								/*  [1]         */
			/*  make sure children  */
			newp = dirscan(p, n);		/*  are logged in   */
		}

		if (!(p = newp))				/*  If no children, exit loop */
			break;

		/*
		 *  check all subdirectories at this level.  if we run out
		 * of siblings in the DND list (p->d_right == NULL), then
		 * we should rescan the whole directory and make sure they
		 * are all logged in.
		 */

		while (p && (xcmps(s, p->d_name) == FALSE))
		{
			newp = p->d_right;			/*  next sibling    */

			if (newp == NULL)			/* if no more siblings  */
			{
				p = 0;
				if (pp)
				{
					p = dirscan(pp, n);
				}
			} else
				p = newp;
		}

	  scanxt:if (*(n = n + i))
			n++;
		else
			break;
	} while (p && i);

	/* p = 0 ==> not found
	   i = 0 ==> found at p (dnd entry)
	   n = points at filename */

	*sp = n;

	return p;
}

/*
 * [1]	The first call to dirscan is if there are no children logged in.
 *	However, we need to call dirscan if children are logged in and we still
 *	didn't find the desired node, as the desired child may've been flushed.
 *	This is a terrible thing to have happen to a child.  However, we can't
 *	afford to have all these kids around here, so when new ones come in, we
 *	see which we can flush out (see makdnd()).  This is a hack -- no doubt
 *	about that; the cached DND scheme needs to be redesigned all around.
 *	Anyway, the second call to dirscan backs up to the parent (note that n
 *	has not yet been bumped, so is still pointing to the current subdir's
 *	name -- in effect, starting us at this level all over again.
 *			-- ktb
 */


/*
 *  xchdir - change current dir to path p (extended cd n:=[a:][\bin])
 *
 *	Function 0x3B	Dsetpath
 *
 *	Error returns
 *		EPTHNF
 *		ckdrv()
 */

/* 306: 00e15876 */
ERROR xchdir(P(const char *) p)
PP(const char *p;)
{
	register long l;
	register int i;
	register int dlog;
	register int dr;
	const char *s;

	if (p[1] == ':')
	{
		dlog = uc(p[0]) - 'A';
		p += 2;
	} else
	{
		dlog = run->p_curdrv;
	}

	/* BUG: if drive was specified in path this will call findit() on the current drive */
	if (!(l = (long) findit(p, &s, 1)))
		return E_PTHNF;

	/* find space in dirtbl */
	dr = run->p_curdir[dlog];

	for (i = 1; i < NCURDIR; i++)
	{
		if (dirtbl[i] == (DND *)l)
		{
			goto found;
		}
	}
	for (i = 1; i < NCURDIR; i++)
	{
		if (!diruse[i])
		{
			dirtbl[i] = (DND *)l;
			goto found;
		}
	}
	return E_PTHNF;

found:

	if (dr != 0 && diruse[dr])
		--diruse[dr];

	diruse[i]++;

	run->p_curdir[dlog] = i;

	return E_OK;
}


/*
 *  xgetdir -
 *
 *	Function 0x47	Dgetpath
 *
 *	Error returns
 *		EDRIVE
 */

/* 306: 00e15972 */
ERROR xgetdir(P(char *) buf, P(int16_t) drv)					/*+ return text of current dir into specified buffer */
PP(char *buf;)
PP(register int16_t drv;)
{
	DND *p;
	int len;

	drv = (drv == 0) ? run->p_curdrv : drv - 1;

	if (!(Drvmap() & (1 << drv)) || (ckdrv(drv) < 0))
	{
		*buf = 0;
		return E_DRIVE;
	}

	p = dirtbl[run->p_curdir[drv]];
	len = 64;
	buf = dopath(p, buf, &len);
	*--buf = 0;							/* null as last char, not slash */

	return E_OK;
}


/*
 *  xmkdir - make a directory, given path 's'
 *
 *	Function 0x39	Dcreate
 *
 */

/* 306: 00e15afc */
ERROR xmkdir(P(const char *) s)
PP(const char *s;)
{
	OFD *f;
	FCB *f2;
	OFD *fd, *f0;
	FCB *b;
	DND *dn;
	int h;
	CLNO cl;
	int plen;
	ERROR rc;

	if ((h = rc = ixcreat(s, FA_DIREC)) < 0)
		return rc;

	f = getofd(h);

	/* build a DND in the tree */

	fd = f->o_dirfil;

	ixlseek(fd, f->o_dirbyt);
	b = (FCB *) ixread(fd, 32L, NULL);

	/* is the total path length >= 64 chars? */

	plen = namlen(b->f_name);
	for (dn = f->o_dnode; dn; dn = dn->d_parent)
		plen += namlen(dn->d_name);
	if (plen >= 64)
	{
		ixdel(f->o_dnode, b, f->o_dirbyt);
		return E_ACCDN;
	}

	if ((dn = makdnd(f->o_dnode, b)) == NULL)
	{
		ixdel(f->o_dnode, b, f->o_dirbyt);
		return E_NSMEM;
	}

	if ((dn->d_ofd = f0 = makofd(dn)) == NULL)
	{
		ixdel(f->o_dnode, b, f->o_dirbyt);
		f->o_dnode->d_left = NULL;
		xmfreblk(dn);
		return E_NSMEM;
	}

	/* initialize dir cluster */

	if (nextcl(f0, 1))
	{
		ixdel(f->o_dnode, b, f->o_dirbyt);
		f->o_dnode->d_left = NULL;
		freednd(dn);
		return E_ACCDN;
	}

	f2 = dirinit(dn);					/* pointer to dirty dir block */

	/* write identifier */

	xmovs(22, dots, (char *) f2);
	f2->f_attrib = FA_DIREC;
	f2->f_td.time = time;
	swp68(&f2->f_td.time);
	f2->f_td.date = date;
	swp68(&f2->f_td.date);
	cl = f0->o_strtcl;
	swp68(&cl);
	f2->f_clust = cl;
	f2->f_fileln = 0;
	f2++;

	/* write parent entry .. */

	xmovs(22, dots2, (char *) f2);
	f2->f_attrib = FA_DIREC;
	f2->f_td.time = time;
	swp68(&f2->f_td.time);
	f2->f_td.date = date;
	swp68(&f2->f_td.date);
	cl = f->o_dirfil->o_strtcl;

	if (cl < 0)
		cl = 0;

	swp68(&cl);
	f2->f_clust = cl;
	f2->f_fileln = 0;
	xmovs(sizeof(OFD), (char *) f0, (char *) f);
	f->o_flag |= O_DIRTY;
	ixclose(f, CL_DIR | CL_FULL);		/* force flush and write */
	xmfreblk(f);
	sft[h - NUMSTD].f_own = 0;
	sft[h - NUMSTD].f_ofd = 0;
	return E_OK;
}


/*
 *  xrmdir - remove (delete) a directory
 *
 *	Function 0x3A	Ddelete
 *
 *	Error returns
 *		EPTHNF
 *		EACCDN
 *		EINTRN
 *
 */

/* 306: 00e15d08 */
ERROR xrmdir(P(const char *)p)
PP(const char *p;)
{
	register DND *d;
	DND *d1, **q;
	FCB *f;
	OFD *fd, *f2;
	long pos;
	const char *s;
	register int i;

	if ((ERROR) (d = findit(p, &s, 1)) < 0)
		return (ERROR)d;
	if (!d)
		return E_PTHNF;

	if (!d->d_parent)					/*  Can't delete root  */
		return E_ACCDN;

	for (i = 1; i <= NCURDIR; i++)		/*  Can't delete in use  */
		if (diruse[i])
			if (dirtbl[i] == d)
				return E_ACCDN;

	if (!(fd = d->d_ofd))
		if (!(fd = makofd(d)))
			return E_NSMEM;

	ixlseek(fd, 0x40L);
	do
	{
		if (!(f = (FCB *) ixread(fd, 32L, NULL)))
			break;
	} while (f->f_name[0] == 0xe5);


	if (f != NULL && f->f_name[0] != 0)
		return E_ACCDN;

	for (d1 = *(q = &d->d_parent->d_left); d1 != d; d1 = *(q = &d1->d_right))
		;								/* follow sib-links */

	if (d1 != d)
		return E_INTRN;				/* internal error */

	if (d->d_files)
		return E_INTRN;				/* open files ? - internal error */

	if (d->d_left)
		return E_INTRN;				/* subdir - internal error */


	/* take him out ! */

	*q = d->d_right;

	if (d->d_ofd)
	{
		xmfreblk(d->d_ofd);
	}

	d1 = d->d_parent;
	xmfreblk(d);
	ixlseek((f2 = fd->o_dirfil), (pos = fd->o_dirbyt));
	f = (FCB *) ixread(f2, 32L, NULL);

	return ixdel(d1, f, pos);
}


/*
 *  ixsfirst - search for first dir entry that matches pattern
 *	search first for matching name, into specified address.  if
 *	address = 0L, caller wants search only, no buffer info
 *  returns:
 *	error code.
 */

/* 306: 00e15e3a */
ERROR ixsfirst(P(const char *) name, P(int16_t) att, P(DTAINFO *)addr)
PP(const char *name;)								/*  name of file to match   */
PP(register int16_t att;)							/*  attribute of file       */
PP(register DTAINFO *addr;)						/*  ptr to dta info         */
{
	const char *s;
	DND *dn;
	FCB *f;
	int32_t pos;

	if (att != 8)
		att |= 0x21;

	if ((ERROR) (dn = findit(name, &s, 0)) < 0)
		return (ERROR)dn;
	if (dn == NULL)
		return E_FILNF;

	/* now scan for filename from start of directory */

	pos = 0;

	if ((f = scan(dn, s, att, &pos)) == NULL)
		return E_FILNF;

	if (addr)
	{
		bmove(s, (char *) & addr->dt_name[0], 12);
		addr->dt_attr = att;
		addr->dt_pos = pos;
		addr->dt_dnd = dn;
		makbuf(f, addr);
	}

	return E_OK;
}



/*
 *  xsnext -
 *	search next, return into dta
 *
 *	Function 0x4F	Fsnext
 *
 *	Error returns
 *		ENMFIL
 */

/* 306: 00e15f20 */
ERROR xsnext(NOTHING)
{
	register FCB *f;
	register DTAINFO *dt;

	dt = (DTAINFO *) run->p_xdta;

	/* has the DTA been initialized? */
	if (dt->dt_dnd == NULL)
		return E_NMFIL;

	f = scan(dt->dt_dnd, &dt->dt_name[0], dt->dt_attr, &dt->dt_pos);

	if (f == NULL)
		return E_NMFIL;

	makbuf(f, (DTAINFO *) run->p_xdta);
	return E_OK;
}


/*
 *  ixclose -
 *
 *	Error returns
 *		EINTRN
 *
 *	Last modified	SCC	10 Apr 85
 *
 *	NOTE:	I'm not sure that returning immediatly upon an error from 
 *		ixlseek() is the right thing to do.  Some data structures may 
 *		not be updated correctly.  Watch out for this!
 *		Also, I'm not sure that the EINTRN return is ok.
 */

/* 306: 00e160ba */
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

		swp68(&fd->o_strtcl);
		swp68l(&fd->o_fileln);

		if (part & CL_DIR)
		{
			tmp = fd->o_fileln;			/* [1] */
			fd->o_fileln = 0;
			ixwrite(fd->o_dirfil, 10L, &fd->o_td.time);
			fd->o_fileln = tmp;
		} else
		{
			ixwrite(fd->o_dirfil, 10L, &fd->o_td.time);
		}
		
		swp68(&fd->o_strtcl);
		swp68l(&fd->o_fileln);
	}

	if ((!part) || (part & CL_FULL))
	{
		q = &fd->o_dnode->d_files;

		for (p = *q; p; p = *(q = &p->o_link))
			if (p == fd)
				break;

		/* someone else has this file open */

		if (p)
			*q = p->o_link;
		else
			return E_INTRN;			/* some kind of internal error */
	}

	/* only flush to appropriate drive */

	for (i = 0; i < 2; i++)
		for (b = bufl[i]; b; b = b->b_link)
			flush(b);

	return E_OK;
}


/*
 *  dirinit -
 */

FCB *dirinit(P(DND *)dn)
PP(DND *dn;)								/*  dir descr for dir           */
{
	OFD *fd;							/*  ofd for this dir            */
	int num, i2;
	char *s1;
	DMD *dm;
	FCB *f1;

	fd = dn->d_ofd;						/*  OFD for dir */
	num = (dm = fd->o_dmd)->m_recsiz;	/*  bytes/rec   */

	/*
	 *  for each record in the current cluster, besides the first record,
	 * get the record and zero it out
	 */

	for (i2 = 1; i2 < dm->m_clsiz; i2++)
	{
		s1 = getrec(fd->o_currec + i2, dn->d_drv, 1);
		bzero(s1, num);
	}

	/*
	 *  now zero out the first record and return a pointer to it
	 */

	f1 = (FCB *) (s1 = getrec(fd->o_currec, dn->d_drv, 1));

	bzero(s1, num);
	return f1;
}


#ifdef	NEWCODE
#define	isnotdelim(x)	((x) && (x!='*') && (x!=SLASH) && (x!='.') && (x!=' '))

#define	MAXFNCHARS	8


/*
 *  builds - build a directory style file spec from a portion of a path name
 *	the string at 's1' is expected to be a path spec in the form of
 *	(xxx/yyy/zzz).  *builds* will take the string and crack it
 *	into the form 'ffffffffeee' where 'ffffffff' is a non-terminated
 *	string of characters, padded on the right, specifying the filename
 *	portion of the file spec.  (The file spec terminates with the first
 *	occurrence of a SLASH or EOS, the filename portion of the file spec
 *	terminates with SLASH, EOS, PERIOD or WILDCARD-CHAR).  'eee' is the
 *	file extension portion of the file spec, and is terminated with
 *	any of the above.  The file extension portion is left justified into
 *	the last three characters of the destination (11 char) buffer, but is
 *	padded on the right.  The padding character depends on whether or not
 *	the filename or file extension was terminated with a separator
 *	(EOS, SLASH, PERIOD) or a WILDCARD-CHAR.
 *
 */

VOID builds(P(const char *) s1, P(char *) s2)
PP(register char *s1;)							/*  source          */
PP(register char *s2;)									/*  s2 dest         */
{
	register int i;
	char c;

	/*
	 * copy filename part of pathname to destination buffer until a
	 * delimiter is found
	 */

	for (i = 0; (i < MAXFNCHARS) && isnotdelim(*s1); i++)
		*s2++ = uc(*s1++);

	/*
	 *  if we have reached the max number of characters for the filename
	 * part, skip the rest until we reach a delimiter
	 */

	if (i == MAXFNCHARS)
		while (*s1 && (*s1 != '.') && (*s1 != SLASH))
			s1++;

	/*
	 *  if the current character is a wildcard character, set the padding
	 * char with a "?" (wildcard), otherwise replace it with a space
	 */

	c = (*s1 == '*') ? '?' : ' ';


	if (*s1 == '*')						/*  skip over wildcard char */
		s1++;

	if (*s1 == '.')						/*  skip over extension delim   */
		s1++;

	/*
	 *  now that we've parsed out the filename part, pad out the
	 * destination with "?" wildcard chars
	 */

	for (; i < MAXFNCHARS; i++)
		*s2++ = c;

	/*
	 *  copy extension part of file spec up to max number of characters
	 * or until we find a delimiter
	 */

	for (i = 0; i < 3 && isnotdelim(*s1); i++)
		*s2++ = uc(*s1++);

	/*
	 *  if the current character is a wildcard character, set the padding
	 * char with a "?" (wildcard), otherwise replace it with a space
	 */

	c = ((*s1 == '*') ? '?' : ' ');

	/*
	 *  pad out the file extension
	 */

	for (; i < 3; i++)
		*s2++ = c;
}

#else

/*
 *  builds -
 *
 *	Last modified	LTG	23 Jul 85
 */

VOID builds(P(const char *) s1, P(char *) s2)
PP(register char *s1;)							/*  source          */
PP(register char *s2;)									/*  s2 dest         */
{
	int i;
	char c;

	for (i = 0; (i < 8) && (*s1) && (*s1 != '*') && (*s1 != SLASH) && (*s1 != '.') && (*s1 != ' '); i++)
		*s2++ = uc(*s1++);

	if (i == 8)
		while (*s1 && (*s1 != '.') && (*s1 != SLASH))
			s1++;

	c = ((*s1 == '*') ? '?' : ' ');

	if (*s1 == '*')
		s1++;

	if (*s1 == '.')
		s1++;

	for (; i < 8; i++)
		*s2++ = c;

	for (i = 0; (i < 3) && (*s1) && (*s1 != '*') && (*s1 != SLASH) && (*s1 != '.') && (*s1 != ' '); i++)
		*s2++ = uc(*s1++);

	c = ((*s1 == '*') ? '?' : ' ');

	for (; i < 3; i++)
		*s2++ = c;
}

#endif



/*
 *  makdnd - make a child subdirectory of directory p
 */

DND *makdnd(P(DND *) p, P(FCB *) b)
PP(DND *p;)
PP(FCB *b;)
{
	register DND *p1;
	register DND **prev;
	OFD *fd;
	register int i;
	int in_use;
	fd = p->d_ofd;

	/*
	 *  scavenge a DND at this level if we can find one that has not
	 *  d_left
	 */

	for (prev = &p->d_left; (p1 = *prev) != NULL; prev = &p1->d_right)
	{
		if (!p1->d_left)
		{
			/* check dirtbl[] to see if anyone is using this guy */

			in_use = 0;
			for (i = 1; i < NCURDIR; i++)
				if (diruse[i])
					if (dirtbl[i] == p1)
						in_use = 1;

			if (!in_use && p1->d_files == NULL)
			{
				/* clean out this DND for reuse */

				p1->d_flag = 0;
				p1->d_scan = 0L;
				p1->d_files = (OFD *) 0;
				if (p1->d_ofd)
				{
					xmfreblk(p1->d_ofd);
				}
				break;
			}
		}
	}

	/* we didn't find one that qualifies, so allocate a new one */

	if (!p1)
	{
		if (!(p1 = MGET(DND)))
			return (DND *) 0;			/* ran out of system memory */

		/* do this init only on a newly allocated DND */
		p1->d_right = p->d_left;
		p->d_left = p1;
		p1->d_parent = p;
	}

	/* complete the initialization */

	p1->d_ofd = (OFD *) 0;
	p1->d_strtcl = b->f_clust;
	swp68(&p1->d_strtcl);
	p1->d_drv = p->d_drv;
	p1->d_dirfil = fd;
	p1->d_dirpos = fd->o_bytnum - 32;
	p1->d_td.time = b->f_td.time;
	p1->d_td.date = b->f_td.date;
	xmovs(11, (char *) b->f_name, (char *) p1->d_name);

	return p1;
}



/*
 *  getpath - get a path element
 *	The buffer pointed to by 'd' must be at least the size of the file
 *	spec buffer in a directory entry (including file type), and will
 *	be filled with the directory style format of the path element if
 *	no error has occurred.
 *
 *  returns
 *	-1 if '.'
 *	-2 if '..'
 *	 0 if p => name of a file (no trailing SLASH or !dirspec)
 *	>0 (nbr of chars in path element (up to SLASH)) && buffer 'd' filled.
 *
 */

int getpath(P(const char *) p, P(char *) d, P(int) dirspec)
PP(const char *p;)								/*  start of path element to crack  */
PP(char *d;)									/*  ptr to destination buffer       */
PP(int dirspec;)							/*  true = no file name, just dir path  */
{
	register int i, i2;
	register const char *p1;

	for (i = 0, p1 = p; *p1 && (*p1 != SLASH); p1++, i++)
		;

	/*
	 *  If the string we have just scanned over is a directory name, it
	 * will either be terminated by a SLASH, or 'dirspec' will be set
	 * indicating that we are dealing with a directory path only
	 * (no file name at the end).
	 */

	if (*p1 != '\0' || dirspec)
	{									/*  directory name  */
		i2 = 0;
		if (p[0] == '.')				/*  dots in name    */
		{
			i2--;						/*  -1 for dot      */
			if (p[1] == '.')
				i2--;					/*  -2 for dotdot   */
			return i2;
		}

		if (i)							/*  if not null path el */
			builds(p, d);				/*  d => dir style fn   */

		return i;						/*  return nbr chars    */
	}

	return 0;							/*  if string is a file name        */
}



/*
 *  match - utility routine to compare file names
 */

BOOLEAN match(P(const char *) s1, P(const char *) s2)
PP(register const char *s1;)							/*  name we are checking        */
PP(register const char *s2;)									/*  name in fcb             */
{
	register int i;

	/*
	 *  check for deleted entry.  wild cards don't match deleted entries,
	 *  only specific requests for deleted entries do.
	 */

	if (*s2 == 0xe5)
	{
		if (*s1 == '?')
			return FALSE;
		else if (*s1 == 0xe5)
			return TRUE;
	}

	/*
	 *  compare names
	 */

	for (i = 0; i < 11; i++, s1++, s2++)
		if (*s1 != '?')
			if (uc(*s1) != uc(*s2))
				return FALSE;

	/*
	 *  check attribute match
	 * volume labels and subdirs must be specifically asked for
	 */

	if ((*s1 != FA_LABEL) && (*s1 != FA_DIREC))
		if (!(*s2))
			return TRUE;

	return *s1 & *s2 ? TRUE : FALSE;
}



/*
 *  makbuf - copy info from FCB into DTA info area
 */

VOID makbuf(P(FCB *) f, P(DTAINFO *) dt)
PP(register FCB *f;)
PP(register DTAINFO *dt;)
{
	dt->dt_fattr = f->f_attrib;
	dt->dt_td.time = f->f_td.time;
	swp68(&dt->dt_td.time);
	dt->dt_td.date = f->f_td.date;
	swp68(&dt->dt_td.date);
	dt->dt_fileln = f->f_fileln;
	swp68l(&dt->dt_fileln);

	if (f->f_attrib & FA_LABEL)
	{
		bmove((char *) & f->f_name[0], (char *) & dt->dt_fname[0], 11);
		dt->dt_fname[11] = '\0';
	} else
	{
		packit(&f->f_name[0], &dt->dt_fname[0]);
	}
}



/*
 *  xcmps - utility routine to compare two 11-character strings
 *
 *	Last modified	19 Jul 85	SCC
 */

int xcmps(P(const char *) s, P(const char *) d)
PP(register const char *s;)
PP(register const char *d;)
{
	register int i;

	for (i = 0; i < 11; i++)
		if (uc(*s++) != uc(*d++))
			return 0;
	return 1;
}


/*
 *  GetDnd - find a dnd with matching name
 */

DND *GetDnd(P(const char *) n, P(DND *) d)
PP(const char *n;)								/*  name of file in FCB format      */
PP(DND *d;)									/*  root where we start the search  */
{
	register DND *dnd;

	for (dnd = d->d_left; dnd; dnd = dnd->d_right)
	{
		if (xcmps(n, &dnd->d_name[0]))
			return dnd;
	}
	return NULL;

}


/*
 *  freednd - free an allocated and linked-in DND
 *
 */

VOID freednd(P(DND *) dn)
PP(DND *dn;)
{
	DND **prev;

	if (dn->d_ofd)						/* free associated OFD if it's linked */
		xmfreblk(dn->d_ofd);

	for (prev = &(dn->d_parent->d_left); *prev != dn; prev = &((*prev)->d_right))
		;								/* find the predecessor to this DND */
	*prev = dn->d_right;				/* then cut this DND out of the list */

	while (dn->d_left)					/* is this step really necessary? */
		freednd(dn->d_left);

	xmfreblk(dn);				/* finally free this DND */
}


/*
 *	namlen -
 *		parameter points to a character string of 11 bytes max
 *
 */

int namlen(P(const char *) s11)
PP(const char *s11;)
{
	int i, len;

	for (i = len = 1; i <= 11; i++, s11++)
		if (*s11 && (*s11 != ' '))
		{
			len++;
			if (i == 9)
				len++;
		}
	return len;
}



/*
 *  dopath -
 *
 */

/* 306: 00e1761a */
char *dopath(P(DND *) p, P(char *) buf, P(int *) len)
PP(DND *p;)
PP(char *buf;)
PP(int *len;)
{
	char temp[14];
	char *tp;
	long tlen;

	if (p->d_parent)
		buf = dopath(p->d_parent, buf, len);

	tlen = (long) packit(p->d_name, temp) - (long) temp;
	tp = temp;
	while (*len)
	{
		(*len)--;						/* len must never go < 0 */
		if (tlen--)
			*buf++ = *tp++;
		else
		{
			*buf++ = SLASH;
			break;
		}
	}
	return buf;
}
