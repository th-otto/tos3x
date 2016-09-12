/*  fsdrive.c - physical drive routines for file system			*/

#include "tos.h"
#include "fs.h"
#include "bios.h"
#include <toserrno.h>

/*
 *  globals
 */

/*
 *  dirtbl - default directories.
 * Each entry points to the DND for someone's default directory.
 * They are linked to each process by the p_curdir entry in the PD.
 * The first entry (dirtbl[0]) is not used, as p_curdir[i]==0
 * means 'none set yet'.
 */

DND *dirtbl[NCURDIR];

/*
 *  diruse - use count 
 *  drvsel - mask of drives selected since power up
 */

char diruse[NCURDIR];

int16_t drvsel;


/*
 *  getdmd - allocate storage for and initialize a DMD
 */

DMD *getdmd(P(int) drv)
PP(int drv;)
{
	DMD *dm;

	if (!(drvtbl[drv] = dm = MGET(DMD)))
		return NULL;

	if (!(dm->m_dtl = MGET(DND)))
		goto fredm;

	if (!(dm->m_dtl->d_ofd = MGET(OFD)))
		goto fredtl;

	if (!(dm->m_fatofd = MGET(OFD)))
		goto freofd;

	return dm;

freofd:
	xmfreblk(dm->m_dtl->d_ofd);
fredtl:
	xmfreblk(dm->m_dtl);
fredm:
	xmfreblk(dm);

	return NULL;
}



/*
 *  login -
 *	log in media 'b' on drive 'drv'.
 *
 */

ERROR login(P(BPB *) b, P(int) drv)
PP(BPB *b;)									/*  bios parm block for drive       */
PP(int drv;)								/*  drive number            */
{
	OFD *fo, *f;
	DND *d;
	DMD *dm;
	int rsiz, cs, n, fs, ncl, fcl;

	rsiz = b->recsiz;
	cs = b->clsiz;
	n = b->rdlen;
	fs = b->fsiz;

	if (!(dm = getdmd(drv)))
		return E_NSMEM;

	d = dm->m_dtl;						/*  root DND for drive      */
	dm->m_fsiz = fs;					/*  fat size            */
	f = d->d_ofd;						/*  root dir file       */
	dm->m_drvnum = drv;					/*  drv nbr into media descr    */
	f->o_dmd = dm;						/*  link to OFD for rt dir file */

	d->d_drv = dm;						/*  link root DND with DMD  */
	d->d_name[0] = 0;					/*  null out name of root   */

	dm->m_16 = b->b_flags & B_16;		/*  set 12 or 16 bit fat flag   */
	dm->m_clsiz = cs;					/*  set cluster size in sectors */
	dm->m_clsizb = b->clsizb;			/*    and in bytes      */
	dm->m_recsiz = rsiz;				/*  set record (sector) size    */
	dm->m_numcl = b->numcl;				/*  set cluster size in records */
	dm->m_clrlog = xlog2(cs);			/*    and log of it     */
	dm->m_clrm = logmsk[dm->m_clrlog];	/*  and mask of it  */
	dm->m_rblog = xlog2(rsiz);			/*  set log of bytes/record */
	dm->m_rbm = logmsk[dm->m_rblog];	/*  and mask of it  */
	dm->m_clblog = xlog2(dm->m_clsizb);	/*  log of bytes/clus   */

	f->o_fileln = n * rsiz;				/*  size of file (root dir) */


	ncl = (n + cs - 1) / cs;			/* number of "clusters" in root */
	d->d_strtcl = f->o_strtcl = -1 - ncl;	/*  root start clus */
	fcl = (fs + cs - 1) / cs;

	fo = dm->m_fatofd;					/*  OFD for 'fat file'      */
	fo->o_strtcl = d->d_strtcl - fcl;	/*  start clus for fat  */
	fo->o_dmd = dm;						/*  link with DMD       */

	dm->m_recoff[0] = b->fatrec - (fo->o_strtcl * cs);
	dm->m_recoff[1] = (b->fatrec + fs) - (d->d_strtcl * cs);

	/* 2 is first data cluster */

	dm->m_recoff[2] = b->datrec - (2 * cs);

	fo->o_bytnum = 3;
	fo->o_curbyt = 3;
	fo->o_fileln = fs * rsiz;

	return E_OK;
}



/*
 *  xlog2 -
 *	return log base 2 of n
 */

int xlog2(P(int) n)
PP(int n;)
{
	int i;

	for (i = 0; n; i++)
		n >>= 1;

	return i - 1;
}
