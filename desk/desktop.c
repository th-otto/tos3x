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
#ifdef BITBLT
STATIC USERBLK chxcache;
#endif
STATIC CICONBLK *ciconaddr;
int16_t pglobal[15];
int16_t gl_apid;

/* for MULITLANG_SUPPORT: */
uint16_t st_time;		/* time code        */
uint16_t st_date;
uint16_t st_dchar;
int16_t st_keybd;

int16_t ch_xcache PROTO((NOTHING));
BOOLEAN re_icon PROTO((NOTHING));
BOOLEAN ini_icon PROTO((NOTHING));
VOID adj_menu PROTO((int16_t which));
VOID ini_rsc PROTO((NOTHING));
VOID ch_machine PROTO((NOTHING));
int32_t inq_cache PROTO((int32_t data));
VOID adjdcol PROTO((uint16_t color));
VOID adjobjects PROTO((NOTHING));

#undef Blitmode
#define Blitmode(a) trp14(64, a)


int16_t ch_xcache(NOTHING)
{
	ch_cache(FALSE);
	return 0;
}



/*
 * Read in icon resource file
 */
BOOLEAN re_icon(NOTHING)
{
	register int16_t i;
	char temp[30];
	char **ptr;
	char buf2[18];
	char *iaddr;

	UNUSED(i);
	
	LBCOPY(temp, pglobal, 30);			/* save the pglobal */

	strcpy(buf2, icndata);
	buf2[0] = (isdrive() & 0x4) ? 'C' : 'A';

	if (ctldown) /* WTF; this is from AES */
		return FALSE;

	if (!rsrc_load(buf2))
		return FALSE;

	ptr = (char **)&pglobal[7];			/* get the new rsc address  */
	iaddr = *ptr;

	if (iconmem)						/* free up memory   */
	{									/* use rs_free to free memory */
		*ptr = iconmem;					/* because it may have color icons */
		rs_free((intptr_t) pglobal);
	}

	iconmem = iaddr;
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
#if COLORICON_SUPPORT
		obj[i].ob_type = G_CICON;		/* 7/11/92 */
#else
		obj[i].ob_type = G_ICON;		/* 7/11/92 */
#endif
		obj[i].ob_flags = HIDETREE;
		obj[i].ob_state = NORMAL;
		obj[i].ob_width = dicon.g_w;
		obj[i].ob_height = dicon.g_h;
		icblk = (CICONBLK *) (iconaddr[0].ob_spec);
#if COLORICON_SUPPORT
		itype->i_cicon = *icblk;
		itype->i_cicon.monoblk.ib_ptext = itype->i_name;
		obj[i].ob_spec = (intptr_t)&itype->i_cicon;
#else
		itype->i_iblk = icblk->monoblk;
		itype->i_iblk.ib_ptext = itype->i_name;
		obj[i].ob_spec = (intptr_t)&itype->i_iblk;
#endif
		itype->i_path = NULL;
	}

	return TRUE;
}


/*
 * Shift the menu
 */
