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
 * Set Color and pattern
 */
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
		obj[j + i].ob_flags |= HIDETREE;

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
			if (win->w_obj)				/* fixed 7/7/92 */
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
