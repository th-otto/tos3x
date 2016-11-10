/*      DESKPREF.C              3/17/89 - 6/15/89       Derek Mui       */
/*      Change at set color and pattern 6/29/90         D.Mui           */
/*      Use m_st to determine the resolution    9/19/90 D.Mui           */
/*      Fixed at col_par_pref for window background 7/7/92      D.Mui   */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"


/*
 * Set preferences dialog
 */
#if !POPUP_SUPPORT
/* 306de: 00e316e8 */
VOID desk_pref(NOTHING)
{
	register OBJECT *obj;
	register int16_t cyes, i, flag;
	int16_t overwrite, font;

	UNUSED(font);
	UNUSED(overwrite);
	
	obj = get_tree(ADSETPREF);
	cyes = cdele_save;
	obj[SPCDYES].ob_state = cyes;
	obj[SPCDNO].ob_state = !cyes;

	cyes = write_save;
	obj[YWRITE].ob_state = !cyes;
	obj[NWRITE].ob_state = cyes;

	cyes = ccopy_save;
	obj[SPCCYES].ob_state = cyes;
	obj[SPCCNO].ob_state = !cyes;

	for (i = SPLOW; i <= SPEXT3; i++)	/* hopefully they are in order  */
		obj[i].ob_state = NORMAL;

	/* set the resolution button */
	for (i = 0, flag = SPLOW; i < 6; i++, flag++)
	{
		if (!restable[i])
			obj[flag].ob_state = DISABLED;
	}

	if (m_st)
	{
		for (i = SPEXT1; i <= SPEXT3; i++)
#if AES3D
			obj[i].ob_flags |= HIDETREE;
#else
			obj[i].ob_flags = HIDETREE;
#endif
	}

	if (restable[4])					/* TT high res  */
		cyes = font_save;

	/* gl_restype is set according to followings:
	                                vdi handle
	1 = LOW RES     320 x 200       0, 2, 5, 7
	2 = MEDIUM RES  640 x 200       3               
	3 = HIGH RES    640 x 400       4
	4 = EXT1        640 x 480       6
	5 = EXT2        1280 x 960      8
	6 = EXT3        320 x 480       9
	
	*/

	switch (gl_restype)
	{
	case 1:
		flag = SPLOW;
		break;

	case 2:
		flag = SPMEDIUM;
		break;

	case 3:
		flag = SPHIGH;
		break;

	case 4:
		flag = SPEXT1;
		break;

	case 5:
		flag = SPEXT2;
		break;

	case 6:
		flag = SPEXT3;
		break;
	}

	obj[flag].ob_state = SELECTED;

	if (fmdodraw(ADSETPREF, 0) == SPOK)
	{
		cdele_save = inf_what(obj, SPCDYES, SPCDNO);
		ccopy_save = inf_what(obj, SPCCYES, SPCCNO);
		write_save = !inf_what(obj, YWRITE, NWRITE);

		flag = inf_gindex(obj, SPLOW, 6) + 1;

		if (app_reschange(flag))
			d_exit = L_CHGRES;
	}

}
#endif


/*
 * Set Color and pattern
 */
/* 306de: 00e318a0 */
VOID col_pa_pref(NOTHING)
{
	register OBJECT *obj;
	register int16_t ret;
	register WINDOW *win;
	int32_t newwin;
	int32_t newdesk;
	int32_t *outpat;
	int16_t last, i, j;

	obj = get_tree(SSCREEN);

	newdesk = background[0].ob_spec;
	newwin = (uint16_t) windspec;
	/* set the background pattern */
	last = ret = obj[DESKPREF].ob_state & SELECTED ? DESKPREF : WINPREF;

	for (j = COLOR0, i = d_maxcolor; i < 16; i++)
#if AES3D
		obj[j + i].ob_flags |= HIDETREE;
#else
		obj[j + i].ob_flags = HIDETREE;
#endif

	fm_draw(SSCREEN);

#if !BINEXACT
	outpat = 0; /* quiet compiler */
#endif
	goto c_1;

	while (TRUE)
	{
		ret = xform_do(obj, 0);
	c_1:
		if (ret == SOK)
			break;

		if (ret == SCANCEL)
		{
			do_finish(SSCREEN);
			return;
		}

		if (ret == DESKPREF)
		{
			outpat = &newdesk;
			obj[DESKPREF].ob_state = SELECTED;
			obj[WINPREF].ob_state = NORMAL;
			goto c_2;
		}

		if (ret == WINPREF)
		{
			outpat = &newwin;
			obj[WINPREF].ob_state = SELECTED;
			obj[DESKPREF].ob_state = NORMAL;
		c_2:
			draw_fld(obj, DESKPREF);
			draw_fld(obj, WINPREF);
			obj[PATTERN].ob_spec = *outpat;
		}

		if (ret >= PAT0 && ret <= PAT7)
		{
			obj[PATTERN].ob_spec &= 0xFFFFFF0FL;
			obj[PATTERN].ob_spec |= obj[ret].ob_spec & 0x000000F0L;
		} else /* color changed    */ if ((ret >= COLOR0) && (ret <= COLOR15))
		{
			obj[PATTERN].ob_spec &= 0xFFFFFFF0L;
			obj[PATTERN].ob_spec |= obj[ret].ob_spec & 0x0000000FL;
		}

		objc_draw(obj, OUTBOX, 2, 0, 0, full.g_w, full.g_h);
		*outpat = obj[PATTERN].ob_spec;
	}

	do_finish(SSCREEN);

	if (background[0].ob_spec != newdesk)
	{
		background[0].ob_spec = newdesk;
		do_redraw(0, &full, 0);
	}

	win = winhead;

	if (windspec != (uint16_t) newwin)
	{
		windspec = (uint16_t) newwin;
		while (win)
		{
#if (TOSVERSION >= 0x400) | !BINEXACT
			if (win->w_obj)				/* fixed 7/7/92 */
#endif
				win->w_obj[0].ob_spec = windspec;
			win = win->w_next;
		}

		win = w_gfirst();
		while (win)
		{
			do_redraw(win->w_id, &full, 0);
			win = w_gnext();
		}
	}
	UNUSED(last);
}
