/*      DESKTOP.C               3/15/89 - 7/26/89       Derek Mui       */
/*      Take out vdi_handle     6/28/89                                 */
/*      Read in icn file        9/23/89                 D.Mui           */
/*      New adj_menu to adjust menu     6/27/90         D.Mui           */
/*      Take the wind_update in deskmain        8/9/90  D.Mui           */
/*      Add m_cpu to check CPU  9/19/90                 D.Mui           */
/*      Fix the cache menu so that it checks the cache when menu is down*/
/*                                      10/24/90        D.Mui           */
/*      Moved the wind_set in the main to just before menu_bar. It is   */
/*      to fix the acc problem                  12/4/90 D.Mui           */
/*      Change appl_init to ap_init and appl_exit to ap_exit 4/3/91     */
/*      Add wm_update before bringing up the menu bar 4/18/91 Mui       */
/*      Install desktop critical error handler  4/22/91 D.Mui           */
/*      Check control key for nodisk system at re_icon 8/13/91  D.Mui   */
/*      Change all the iconblk to ciconblk      7/11/92 D.Mui           */
/*      Added adjobjects() to adjust objects    8/06/92 cgee            */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"
#include "aesbind.h"
#include "ws.h"


BOOLEAN m_st;							/* machine type flag    */
int16_t m_cpu;							/* cpu type     */
int16_t numicon;						/* the number of icon in the resource   */
STATIC char *iconmem;					/* icon data memory address */
STATIC int16_t xprint;					/* do it once flag  */
char restable[6];						/* resolution table Low, Medium, High, TT Medium, TT High, TT Low */
int16_t d_maxcolor;
int16_t pglobal[15];
int16_t gl_apid;

#if MULTILANG_SUPPORT
uint16_t st_time;		/* time code        */
uint16_t st_date;
uint16_t st_dchar;
int16_t st_keybd;
#endif

BOOLEAN re_icon PROTO((NOTHING));
BOOLEAN ini_icon PROTO((NOTHING));
VOID adj_menu PROTO((int16_t which));
VOID ini_rsc PROTO((NOTHING));
VOID ch_machine PROTO((NOTHING));
int32_t inq_cache PROTO((int32_t data));

#undef Blitmode
#define Blitmode(a) trp14(64, a)


/*
 * Read in icon resource file
 */
/* 306de: 00e335e0 */
BOOLEAN re_icon(NOTHING)
{
	register int16_t i;
	char temp[30];
	char **ptr;
	char buf2[18];

	UNUSED(i);
	
	LBCOPY(temp, pglobal, 30);			/* save the pglobal */

	strcpy(buf2, icndata);
	buf2[0] = (isdrive() & 0x4) ? 'C' : 'A';

	if (ctldown) /* WTF; this is from AES */
		return FALSE;

	if (!rsrc_load(buf2))
		return FALSE;

	if (iconmem)						/* free up memory   */
	{
		Mfree(iconmem);
	}

	ptr = (char **)&pglobal[7];			/* get the new rsc address  */

	iconmem = *ptr;

	iconaddr = get_tree(0);
	numicon = iconaddr[0].ob_tail;
	iconaddr++;							/* get the icon address */
	LBCOPY(pglobal, temp, 30);

	rs_sglobe((intptr_t) pglobal);		/* restore the pglobal  */
	return TRUE;
}


/*
 * Initalize the icon and allocate backid memory
 * Change the G_ICON to G_CICON
 */
/* 306de: 00e336d8 */
BOOLEAN ini_icon(NOTHING)
{
	register int16_t i;
	register OBJECT *obj;
	register IDTYPE *itype;
	CICONBLK *icblk;

	backid = (IDTYPE *)Malloc((int32_t) (sizeof(IDTYPE) * (maxicon + 1)));

	if (!backid)
		return FALSE;

	obj = background;

	for (i = 1; i <= maxicon; i++)
	{
		itype = &backid[i];
		obj[i].ob_type = G_ICON;		/* 7/11/92 */
		obj[i].ob_flags = HIDETREE;
		obj[i].ob_state = NORMAL;
		obj[i].ob_width = dicon.g_w;
		obj[i].ob_height = dicon.g_h;
		icblk = (CICONBLK *) (iconaddr[0].ob_spec);
		itype->i_iblk = icblk->monoblk;
		itype->i_iblk.ib_ptext = itype->i_name;
		obj[i].ob_spec = (intptr_t)&itype->i_iblk;
		itype->i_path = NULL;
	}

	return TRUE;
}


/*
 * Shift the menu
 */
