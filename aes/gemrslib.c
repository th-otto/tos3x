/*
*************************************************************************
*			Revision Control System
* =======================================================================
*  $Revision: 2.3 $	$Source: /u2/MRS/osrevisions/aes/gemrslib.c,v $
* =======================================================================
*  $Author: mui $	$Date: 89/04/26 18:26:37 $	$Locker: kbad $
* =======================================================================
*  $Log:	gemrslib.c,v $
* Revision 2.3  89/04/26  18:26:37  mui
* TT
* 
* Revision 2.2  89/04/01  03:12:36  kbad
* Change to rs_readit to allow sh_find to look for files in the directory
* from which a program was launched.  rs_readit now uses a local buffer
* to store the filename instead of ad_shcmd, therefore ad_shcmd is intact
* ; contains the full pathname of the last shel_written program; and can
* be used by shel_find.
* 
* Revision 2.1  89/02/22  05:29:18  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  88/06/02  12:35:03  lozben
* Initial revision
* 
*************************************************************************
*/
/*  	GEMRSLIB.C	5/14/84 - 04/09/85	Lowell Webster		*/
/*	Reg Opt		03/08/85 - 03/09/85	Derek Mui		*/
/*	Fix the get_addr for imagedata	4/16/86	Derek Mui		*/
/*	Unfix the get_addr		10/24/86	Derek Mui	*/
/*	Replace LSTRLEN as strlen	3/15/88		D.Mui		*/
/*	Fix rs_load to include new resource type	7/10/92	D.Mui	*/
/*	Fix rs_readit, if extended format is invalid, ignore it		*/
/*	Fix at dos_lseek and if not extended format make sure the word 	*/
/*	is zero				7/13/92		D.Mui		*/
/*	Check the RT_VRSN make sure it is greater or equal to RS_SIZE	*/

/*
*	-------------------------------------------------------------
*	GEM Application Environment Services		  Version 1.1
*	Serial No.  XXXX-0000-654321		  All Rights Reserved
*	Copyright (C) 1985			Digital Research Inc.
*	-------------------------------------------------------------
*/

#include <portab.h>
#include <machine.h>
#include <struct88.h>
#include <baspag88.h>
#include <obdefs.h>
#include <taddr.h>
#include <gemlib.h>

#define X_READ	0x3F

#define NUM_OBS LWGET(rs_hdr + 2*R_NOBS)
#define NUM_TREE LWGET(rs_hdr + 2*R_NTREE)
#define NUM_TI LWGET(rs_hdr + 2*R_NTED)
#define NUM_IB LWGET(rs_hdr + 2*R_NICON)
#define NUM_BB LWGET(rs_hdr + 2*R_NBITBLK)
#define NUM_FRSTR LWGET(rs_hdr + 2*R_NSTRING)
#define NUM_FRIMG LWGET(rs_hdr + 2*R_IMAGES)

#define ROB_TYPE (psubstruct + 6)		/* Long pointer in OBJECT   */
#define ROB_SPEC (psubstruct + 12)		/* Long pointer in OBJECT   */

#define RTE_PTEXT (psubstruct + 0)		/* Long pointers in TEDINFO */
#define RTE_PTMPLT (psubstruct + 4)
#define RTE_PVALID (psubstruct + 8)
#define RTE_TXTLEN (psubstruct + 24)
#define RTE_TMPLEN (psubstruct + 26)

#define RIB_PMASK (psubstruct + 0)		/* Long pointers in ICONBLK */
#define RIB_PDATA (psubstruct + 4)
#define RIB_PTEXT (psubstruct + 8)

#define RBI_PDATA (psubstruct + 0)		/* Long pointer in BITBLK   */
#define RBI_WB (psubstruct + 4)
#define RBI_HL (psubstruct + 6)
					/* in global array      */
#define APP_LOPNAME (rs_global + 10)
#define APP_LO1RESV (rs_global + 14)
#define APP_LO2RESV (rs_global + 18)

					/* in DOS.C         */
extern int16_t dos_open();

extern int32_t dos_lseek();

extern int16_t dos_read();

extern int16_t dos_close();

extern int32_t dos_alloc();

extern int16_t dos_free();

extern int16_t fm_error();

extern int32_t ad_shcmd;

extern int32_t ad_sysglo;

extern int16_t DOS_ERR;

extern int16_t DOS_AX;

extern int16_t gl_width;

extern int16_t gl_wchar;

extern int16_t gl_hchar;

extern THEGLO D;

extern PD *rlr;

