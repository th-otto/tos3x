/*  fsfat.c - fat mgmt routines for file system				*/

#include	"tos.h"
#include	"fs.h"
#include	"bios.h"
#include	<toserrno.h>


VOID clfix PROTO((CLNO cl, CLNO link, DMD *dm));


/*
**  cl2rec -
*/

RECNO cl2rec(P(CLNO) cl, P(DMD *)dm)
PP(CLNO cl;)
PP(DMD *dm;)
{
	return cl * dm->m_clsiz;
}




/*
**  clfix -
**	replace the contents of the fat entry indexed by 'cl' with the value
**	'link', which is the index of the next cluster in the chain.
*/

VOID clfix(P(CLNO) cl, P(CLNO) link, P(DMD *) dm)
PP(CLNO cl;)
PP(CLNO link;)
PP(DMD *dm;)
{
	CLNO f[1];
	CLNO mask;
	long pos;

	if (dm->m_16)
	{
		swp68(link);
		pos = (long) (cl) << 1;
		ixlseek(dm->m_fatofd, pos);
		ixwrite(dm->m_fatofd, 2L, &link);
		return;
	}

	pos = (cl + (cl >> 1));

	link = link & 0x0fff;

	if (cl & 1)
	{
		link = link << 4;
		mask = 0x000f;
	} else
		mask = 0xf000;

	ixlseek(dm->m_fatofd, pos);

	/* pre -read */
	ixread(dm->m_fatofd, 2L, f);

	swp68(f[0]);
	f[0] = (f[0] & mask) | link;
	swp68(f[0]);

	ixlseek(dm->m_fatofd, pos);
	ixwrite(dm->m_fatofd, 2L, f);
}




/*
**  getcl -
**	get the contents of the fat entry indexed by 'cl'.
**
**  returns
**	0xffff if entry contains the end of file marker
**	otherwise, the contents of the entry (16 bit value always returned).
*/

CLNO getcl(P(int) cl, P(DMD *) dm)
PP(int cl;)
PP(DMD *dm;)
{
	CLNO f[1];

	if (cl < 0)
		return (cl + 1);

	if (dm->m_16)
	{
		ixlseek(dm->m_fatofd, (long) ((long) (cl) << 1));
		ixread(dm->m_fatofd, 2L, f);
		swp68(f[0]);
		return (f[0]);
	}

	ixlseek(dm->m_fatofd, ((long) (cl + (cl >> 1))));
	ixread(dm->m_fatofd, 2L, f);
	swp68(f[0]);

	if (cl & 1)
		cl = f[0] >> 4;
	else
		cl = 0x0fff & f[0];

	if (cl == 0x0fff)
		return (-1);

	return (cl);
}




/*
**  nextcl -
**	get the cluster number which follows the cluster indicated in the curcl
**	field of the OFD, and place it in the OFD.
**
**  returns
**	E_OK	if success,
**	-1	if error
**
*/

int nextcl(P(OFD *) p, P(int) wrtflg)
PP(OFD *p;)
PP(int wrtflg;)
{
	DMD *dm;
	CLNO i;
	CLNO rover;
	CLNO cl, cl2;

	cl = p->o_curcl;
	dm = p->o_dmd;

	if ((int) (cl) < 0)
	{
		cl2 = cl + 1;
		goto retcl;
	}

	if ((int) (cl) > 0)
		cl2 = getcl(cl, dm);

	if (cl == 0)
		cl2 = (p->o_strtcl ? p->o_strtcl : 0xffff);

	if (wrtflg && (cl2 == 0xffff))
	{									/* end of file, allocate new clusters */
		rover = cl;
		for (i = 2; i < dm->m_numcl; i++)
		{
			if (rover < 2)
				rover = 2;

			if (!(cl2 = getcl(rover, dm)))
				break;
			else
				rover = (rover + 1) % dm->m_numcl;
		}

		cl2 = rover;

		if (i < dm->m_numcl)
		{
			clfix(cl2, 0xffff, dm);
			if (cl)
				clfix(cl, cl2, dm);
			else
			{
				p->o_strtcl = cl2;
				p->o_flag |= O_DIRTY;
			}
		} else
			return (0xffff);
	}

	if (cl2 == 0xffff)
		return (0xffff);

  retcl:p->o_curcl = cl2;
	p->o_currec = cl2rec(cl2, dm);
	p->o_curbyt = 0;

	return (E_OK);
}





/*	Function 0x36	d_free

	Error returns
		ERR

	Last modified	SCC	15 May 85
*/

ERROR xgetfree(P(int32_t *) buf, P(int16_t) drv)					/*+ get disk free space data into buffer */
PP(int32_t *buf;)
PP(int16_t drv;)
{
	int i, free;
	DMD *dm;

	drv = (drv ? drv - 1 : run->p_curdrv);

	if ((i = ckdrv(drv)) < 0)
		return (ERR);

	dm = drvtbl[i];
	free = 0;
	for (i = 2; i < dm->m_numcl; i++)
		if (!getcl(i, dm))
			free++;
	*buf++ = (long) (free);
	*buf++ = (long) (dm->m_numcl);
	*buf++ = (long) (dm->m_recsiz);
	*buf++ = (long) (dm->m_clsiz);
	return (E_OK);
}