/* 306de: 00e337cc */
VOID adj_menu(P(int16_t) which)
PP(int16_t which;)
{
	OBJECT *obj;
	int16_t w, x, y;

	obj = d->rtree[ADMENU];					/* shift the menu   */
	objc_offset(d->rtree[ADMENU], which, &x, &y);

	w = obj[which].ob_width + x;
	if (w >= (full.g_w + full.g_x))
	{
		x = w - (full.g_w + full.g_x) + gl_wchar;
		obj[which].ob_x -= x;
	}
}


/*
 * Initalize the desktop resource
 */
/* 306de: 00e3385a */
VOID ini_rsc(NOTHING)
{
	register OBJECT *obj;
	GRECT pt;
	ICONBLK *iblk;
	int16_t w;

	rom_ram(1, (intptr_t)pglobal);				/* load in resource     */

	adj_menu(IDSKITEM - 1);
	adj_menu(ICONITEM - 1);

	if (!iconaddr)						/* if icon hasn't read in then read it again    */
	{
		if (!re_icon())
		{
			iconaddr = get_tree(ADICON);
			numicon = iconaddr[0].ob_tail;
			iconaddr++;
		}
	}

	background = get_tree(BACKGROUND);

	maxicon = background[0].ob_tail;	/* max background icon      */

	rc_copy(&full, (GRECT *)&background[0].ob_x);

	/* Precalculate the disk icon's pline values */

	iblk = (ICONBLK *) (iconaddr[0].ob_spec);
	
	rc_copy((GRECT *)&iblk->ib_xicon, &pt);		/* get the icon's xywh  */

	d_xywh[0] = pt.g_x;					/* disk icon outline    */
	d_xywh[3] = d_xywh[1] = pt.g_y;
	d_xywh[4] = d_xywh[2] = d_xywh[0] + pt.g_w;
	d_xywh[5] = d_xywh[3] + pt.g_h;

	rc_copy((GRECT *)&iblk->ib_xtext, &pt);
	d_xywh[8] = d_xywh[6] = pt.g_x + pt.g_w;
	d_xywh[13] = d_xywh[7] = pt.g_y;
	d_xywh[11] = d_xywh[9] = d_xywh[7] + pt.g_h;
	d_xywh[12] = d_xywh[10] = pt.g_x;
	d_xywh[16] = d_xywh[14] = d_xywh[0];
	d_xywh[15] = d_xywh[5];
	d_xywh[17] = d_xywh[1];

	dicon.g_x = 0;						/* precalculate text icon's x,y,w,h */
	dicon.g_y = 0;
	dicon.g_w = iblk->ib_wtext;
	dicon.g_h = iblk->ib_hicon + iblk->ib_htext;

	rc_copy(&dicon, &r_dicon);
	r_dicon.g_w += 5;
	r_dicon.g_h += 7;
	r_dicon.g_w += (full.g_w % r_dicon.g_w) / (full.g_w / r_dicon.g_w);
	r_dicon.g_h += (full.g_h % r_dicon.g_h) / (full.g_h / r_dicon.g_h);

	w = gl_wchar * 14;					/* text outline */

	obj = get_tree(TEXTLINES);
	f_xywh[9] = f_xywh[8] = f_xywh[6] = f_xywh[3] = f_xywh[1] = f_xywh[0] = 0;
	f_xywh[2] = w;
	f_xywh[4] = w;
	f_xywh[5] = obj[TSTYLE].ob_height;
	f_xywh[7] = obj[TSTYLE].ob_height;
}