int32_t rs_hdr;

int32_t rs_global;

uint16_t hdr_buff[HDR_LENGTH / 2];


/*
*	Fix up a character position, from offset,row/col to a pixel value.
*	If column or width is 80 then convert to rightmost column or 
*	full screen width. 
*/
VOID fix_chpos(pfix, ifx)
int32_t pfix;

int16_t ifx;
{
	register int16_t cpos,
	 coffset;

	cpos = LWGET(pfix);
	coffset = (cpos >> 8) & 0x00ff;
	cpos &= 0x00ff;
	if ((ifx) && (cpos == 80))
		cpos = gl_width;
	else
		cpos *= (ifx) ? gl_wchar : gl_hchar;
	cpos += (coffset > 128) ? (coffset - 256) : coffset;
	LWSET(pfix, cpos);
}


/************************************************************************/
/* rs_obfix								*/
/************************************************************************/
VOID rs_obfix(tree, curob)
int32_t tree;

int16_t curob;
{
	register int16_t i,
	 val;

	register int32_t p;

	/* set X,Y,W,H with */
	/*   fixch, use val */
	/*   to alternate TRUEs */
	/*   and FALSEs     */
	p = OB_X(curob);

	val = TRUE;
	for (i = 0; i < 4; i++)
	{
		fix_chpos(p + (int32_t) (2 * i), val);
		val = !val;
	}
}


char * rs_str(stnum)
{
	int32_t ad_string;

	rs_gaddr(ad_sysglo, R_STRING, stnum, &ad_string);
	LSTCPY(&D.g_loc1[0], ad_string);
	return (&D.g_loc1[0]);
}

int32_t get_sub(rsindex, rtype, rsize)
int16_t rsindex,
	rtype,
	rsize;
{
	uint16_t offset;

	offset = LWGET(rs_hdr + LW(rtype * 2));
	/* get base of objects  */
	/*   and then index in  */
	return (rs_hdr + LW(offset) + LW(rsize * rsindex));
}


/*
 *	return address of given type and index, INTERNAL ROUTINE
*/
int32_t get_addr(rstype, rsindex)
register uint16_t rstype;

register uint16_t rsindex;
{
	register int32_t psubstruct;
	register int16_t size;
	register int16_t rt;
	int16_t valid;
	uint16_t junk;

	valid = TRUE;
	switch (rstype)
	{
	case R_TREE:
#ifdef __ALCYON__
		junk = LW(rsindex * 4);
		return (LLGET(LLGET(APP_LOPNAME) + junk));
#else
		return (LLGET(LLGET(APP_LOPNAME) + LW(rsindex * 4)));
#endif
	case R_OBJECT:
		rt = RT_OB;
		size = sizeof(OBJECT);
		break;
	case R_TEDINFO:
	case R_TEPTEXT:
		rt = RT_TEDINFO;
		size = sizeof(TEDINFO);
		break;
	case R_ICONBLK:
	case R_IBPMASK:
		rt = RT_ICONBLK;
		size = sizeof(ICONBLK);
		break;
	case R_BITBLK:
	case R_BIPDATA:
		rt = RT_BITBLK;
		size = sizeof(BITBLK);
		break;
	case R_OBSPEC:
		psubstruct = get_addr(R_OBJECT, rsindex);
		return (ROB_SPEC);
	case R_TEPTMPLT:
	case R_TEPVALID:
		psubstruct = get_addr(R_TEDINFO, rsindex);
		if (rstype == R_TEPTMPLT)
			return (RTE_PTMPLT);
		else
			return (RTE_PVALID);
	case R_IBPDATA:
	case R_IBPTEXT:
		psubstruct = get_addr(R_ICONBLK, rsindex);
		if (rstype == R_IBPDATA)
			return (RIB_PDATA);
		else
			return (RIB_PTEXT);
	case R_STRING:
		return (LLGET(get_sub(rsindex, RT_FREESTR, sizeof(int32_t))));
	case R_IMAGEDATA:
		return (LLGET(get_sub(rsindex, RT_FREEIMG, sizeof(int32_t))));
	case R_FRSTR:
		rt = RT_FREESTR;
		size = sizeof(int32_t);
		break;
	case R_FRIMG:
		rt = RT_FREEIMG;
		size = sizeof(int32_t);
		break;
	default:
		valid = FALSE;
		break;
	}
	if (valid)
		return (get_sub(rsindex, rt, size));
	else
		return (-1L);
}										/* get_addr() */