VOID adj_menu(P(int16_t) which)
PP(int16_t which;)
{
	OBJECT *obj;
	int16_t w, x, y;

	obj = menu_addr;					/* shift the menu   */
	objc_offset(menu_addr, which, &x, &y);

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
VOID ini_rsc(NOTHING)
{
	register OBJECT *obj;
	GRECT pt;
	ICONBLK *iblk;
	int16_t w, i;
	CICONBLK *icblk;

	rom_ram(1, (intptr_t)pglobal);				/* load in resource     */

	menu_addr = get_tree(ADMENU);		/* get the menu address     */

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

	/* 	Allocate memory for color icons 
	 * 	These should go away if we have a RCS that can handle color icon 
	 */
	
	if (!ciconaddr)						/* 7/10/92 */
	{
		if ((ciconaddr = (CICONBLK *)Malloc((int32_t) (sizeof(CICONBLK) * maxicon))))
		{
			for (i = 0; i < maxicon; i++)
			{
				/* ciconaddr[i].monoblk = *((ICONBLK*)background[i+1].ob_spec);	*/
				background[i + 1].ob_spec = (intptr_t)&ciconaddr[i];
				background[i + 1].ob_type = G_CICON;
			}
		} else
		{
			Cconws("Color icon failed \r\n");
#if BINEXACT
			return FALSE;
#else
			return;
#endif
		}
	}

	rc_copy(&full, (GRECT *)&background[0].ob_x);

	/* Precalculate the disk icon's pline values */

	if (iconaddr[0].ob_type == G_CICON)
	{
		icblk = (CICONBLK *) iconaddr[0].ob_spec;
		iblk = &icblk->monoblk;
	} else
	{
		iblk = (ICONBLK *) (iconaddr[0].ob_spec);
	}
	
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



BOOLEAN deskmain(NOTHING)
{
	register int16_t i;
	BOOLEAN ret;
	int16_t handle, x;
	int16_t *ptr;
	char temp[30];
	VOIDPTR *lptr;

	UNUSED(x);
	UNUSED(handle);
	
	if (!inf_path[0])					/* Not set up yet       */
		m_infpath(inf_path);

#if 0
	ciconaddr = NULL;					/* 7/10/92 */
#endif

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

	if (!mem_init())					/* init the app path buffer */
	{									/* and desk app buffer      */
m_2:
		desknoerr();
		goto m_1;
	}

	/* initalize all the windows before reading in inf file */

	ini_windows();

	d_maxcolor = gl_ws.ws_ncolors;

#if AESVERSION >= 0x330
	if (!do_once)						/* do it once only  */
	{
		adjdcol(WHITE);					/* adjust dialogue box's color  */
		adjobjects();					/* adjust object positions      */
		do_once = TRUE;
	}
#endif

	read_inf();							/* Let see what the user want   */

	q_inf();							/* make a copy of inf file  */

	desknoerr();

	pref_save = gl_restype;				/* get the real res     */
	/* change the menu bar      */
	do_view((s_view == S_ICON) ? ICONITEM : TEXTITEM);

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
	/* set up the right menu text */
	/* do it here!!!!!! */
#ifdef BITBLT /* take out for sparrow */
#if STR_IN_RSC
	strcpy((char *)menu_addr[BITBLT].ob_spec, get_fstring(m_cpu == 30 ? CACHETXT : BLTTXT));
#else
	strcpy((char *)menu_addr[BITBLT].ob_spec, m_cpu == 30 ? Cachetxt : Blttxt);
#endif

	menu_addr[SUPERITEM].ob_type = G_USERDEF;
	chxcache.ub_code = (intptr_t)ch_xcache;
	menu_addr[SUPERITEM].ob_spec = (intptr_t)&chxcache;
#endif


	ch_cache(TRUE);						/* set the cache    */

	xprint = TRUE;
	/* check desktop file   */
	put_keys();							/* set up the key menu  */

	wind_update(TRUE);
	menu_bar(menu_addr, TRUE);			/* show the menu    */

	do_redraw(0, &full, 0);
	wind_update(FALSE);

	open_def();							/* open default windows */
	desk_wait(FALSE);

	actions();							/* handle the events    */

	wind_update(TRUE);					/* Block out any more events    */
	desk_wait(TRUE);
	free_windows();						/* free up all windows  */

	wind_set(0, WF_NEWDESK, 0x0L, 0, 0);

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
		if (ciconaddr)					/* 7/10/92  */
		{
			Mfree(ciconaddr);
			ciconaddr = NULL;
		}

		iconaddr = (OBJECT *) 0;
		if (iconmem)
		{
			LBCOPY(temp, pglobal, 30);	/* construct the pglobal   */
			ptr = (int16_t *)temp;
			lptr = (VOIDPTR *)&ptr[7];
			*lptr = iconmem;
			rs_free((intptr_t) temp);
			iconmem = NULL;
		}
		ret = FALSE;					/* resolution change    */
		inf_path[0] = 0;
		gl_rschange = TRUE;
	}

	return ret;
}


/*
 * Check the machine type and set res table
 */
VOID ch_machine(NOTHING)
{
	int32_t value;

	m_st = TRUE;
	/* _VDO */
	if (getcookie(0x5F56444FL, &value))
	{									/* 7/16/92 */
		if (((value >> 16) & 0x0000FFFFL) >= 0x02)	/* TT   */
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



#if BINEXACT
/* same ugly Alcyon-only hack as in AES: relies on data being assigned to d7 */
int32_t inq_cache(P(int32_t) data)
PP(register int32_t data;)
{
	asm("dc.w $4e7a,$0002");			/* movec.l cacr,d0        */

	if (data != -1)
		asm("dc.w $4e7b,$7002");		/* movec.l d7,cacr      */

	asm("dc.w $4e7a,$0002");			/* movec.l cacr,d0        */
}
#endif


/*
 * Turn on the cache or bitblt
 */
VOID ch_cache(P(BOOLEAN) set)
PP(BOOLEAN set;)
{
	int16_t value;
	int32_t data;
	int16_t temp;

	UNUSED(value);
#if 0									/* take out for sparrow */
	menu_addr[BITBLT].ob_state &= ~DISABLED;
	menu_ienable(menu_addr, BITBLT, TRUE);
#endif

	if (m_cpu == 30)
	{
		if (set)
			data = s_cache ? CACHE_ON : CACHE_OFF;
		else
			data = 0xFFFFFFFFL;

		if (inq_cache(data) == XCA_ON)
			s_cache = TRUE;
		else
			s_cache = FALSE;

		value = s_cache;
	} else
	{									/* turn the blt on  */
		/* blt is there     */
	ch_1:
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
#if 0									/* take out for sparrow */
			menu_addr[BITBLT].ob_state |= DISABLED;
			menu_ienable(menu_addr, BITBLT, FALSE);
#endif
		}
	}

#if 0									/* take out for sparrow */
	menu_icheck( menu_addr, BITBLT, value ? TRUE : FALSE );
	if (value)
		menu_addr[BITBLT].ob_state |= CHECKED;
	else
		menu_addr[BITBLT].ob_state &= ~CHECKED;
#endif
}



/*
 * adjust object colors if it is invalid
 * This routine should no longer be necessary, but is left
 * in for the TOS 4.02 release as a precautionary measure.
 * objc_draw now understands that "hollow, white, 3D" objects
 * should be drawn in the appropriate 3D color, or white if
 * there aren't enough colors, and the resources have been
 * updated accordingly	++ERS 1/19/93
 */
VOID adjdcol(P(uint16_t) color)
PP(uint16_t color;)
{
	register OBJECT *obj;

	if (gl_ws.ws_ncolors > LWHITE)
		return;

	obj = get_tree(ADFILEIN);
	obj[FILEFT].ob_spec = (obj[FILEFT].ob_spec & 0xfffffff0) | color;
	obj[FIRIGHT].ob_spec = (obj[FIRIGHT].ob_spec & 0xfffffff0) | color;

	obj = get_tree(ADINSDIS);
	obj[IUP].ob_spec = (obj[IUP].ob_spec & 0xfffffff0) | color;
	obj[IDOWN].ob_spec = (obj[IDOWN].ob_spec & 0xfffffff0) | color;

	obj = get_tree(ADFORMAT);
	obj[SRCDRA].ob_spec = (obj[SRCDRA].ob_spec & 0xfffffff0) | color;
	obj[SRCDRB].ob_spec = (obj[SRCDRB].ob_spec & 0xfffffff0) | color;
	obj[ADRIVE].ob_spec = (obj[ADRIVE].ob_spec & 0xfffffff0) | color;
	obj[BDRIVE].ob_spec = (obj[BDRIVE].ob_spec & 0xfffffff0) | color;

	obj = get_tree(INWICON);
	obj[WUP].ob_spec = (obj[WUP].ob_spec & 0xfffffff0) | color;
	obj[WDOWN].ob_spec = (obj[WDOWN].ob_spec & 0xfffffff0) | color;

	obj = get_tree(MNSYSTEM);
	obj[MFLEFT].ob_spec = (obj[MFLEFT].ob_spec & 0xfffffff0L) | color;
	obj[MFRIGHT].ob_spec = (obj[MFRIGHT].ob_spec & 0xfffffff0L) | color;
	obj[MFUP].ob_spec = (obj[MFUP].ob_spec & 0xfffffff0L) | color;
	obj[MFDOWN].ob_spec = (obj[MFDOWN].ob_spec & 0xfffffff0L) | color;
	obj[MKUPS].ob_spec = (obj[MKUPS].ob_spec & 0xfffffff0L) | color;
	obj[MKDOWNS].ob_spec = (obj[MKDOWNS].ob_spec & 0xfffffff0L) | color;
}


/*
 * Adjust Object Positions
 */
VOID adjobjects(NOTHING)
{
	register OBJECT *obj;
	int16_t x, y, w, h, dx, dy;

	obj = get_tree(ADINSDIS);
#if AES3D
	objc_gclip((LPTREE)obj, IUP, &dx, &dy, &x, &y, &w, &h);
#else
	objc_offset(obj, IUP, &x, &y);
#endif
	obj[IDOWN].ob_y = h + obj[IUP].ob_y;
	obj[IDRIVE].ob_y += 2;
	obj[ITRASH].ob_y += 2;
	obj[IPRINTER].ob_y += 2;

	obj = get_tree(INWICON);
#if AES3D
	objc_gclip((LPTREE)obj, WUP, &dx, &dy, &x, &y, &w, &h);
#else
	objc_offset(obj, WUP, &x, &y);
#endif
	obj[WDOWN].ob_y = h + obj[WUP].ob_y;
	obj[WFOLDER].ob_height = gl_hchar;
	obj[WNONE].ob_height = gl_hchar;
	obj[WOK].ob_height = gl_hchar;
	obj[WREMOVE].ob_height = gl_hchar;
	obj[WSKIP].ob_height = gl_hchar;
	obj[WQUIT].ob_height = gl_hchar;

	obj = get_tree(MNSYSTEM);
#if AES3D
	objc_gclip((LPTREE)obj, MFUP, &dx, &dy, &x, &y, &w, &h);
#else
	objc_offset(obj, MFUP, &x, &y);
#endif
	obj[MFDOWN].ob_y = h + obj[MFUP].ob_y;
#if AES3D
	objc_gclip((LPTREE)obj, MKUPS, &dx, &dy, &x, &y, &w, &h);
#else
	objc_offset(obj, MKUPS, &x, &y);
#endif
	obj[MKDOWNS].ob_y = h + obj[MKUPS].ob_y;
	obj[MFBASE].ob_height += 2;

	obj = get_tree(ADFILEIN);
	obj[FIOK].ob_y += 2;
	obj[FISKIP].ob_y += 2;
	obj[FICNCL].ob_y += 2;
	obj[FIRONLY].ob_y -= 1;
	obj[FIRWRITE].ob_y -= 1;
}