/* 206de. 00e3008c */
/* 306de: 00e33af2 */
/* 104de: 00fdbc88 */
BOOLEAN deskmain(NOTHING)
{
	register int16_t i;
	BOOLEAN ret;
	int16_t handle, x;
	int16_t *ptr;

	UNUSED(x);
	UNUSED(handle);
	
	if (!inf_path[0])					/* Not set up yet       */
		m_infpath(inf_path);

top:
	gl_apid = ap_init((intptr_t) pglobal);			/* initalize the application    */

	deskerr();							/* install our critical error   */

	ret = TRUE;							/* assume everything is OK  */
	d_exit = L_NOEXIT;

	appnode = NULL;						/* No app buffer yet        */
	applist = NULL;						/* No app list yet      */
	apsize = 0;							/* Initalize app size       */

	desk_wait(TRUE);
	/* get the full window size */
	wind_get(0, WF_WORKXYWH, &full.g_x, &full.g_y, &full.g_w, &full.g_h);

	ini_rsc();							/* init the resource        */

	if (!ini_icon())					/* hide all the desktop icons   */
		goto m_2;

	/* initalize all the windows before reading in inf file */

	ini_windows();

	d_maxcolor = gl_ws.ws_ncolors;

	if (read_inf() == FALSE)							/* Let see what the user want   */
	{
		ap_exit();
		return FALSE;
	}
	
	q_inf();							/* make a copy of inf file  */

	/* change the menu bar      */
	do_view(s_view == S_ICON ? ICONITEM : TEXTITEM);

#if !BINEXACT
	i = NOSORT; /* quiet compiler */
#endif
	switch (s_sort)
	{
	case S_NAME:
		i = NAMEITEM;
		break;
	case S_DATE:
		i = DATEITEM;
		break;
	case S_SIZE:
		i = SIZEITEM;
		break;
	case S_TYPE:
		i = TYPEITEM;
		break;
	case S_NO:
		i = NOSORT;
	}

	wind_set(0, WF_NEWDESK, background, 0, 0);

	do_view(i);							/* fix up the menu bar  */

	ch_machine();						/* check the machine    */

	ch_cache(TRUE);						/* set the cache    */

	xprint = TRUE;
	/* check desktop file */
	put_keys();							/* set up the key menu  */

	wind_update(TRUE);
	menu_bar(d->rtree[ADMENU], TRUE);	/* show the menu    */

	do_redraw(0, &full, 0);
	wind_update(FALSE);

	open_def();							/* open default windows */
	desk_wait(FALSE);

	actions();							/* handle the events    */

	wind_update(TRUE);					/* Block out any more events    */
	desk_wait(TRUE);
	free_windows();						/* free up all windows  */

#if (TOSVERSION >= 0x400) | !BINEXACT
	wind_set(0, WF_NEWDESK, 0x0L, 0, 0);
#endif

	Mfree(lp_start);					/* free string buffer   */
	Mfree(appnode);						/* free app buffer  */
	Mfree(backid);
	Mfree(q_addr);						/* update inf file on disk  */
	menu_bar(0x0L, FALSE);
	wind_update(FALSE);					/* release window   */
m_1:
	ap_exit();
	/* Loop again */
	if (d_exit == L_READINF)
	{
		wind_new();						/* Read inf file    */
		goto top;
	}

	if (d_exit == L_CHGRES)				/* if reschange free the memory */
	{									/* start everything over again  */
		iconaddr = NULL;

		if (iconmem)
		{
			UNUSED(ptr);
			Mfree(iconmem);
			iconmem = NULL;
		}
		ret = FALSE;					/* resolution change    */
		inf_path[0] = 0;
	}

	return ret;
}


/*
 * Check the machine type and set res table
 */
/* 206de: 00e30354 */
/* 306de: 00e33dba */
VOID ch_machine(NOTHING)
{
	int32_t value;

	m_st = TRUE;
	/* _VDO */
	if (getcookie(0x5F56444FL, &value))
	{									/* 7/16/92 */
#if (TOSVERSION >= 0x400) | !BINEXACT
		if (((value >> 16) & 0x0000FFFFL) >= 0x02)	/* TT   */
#else
		if (value == 0x20000L)	/* TT   */
#endif
			m_st = FALSE;
	}
	/* _CPU */
	getcookie(0x5F435055L, &value);
	m_cpu = value;

	/* the restable is set according to gl_restype value    */

	bfill(6, 0, restable);				/* zero the table   */

	if (!m_st)							/* TT machine   */
	{
		if (gl_restype == 5)			/* TT High  */
		{
			restable[4] = 1;
		} else
		{
			bfill(4, 1, restable);		/* LOW MEDIUM HIGH  */
			restable[5] = 1;			/* TT-LOW MEDIUM    */
		}
	} else
	{
		if (gl_restype == 3)			/* ST HIGH      */
		{
			restable[2] = 1;
		} else
		{
			restable[0] = 1;
			restable[1] = 1;
		}
	}
}



/*
 * Turn on the cache or bitblt
 */
/* 206de: 00e30446 */
/* 306de: 00e33eac */
VOID ch_cache(P(BOOLEAN) set)
PP(BOOLEAN set;)
{
	int16_t value;
	int32_t data;
	int16_t temp;

	UNUSED(value);
	d->rtree[ADMENU][BITBLT].ob_state &= ~DISABLED;

	{									/* turn the blt on  */
		/* blt is there     */
		if ((temp = Blitmode(-1)) & 0x2)
		{
			if (set)
			{
				Blitmode(cbit_save ? 1 : 0);
				set = FALSE;
				goto ch_1;				/* check status again   */
			} else
			{
				cbit_save = (temp & 0x1) ? TRUE : FALSE;
			}
			value = cbit_save;
		} else
		{
			value = FALSE;
			d->rtree[ADMENU][BITBLT].ob_state |= DISABLED;
		}
	}

	if (value)
		d->rtree[ADMENU][BITBLT].ob_state |= CHECKED;
	else
		d->rtree[ADMENU][BITBLT].ob_state &= ~CHECKED;
}