fix_trindex()
{
	register int16_t ii;

	register int32_t ptreebase;

	ptreebase = get_sub(0, RT_TRINDEX, sizeof(int32_t));
	LLSET(APP_LOPNAME, ptreebase);

	for (ii = NUM_TREE - 1; ii >= 0; ii--)
		fix_long(ptreebase + LW(ii * 4));
}


/*	Fix up the G_ICON table		*/

VOID fix_cicon()
{
	int32_t *ctable;

	uint16_t *header;

	header = rs_hdr;
	if (header[RT_VRSN] & 0x0004)		/* if extended type */
	{
		ctable = rs_hdr + (int32_t) header[RS_SIZE];
		if (ctable[1] && (ctable[1] != -1))
			get_colo_rsc(ctable[1] + rs_hdr);
	}
}


/*	Fix up the objects including color icons	*/

VOID fix_objects()
{
	register int16_t ii;

	register int16_t obtype;

	register int32_t psubstruct;

	int32_t *ctable;

	uint16_t *header;

	header = rs_hdr;

	if (header[RT_VRSN] & 0x0004)
	{
		ctable = rs_hdr + (int32_t) header[RS_SIZE];
		ctable = ctable[1] + rs_hdr;
	} else
		ctable = 0x0L;

	for (ii = NUM_OBS - 1; ii >= 0; ii--)
	{
		psubstruct = get_addr(R_OBJECT, ii);
		rs_obfix(psubstruct, 0);
		obtype = (LWGET(ROB_TYPE) & 0x00ff);
		if ((obtype == G_CICON) && ctable)
			*((int32_t *) ROB_SPEC) = ctable[*((int32_t *) (ROB_SPEC))];

		if ((obtype != G_BOX) && (obtype != G_IBOX) && (obtype != G_BOXCHAR) && (obtype != G_CICON))
			fix_long(ROB_SPEC);
	}
}


fix_tedinfo()
{
	register int16_t ii, i;
	register int32_t psubstruct;
	int32_t tl[2], ls[2];

	for (ii = NUM_TI - 1; ii >= 0; ii--)
	{
		psubstruct = get_addr(R_TEDINFO, ii);
		tl[0] = tl[1] = 0x0L;
		if (fix_ptr(R_TEPTEXT, ii))
		{
			tl[0] = RTE_TXTLEN;
			ls[0] = RTE_PTEXT;
		}
		if (fix_ptr(R_TEPTMPLT, ii))
		{
			tl[1] = RTE_TMPLEN;
			ls[1] = RTE_PTMPLT;
		}
		for (i = 0; i < 2; i++)
		{
			if (tl[i])
				LWSET(tl[i], strlen(LLGET(ls[i])) + 1);
		}
		fix_ptr(R_TEPVALID, ii);
	}
}


int16_t fix_nptrs(cnt, type)
int16_t cnt;
int16_t type;
{
	register int16_t i;

	for (i = cnt; i >= 0; i--)
		fix_long(get_addr(type, i));
}


int16_t fix_ptr(type, index)
int16_t type;
int16_t index;
{
	return (fix_long(get_addr(type, index)));
}


int16_t fix_long(plong)
register int32_t plong;
{
	register int32_t lngval;

	lngval = LLGET(plong);
	if (lngval != -1L)
	{
		LLSET(plong, rs_hdr + lngval);
		return (TRUE);
	} else
		return (FALSE);
}


/*
*	Set global addresses that are used by the resource library sub-
*	routines
*/
VOID rs_sglobe(pglobal)
int32_t pglobal;
{
	rs_global = pglobal;
	rs_hdr = LLGET(APP_LO1RESV);
}


/*
*	Free the memory associated with a particular resource load.
*/
int16_t rs_free(pglobal)
int32_t pglobal;
{
	uint16_t *header;

	int32_t *ctable;

	rs_sglobe(pglobal);					/* set global values */

	header = rs_hdr;

	if (header[RT_VRSN] & 0x0004)		/* extended format */
	{
		ctable = rs_hdr + (int32_t) header[RS_SIZE];
		if (ctable[1] && (ctable[1] != -1))
		{
			ctable = ctable[1] + rs_hdr;
			free_cicon(ctable);
		}
	}

	dos_free(rs_hdr);
	return (!DOS_ERR);

}										/* rs_free() */


/*
*	Get a particular ADDRess out of a resource file that has been
*	loaded into memory.
*/
int16_t rs_gaddr(pglobal, rtype, rindex, rsaddr)
int32_t pglobal;

uint16_t rtype;

uint16_t rindex;

register int32_t *rsaddr;
{
	rs_sglobe(pglobal);

	*rsaddr = get_addr(rtype, rindex);
	return (*rsaddr != -1L);
}										/* rs_gaddr() */


/*
*	Set a particular ADDRess in a resource file that has been
*	loaded into memory.
*/
VOID rs_saddr(pglobal, rtype, rindex, rsaddr)
int32_t pglobal;

uint16_t rtype;

uint16_t rindex;

int32_t rsaddr;
{
	register int32_t psubstruct;

	rs_sglobe(pglobal);

	psubstruct = get_addr(rtype, rindex);
	if (psubstruct != -1L)
	{
		LLSET(psubstruct, rsaddr);
		return (TRUE);
	} else
		return (FALSE);
}										/* rs_saddr() */


/*
*	Read resource file into memory and fix everything up except the
*	x,y,w,h, parts which depend upon a GSX open workstation.  In the
*	case of the GEM resource file this workstation will not have
*	been loaded into memory yet.
*/
int16_t rs_readit(pglobal, rsfname)
int32_t pglobal;

int32_t rsfname;
{
	register uint16_t fd,
	 ret;

	int32_t rslsize;

	char rspath[128];

	/* make sure its there  */
	LSTCPY(rspath, rsfname);
	if (!sh_find(rspath, NULL))
		return (FALSE);
	/* init global      */
	rs_global = pglobal;
	/* open then file and   */
	/*   read the header    */
	fd = dos_open(rspath, RMODE_RD);
	if (!DOS_ERR)
		dos_read(fd, HDR_LENGTH, ADDR(&hdr_buff[0]));

	/* read in resource and */
	/*   interpret it   */
	if (!DOS_ERR)
	{
		/* get size of resource */

		if (hdr_buff[RT_VRSN] & 0x0004)	/* New format?      */
		{								/* seek to the 1st entry */
			/* of the table     */
			if (dos_lseek(fd, SMODE, (int32_t) (hdr_buff[RS_SIZE])) != hdr_buff[RS_SIZE])
			{
				ret = FALSE;
				goto rs_exit;
			}
			/* read the size    */
			dos_read(fd, sizeof(int32_t), &rslsize);
			if (DOS_ERR)
				goto rs_end;

		} else
			rslsize = hdr_buff[RS_SIZE];
		/* allocate memory  */
		rs_hdr = dos_alloc(rslsize);

		if (!DOS_ERR)
		{
			dos_lseek(fd, SMODE, 0x0L);	/* read it all in   */
			trap(X_READ, fd, rslsize, rs_hdr);
/*	    dos_read(fd, rslsize, rs_hdr);	*/
			if (!DOS_ERR)
				do_rsfix(rs_hdr, (uint16_t) rslsize);	/* do all the fixups    */
		}
	}

  rs_end:								/* close file and return */
	ret = !DOS_ERR;
  rs_exit:
	dos_close(fd);
	return (ret);
}


/* do all the fixups. rs_hdr must be initialized	*/

VOID do_rsfix(hdr, size)
int32_t hdr;

int16_t size;
{
	register int16_t ibcnt;

	LLSET(APP_LO1RESV, hdr);
	LWSET(APP_LO2RESV, size);
	/* xfer RT_TRINDEX to global    */
	/*   and turn all offsets from  */
	/*   base of file into pointers */

	fix_cicon();						/* fix color icon       */
	fix_trindex();
	fix_tedinfo();
	ibcnt = NUM_IB - 1;
	fix_nptrs(ibcnt, R_IBPMASK);
	fix_nptrs(ibcnt, R_IBPDATA);
	fix_nptrs(ibcnt, R_IBPTEXT);
	fix_nptrs(NUM_BB - 1, R_BIPDATA);
	fix_nptrs(NUM_FRSTR - 1, R_FRSTR);
	fix_nptrs(NUM_FRIMG - 1, R_FRIMG);
}


/*
*	Fix up objects separately so that we can read GEM resource before we
*	do an open workstation, then once we know the character sizes we
*	can fix up the objects accordingly.
*/

VOID rs_fixit(pglobal)
int32_t pglobal;
{
	rs_sglobe(pglobal);
	fix_objects();
}


/*
*	RS_LOAD		mega resource load
*/
int16_t rs_load(pglobal, rsfname)
register int32_t pglobal;

int32_t rsfname;
{
	register int16_t ret;

	ret = rs_readit(pglobal, rsfname);
	if (ret)
		rs_fixit(pglobal);
	return (ret);
}
