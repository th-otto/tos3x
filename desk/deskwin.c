/*      DESKWIN.C               3/16/89 - 6/15/89       Derek Mui       */
/*      Take out vdi_handle     6/28/89                                 */
/*      Put in error checking for window memory allocating 9/24/89      */
/*      Add up_1allwin          11/14/90                D.Mui           */
/*      Add up_2allwin          4/18/91                 D.Mui           */
/*      Fix at view_adjust for vertical slide box's size 7/9/91 D.Mui   */
/*      Alloc window at create_window                   7/7/92  D.Mui   */
/*      Change all the iconblk to ciconblk      7/11/92 D.Mui           */
/*      Change w_iblk to w_ciblk        7/11/92         D.Mui           */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"

static GRECT const w_sizes = { 150, 150, 150, 150 };
static OBJECT const blank = { -1, -1, -1, G_BOX, NONE, NORMAL, 0x000000F0L, 0, 0, 0, 0 };

BOOLEAN alloc_obj PROTO((int16_t mode, WINDOW *win));
#if TOSVERSION >= 0x400
VOID ret_win PROTO((WINDOW *win));
#endif


/*
 * Update the window info line
 */
/* 306de: 00e34308 */
VOID winfo(P(WINDOW *) win)
PP(WINDOW *win;)
{
	register int16_t i;
	register DIR *dir;
	uint16_t j;
	int32_t sizes;
	int32_t sizes1;
	struct {
		int32_t size;
		int16_t items;
		char filler[8];
	} buffer;
	const char *type;

	dir = win->w_memory;
	j = 0;
	sizes = 0;
	sizes1 = 0;

	for (i = 0; i < win->w_items; i++)
	{
		sizes += dir[i].d_size;
		if (dir[i].d_state & SELECTED)
		{
			sizes1 += dir[i].d_size;
			j++;
		}
	}

	if (j)								/* some items are selected  */
	{
		buffer.size = sizes1;
		buffer.items = j;
#if STR_IN_RSC
		type = get_fstring(j == 1 ? ISEL : ISELS);
#else
		type = j == 1 ? Isel : Isels;
#endif
	} else
	{
		buffer.size = sizes;
		buffer.items = (int16_t) win->w_items;
#if STR_IN_RSC
		type = get_fstring(win->w_items == 1 ? ISTR : ISTRS);
#else
		type = win->w_items == 1 ? Istr : Istrs;
#endif
	}

	merge_str(win->w_info, type, &buffer);
	wind_set(win->w_id, WF_INFO, win->w_info, 0, 0);
}


/* 306de: 00e34418 */
WINDOW *w_gnext(NOTHING)
{
	WINDOW *win;

	while (ww_win)
	{
		win = ww_win;
		ww_win = ww_win->w_next;
		if (win->w_id != -1)
			return win;
	}

	return NULL;
}


/* 306de: 00e34452 */
WINDOW *w_gfirst(NOTHING)
{
	ww_win = winhead;
	return w_gnext();
}



/*
 * Update all the windows, change dir path if fails
 */
/* 306de: 00e34466 */
VOID up_2allwin(P(const char *) path)
PP(const char *path;)
{
	register WINDOW *win;

	win = w_gfirst();

	while (win)
	{
		if (path[0] == win->w_path[0])
		{
			if (!open_subdir(win, 0, FALSE, FALSE, TRUE))
			{
				strcpy(&win->w_path[2], wilds);
				open_subdir(win, 0, FALSE, FALSE, TRUE);
			}
		}

		win = w_gnext();
	}
}


/*
 * Update all the windows
 * If the path has nothing, then update all the windows
 */
/* 306de: 00e344e4 */
VOID up_1allwin(P(const char *) path, P(BOOLEAN) dofull, P(BOOLEAN) change)
PP(const char *path;)
PP(BOOLEAN dofull;)								/* compare full path    */
PP(BOOLEAN change;)
{
	register WINDOW *win;

	win = w_gfirst();

	while (win)
	{
		if (!path[0])
			goto uu_1;

		if (dofull)						/* compare the full path    */
		{
			if (streq(path, win->w_path))
				goto uu_1;
		} else							/* just the id  */
		{
			if (path[0] == win->w_path[0])
			{
			uu_1:
				if (!up_win(win, change))
				{
					strcpy(&win->w_path[2], wilds);
					up_win(win, FALSE);
				}
			}
		}

		win = w_gnext();
	}
}


/* 306de: 00e3456c */
VOID up_allwin(P(const char *) path, P(BOOLEAN) dofull)
PP(const char *path;)
PP(BOOLEAN dofull;)
{
	up_1allwin(path, dofull, FALSE);
}



/*
 * Do the actual update of a particular window
 */
/* 306de: 00e34584 */
BOOLEAN up_win(P(WINDOW *) win, P(BOOLEAN) mediac)
PP(WINDOW *win;)
PP(BOOLEAN mediac;)
{
	char *ptr;
	BOOLEAN ret;

	ptr = win->w_path;

#if !BINEXACT
	ret = FALSE; /* BUG; maybe used uninitalized below */
#endif
	if (ptr[0] != CHAR_FOR_CARTRIDGE)
	{
		if (mediac)
		{
			desk_wait(TRUE);
			if ((ret = hit_disk((int16_t) ptr[0])))
				mediach((int16_t) (ptr[0] - 'A'));

			desk_wait(FALSE);

			if (!ret)
				return FALSE;
		}
	}

	if (!open_subdir(win, 0, FALSE, FALSE, TRUE))
	{
		if (mediac && ret)			/* it is force media change */
		{
			strcpy(&win->w_path[2], wilds);
			return open_subdir(win, 0, FALSE, FALSE, TRUE);
		}
	}

	return TRUE;
}


/*
 * Bring bottom window to top
 */
/* 306de: 00e3464c */
VOID bottop(NOTHING)
{
	register WINDOW *win;
	WINDOW *win1;
	int16_t item;
	int16_t buffer[8];

	UNUSED(item);
	if ((win = w_gfirst()) != NULL)
	{
		while (win)						/* get to the bottom one    */
		{
			win1 = win;
			win = w_gnext();
		}

		if (win1 != winhead)			/* stimulate a message call */
		{
			buffer[0] = WM_TOPPED;
			buffer[3] = win1->w_id;
			hd_msg(buffer);
		}
	}
}


/*
 * Allocate memory for window path
 */
/* 306de: 00e346a2 */
BOOLEAN path_alloc(P(int16_t) level)					/* requested level  */
PP(int16_t level;)
{
	int16_t oldlevel, wsize;
	int32_t size;
	register int16_t i;
	register char *addr;
	char *path;

	oldlevel = d_level;

	while (d_level <= level)
	{
		d_level = d_level * 2;
	}

	wsize = (13 * (d_level + 1)) + 3;
	size = (MAXWIN * 2 * (uint16_t) wsize) + 1;
	addr = path = (char *)Malloc(size);

	if (path)
	{
		path3 = addr;
		addr += wsize;

		for (i = 0; i < MAXWIN; i++)
		{
			if (winpd[i].w_path)
				strcpy(addr, winpd[i].w_path);
			else
				*addr = 0;

			winpd[i].w_path = addr;
			addr += wsize;
			winpd[i].w_buf = addr;
			addr += wsize;
			winpd[i].w_level = d_level;
		}

		free_path();

		d_path = path;
		return TRUE;
	} else
	{
		do1_alert(FCNOMEM);
	}
	
	d_level = oldlevel;					/* restore the old level    */
	return FALSE;
}


/* 306de: 00e347dc */
VOID free_path(NOTHING)
{
	if (d_path)
	{
#if 0
		form_alert(1, "[1][ Free Path Memory ][ OK ]");
#endif
		Mfree(d_path);
		d_path = NULL;
	}
}


/*
 * Clean up all the window
 */
/* 306de: 00e34804 */
VOID clr_allwin(NOTHING)
{
	register WINDOW *win;

	win = w_gfirst();

	while (win)
	{
		clr_xwin(win, TRUE);
		win = w_gnext();
	}
}


/*
 * Clean up and dselect the window objects
 */
/* 306de: 00e34832 */
VOID clr_xwin(P(WINDOW *) win, P(BOOLEAN) infoupdate)
PP(register WINDOW *win;)
PP(BOOLEAN infoupdate;)
{
	OBJECT *obj;
	register int16_t i;
	BOOLEAN change, ret;
	register DIR *dir;
	GRECT pt;

	dir = win->w_memory;
	obj = win->w_obj;
	ret = build_rect(obj, &pt, obj[1].ob_width, obj[1].ob_height);
	change = FALSE;

	for (i = 0; i < win->w_items; i++)
	{
		if (dir[i].d_state & SELECTED)
		{
			change = TRUE;
			dir[i].d_state = NORMAL;
		}
	}

	if (change)
	{
		set_newview(win->w_srtitem, win);
		if (ret)
			do_redraw(win->w_id, &pt, 0);
	}

	if (change && infoupdate)
		winfo(win);
}



/*
 * Move the vertical bar
 */
/* 306de: 00e348f4 */
VOID srl_verbar(P(WINDOW *) win, P(uint16_t) pos)
PP(register WINDOW *win;)
PP(uint16_t pos;)
{
	int32_t l;
	uint16_t i;
	int16_t diff, dir;

	l = pos * win->w_vvicons;
	i = l / 1000;

	if (i == win->w_rowi)				/* No change    */
		return;

	diff = i - win->w_rowi;				/* Up or down   */

	if (diff < 0)
	{
		diff *= -1;
		dir = SUP;
	} else
	{
		dir = SDOWN;
	}
	
	if (diff < win->w_xrow)				/* less than one window */
	{
		srl_row(win, diff, dir);
	} else
	{
		win->w_rowi = i;
		if (win->w_vvicons)
		{
			l = 1000 * win->w_rowi;
			l = l / win->w_vvicons;
		} else
		{
			l = 0;
		}
		
		wind_set(win->w_id, WF_VSLIDE, (uint16_t) l, 0, 0, 0);

		win->w_obj->ob_y = win->w_work.g_y;	/* reset at the topmost */
		win->w_srtitem = i * win->w_icol;
		up_1(win);
	}
}


/*
 * Move the horizontal bar
 */
/* 306de: 00e349ec */
VOID srl_hzbar(P(WINDOW *) win, P(uint16_t) pos)
PP(register WINDOW *win;)
PP(uint16_t pos;)
{
	int32_t l;
	uint16_t x, w;
	register OBJECT *obj;
	register int16_t diff, i;
	int16_t dir;

	l = pos * win->w_hvicons;
	i = l / 1000;

	if (i == win->w_coli)
		return;

	diff = i - win->w_coli;

	if (diff < 0)
	{
		dir = SRIGHT;
		diff *= -1;
	} else
	{
		dir = SLEFT;
	}
	
	if (diff < win->w_xcol)				/* less than one window */
	{
		srl_col(win, diff, dir);
	} else
	{
		win->w_coli = i;
		if (win->w_hvicons)
		{
			l = 1000 * win->w_coli;
			l /= win->w_hvicons;
		} else
		{
			l = 0;
		}
		
		wind_set(win->w_id, WF_HSLIDE, (uint16_t) l, 0, 0, 0);

		x = win->w_work.g_x;
		w = win->w_work.g_w;

		obj = win->w_obj;
		obj[0].ob_x = x;				/* reset at the leftmost    */
		obj[0].ob_width = w;
		i = i * fobj.g_w;
		obj[0].ob_x -= i;
		obj[0].ob_width += i;
		objc_draw(obj, ROOT, MAX_DEPTH, x, win->w_work.g_y, w, win->w_work.g_h);
	}
}



/*
 * Scroll the content down
 */
/* 306de: 00e34afe */
VOID srl_row(P(WINDOW *) win, P(int16_t) row, P(int16_t) dir)
PP(register WINDOW *win;)
PP(int16_t row;)
PP(int16_t dir;)
{
	int16_t i;

	if (dir == SDOWN)					/* scroll content dowm  */
	{
		i = win->w_vvicons - win->w_rowi;
		if (i)
		{
			if (row > i)
				row = i;

			set_newview(win->w_srtitem + (win->w_icol * row), win);
			blt_window(win, SDOWN, row);
		}
	} else if (win->w_rowi)				/* scroll content up    */
	{
		if ((win->w_rowi - row) < 0)
			row = win->w_rowi;

		set_newview(win->w_srtitem - (win->w_icol * row), win);
		blt_window(win, SUP, row);
	}
}


/* 306de: 00e34ba6 */
VOID srl_col(P(WINDOW *) win, P(int16_t) col, P(int16_t) dir)
PP(register WINDOW *win;)
PP(register int16_t col;)
PP(int16_t dir;)
{
	if (dir == SRIGHT)					/* scroll content right */
	{
		if (win->w_coli > 0)
		{
			if ((win->w_coli - col) < 0)
				col = win->w_coli;

			blt_window(win, SRIGHT, col);
		}
	} else /* scroll content left  */ if (win->w_coli < win->w_hvicons)
	{
		if ((win->w_coli + col) > win->w_hvicons)
			col = win->w_hvicons - win->w_coli;

		blt_window(win, SLEFT, col);
	}
}


/*
 * Bitblt a window content either up, dowm, left or right
 */
/* 306de: 00e34c12 */
VOID blt_window(P(WINDOW *) win, P(int16_t) mode, P(int16_t) size)
PP(register WINDOW *win;)
PP(int16_t mode;)
PP(int16_t size;)
{
	register OBJECT *obj;
	register int16_t x, y;
	int16_t e_x[12];						/* bitblt rectangle */
	int32_t trash, l;
	int16_t y1, block, w, h, i;
	int16_t *ptr;
	GRECT pt;

	UNUSED(pt);
	mice_state(M_OFF);
	obj = win->w_obj;					/* parent box       */
	trash = 0x0L;

	rc_copy(&win->w_work, (GRECT *)e_x);			/* copy the window's rectangle  */
	rc_intersect(&full, (GRECT *)e_x);		/* clip with screen */

	x = e_x[0];							/* get the real x,y,w,h */
	y = e_x[1];
	w = e_x[2];
	h = e_x[3];

	e_x[2] = e_x[0] + e_x[2];
	e_x[3] = e_x[1] + e_x[3];
	y1 = e_x[3];

	dvs_clip(1, e_x);					/* set clipping rect    */
	/* scroll up or down    */
	if (mode == SUP || mode == SDOWN)
	{
		i = size * win->w_icol;
		block = obj[i].ob_height + obj[i].ob_y;
		e_x[11] = e_x[3] = y + h - 1;	/* source's bottom  */
		e_x[1] = y + block;				/* source's top     */

		if (e_x[1] > e_x[3])			/* top > bottom     */
			e_x[1] = e_x[3];

		e_x[9] = e_x[1];
		e_x[8] = e_x[4] = e_x[0] = x;
		e_x[10] = e_x[6] = e_x[2] = x + w - 1;
		e_x[5] = y;						/* top of destination   */
		e_x[7] = e_x[3] - e_x[1] + e_x[5];

		if (mode == SUP)
		{
			win->w_rowi -= size;
			ptr = &e_x[4];
			y = e_x[5];
		} else
		{
			win->w_rowi += size;
			ptr = &e_x[0];
			y = e_x[7];
		}

		if (y1 >= ptr[1])				/* source is not off the screen */
			avro_cpyfm(S_ONLY, ptr, (FDB *)&trash, (FDB *)&trash); /* ugly */
		else
			y = win->w_work.g_y;

		objc_draw(obj, ROOT, MAX_DEPTH, x, y, w, block + 1);

		if (win->w_vvicons)
		{
			l = 1000 * win->w_rowi;
			l = l / win->w_vvicons;
		} else
		{
			l = 0;
		}
		
		wind_set(win->w_id, WF_VSLIDE, (uint16_t) (l), 0, 0, 0);

	} else								/* scroll left or right     */
	{
		block = obj[size].ob_width + obj[size].ob_x;
		e_x[10] = e_x[2] = x + w - 1;
		e_x[0] = x + block;
		if (e_x[0] > e_x[2])
			e_x[0] = e_x[2];

		e_x[8] = e_x[0];
		e_x[9] = e_x[5] = e_x[1] = y;
		e_x[7] = e_x[11] = e_x[3] = y + h - 1;
		e_x[4] = x;
		e_x[6] = e_x[2] - e_x[0] + e_x[4];

		if (mode == SRIGHT)
		{
			win->w_coli -= size;
			obj[0].ob_x += block;
			obj[0].ob_width -= block;
			x = e_x[4];
			ptr = &e_x[4];
		} else
		{
			win->w_coli += size;
			obj[0].ob_x -= block;
			obj[0].ob_width += block;
			x = e_x[6];
			ptr = &e_x[0];
		}

		if (block > w)
		{
			x = win->w_work.g_x;
			goto bb_1;
		} else
		{
			w = block + 1;
		}
		
		avro_cpyfm(S_ONLY, ptr, (FDB *)&trash, (FDB *)&trash); /* ugly */
	bb_1:
		objc_draw(obj, ROOT, MAX_DEPTH, x, y, w, h);
		if (win->w_hvicons)
		{
			l = 1000 * win->w_coli;
			l /= win->w_hvicons;
		} else
		{
			l = 0;
		}
		
		wind_set(win->w_id, WF_HSLIDE, (uint16_t) l, 0, 0, 0);

	}

	mice_state(M_ON);
}


/*
 * Adjust everything inside the window because of window's
 * size has been changed or object type is changed.
 * Adjust means we have to do recalculation.
 * Otherwise, pin down the left upper corner and just
 * adjust the scroll bars.
 */
/* 306de: 00e34fbc */
VOID view_adjust(P(WINDOW *) win)
PP(register WINDOW *win;)
{
	register OBJECT *obj;
	register uint16_t x, col;
	uint16_t row;							/* must be unsigned */
	int32_t l;
	uint16_t i;

	obj = win->w_obj;

	/***  Figure out the vertical slide bar	***/

	x = win->w_items / win->w_icol;		/* number of row needed */

	if (win->w_items % win->w_icol)
		x++;

	if (!x)								/* at least one row    */
		x = 1;

	row = win->w_work.g_h / fobj.g_h;		/* current visible number of row */

	win->w_xrow = row;

	win->w_vvicons = x < row ? 0 : x - row;

	if (x > row)						/* if there is more need to be shown */
	{
		l = 1000 * row;
		i = l / x;
	} else
	{
		i = 1000;
	}
	
	wind_set(win->w_id, WF_VSLSIZE, i, 0, 0, 0);

	if (win->w_rowi > win->w_vvicons)
		win->w_rowi = win->w_vvicons;

	if (!win->w_vvicons)
	{
		i = 0;
	} else
	{
		l = 1000 * win->w_rowi;
		i = l / win->w_vvicons;
	}

	wind_set(win->w_id, WF_VSLIDE, i, 0, 0, 0);

	/***    Figure the horizontal slide bar size	***/

	if (win->w_icol <= 1)
		col = 1;
	else
		col = win->w_work.g_w / fobj.g_w;	/* current visible column   */

	if (!col)
		col = 1;

	x = win->w_items > win->w_icol ? win->w_icol : win->w_items;

	win->w_xcol = col;					/* current visible column   */

	win->w_hvicons = x <= col ? 0 : x - col;

	if (!x)
	{
		i = 1000;
	} else
	{
		l = 1000 * col;
		i = l / x;
	}

	wind_set(win->w_id, WF_HSLSIZE, i, 0, 0, 0);

	if (win->w_coli > win->w_hvicons)
		win->w_coli = win->w_hvicons;

	obj[0].ob_x = win->w_work.g_x;
	obj[0].ob_x -= (win->w_coli * fobj.g_w);
	obj[0].ob_width = win->w_work.g_w;
	obj[0].ob_width += (win->w_coli * fobj.g_w);

	if (!win->w_hvicons)				/* fits all the file    */
	{
		i = 0;
	} else
	{
		l = 1000 * win->w_coli;
		i = l / win->w_hvicons;
	}

	wind_set(win->w_id, WF_HSLIDE, i, 0, 0, 0);
	set_newview(win->w_rowi * win->w_icol, win);
}


/*
 * Change the viewing mode, form text to icon or vice verse
 */
/* 306de: 00e35202 */
VOID sort_show(P(int16_t) mode, P(BOOLEAN) view)
PP(int16_t mode;)
PP(BOOLEAN view;)
{
	register WINDOW *win;

	desk_wait(TRUE);

	win = w_gfirst();

	while (win)
	{
		if (view)
		{
			view_fixmode(win);			/* change icon or text mode */
			view_adjust(win);			/* view adjust will set new view ptr */
			do_redraw(win->w_id, &full, 0);
		} else
		{
			sort_file(win, mode);
			up_1(win);
		}

		win = w_gnext();
	}

	desk_wait(FALSE);
}


/*
 * The viewing mode has been changed
 * Fix up the icon position according to largest window size
 * Call this routine before calling view_adjust!!!!!!!
 * It used the s_stofit flag to set the horizontal number of icon!
 */
/* 306de: 00e35272 */
VOID view_fixmode(P(WINDOW *) win)
PP(WINDOW *win;)
{
	register int16_t x, y, w, i, offx;
	int16_t h, limitw, limith;
	int16_t type, offy;
	int16_t len, col, row, doitc;
#if COLORICON_SUPPORT
	CICONBLK *iblk;
#else
	ICONBLK *iblk;
#endif
	char *text;
	register OBJECT *obj;
	OBJECT *obj1;
	TEDINFO *ted;

	obj = win->w_obj;

	if (s_view == S_ICON)				/* icon mode    */
	{
		w = dicon.g_w;
		h = dicon.g_h;					/* Change also at deskinf.c */
		offx = gl_hchar == 8 ? gl_wchar / 2 : gl_wchar;
		offy = gl_hchar / 2;
#if COLORICON_SUPPORT
		type = G_CICON;
		iblk = win->w_ciblk;
#else
		type = G_ICON;
		iblk = win->w_iblk;
#endif
#if !BINEXACT
		ted = 0; /* quiet compiler */
		text = 0;
		len = 0;
#endif
	} else								/* text mode    */
	{
		obj1 = get_tree(TEXTLINES);
		len = strlen(((TEDINFO *) (obj1[TSTYLE].ob_spec))->te_ptext);
		w = (len - 1) * gl_wchar;
		h = gl_hchar;
		offx = 3 * gl_wchar;
		offy = 2;
		type = G_TEXT;
		text = win->w_text;
		ted = win->w_ted;
#if !BINEXACT
		iblk = 0; /* quiet compiler */
#endif
	}

	fobj.g_w = w + offx;
	fobj.g_h = h + offy;

	if (s_stofit)						/* size to fit  */
		limitw = win->w_work.g_w;
	else
		limitw = full.g_w - gl_wbox;

	limith = full.g_h - (3 * gl_hbox);

	x = offx;
	y = offy;

	col = 0;							/* min one col one row  */
	row = 0;

	doitc = TRUE;

	for (i = 1; i < (win->w_maxicons + 1); i++)
	{
		obj[i].ob_type = type;
		obj[i].ob_x = x;
		obj[i].ob_y = y;
		obj[i].ob_width = w;
		obj[i].ob_height = h;
		obj[i].ob_next = i + 1;

		if (s_view == S_ICON)			/* icon mode    */
		{
			obj[i].ob_spec = (intptr_t)(iblk++);
		} else
		{
			obj[i].ob_spec = (intptr_t)ted;
			ted->te_ptext = text;
			text += len;
			ted++;
		}

		col++;							/* one icon added   */
		x = x + w + offx;				/* point to next x pos  */

		if ((x + w) > limitw)			/* off the edge?    */
		{
			x = offx;					/* yes          */
			y = y + h + offy;
			row++;
			if (doitc)
			{
				win->w_icol = col;		/* maximum col of icons */
				doitc = FALSE;
			}
		}

		if (y >= limith)				/* At the bottom    */
			break;
	}

	win->w_irow = row;					/* maximum row of icons */

	/* Free up the rest of the icons */

	obj[0].ob_tail = i;
	obj[i].ob_next = 0;

	win->w_vicons = i;					/* maximum visible icons */
}


/*
 * Make this window to be the top one
 */
/* 306de: 00e354c4 */
VOID make_top(P(WINDOW *) win)
PP(register WINDOW *win;)
{
	register WINDOW *winptr;

	if (win)
	{
#if 0
		set_dir(win->w_path);
#endif
		if (winhead == win)				/* already on top   */
			return;

		winptr = winhead;

		while (winptr)
		{
			if (winptr->w_next == win)
			{
				winptr->w_next = win->w_next;
				win->w_next = winhead;
				winhead = win;
				break;
			}

			winptr = winptr->w_next;
		}
	}
}


/*
 * Allocate a window object
 */
/* 306de: 00e35508 */
BOOLEAN alloc_obj(P(int16_t) mode, P(WINDOW *) win)
PP(int16_t mode;)
PP(register WINDOW *win;)
{
	register int16_t i, j;
	register uint16_t item;
	register OBJECT *obj1, *obj2;
	int16_t w, h, len;
#if COLORICON_SUPPORT
	CICONBLK *iblk;
#else
	ICONBLK *iblk;
#endif
	TEDINFO *ted;
	uint16_t k;
	int32_t size;

	UNUSED(w);
	UNUSED(h);
	i = full.g_w / dicon.g_w;
	if (full.g_w % dicon.g_w)
		i++;

	j = full.g_h / dicon.g_h;
	if (full.g_h % dicon.g_h)
		j++;

	item = i * j;

	obj2 = get_tree(TEXTLINES);

	i = full.g_w / obj2[TSTYLE].ob_width;
	if (full.g_w % obj2[TSTYLE].ob_width)
		i++;

	j = full.g_h / obj2[TSTYLE].ob_height;
	if (full.g_h % obj2[TSTYLE].ob_height)
		j++;

	k = i * j;

	if (k > item)
		item = k;

	len = strlen(((TEDINFO *) (obj2[TSTYLE].ob_spec))->te_ptext);

	size = 0;
	size += (uint16_t) ((item + 1) * sizeof(OBJECT));
	size += (uint16_t) (item * len);
	size += (uint16_t) (item * sizeof(TEDINFO));
	size += (uint16_t) (item * sizeof(*iblk));

	if ((win->w_obj = (OBJECT *)Malloc(size)))
	{
		obj1 = win->w_obj;

		LBCOPY(obj1++, &blank, sizeof(OBJECT));

		for (i = 0; i < item; i++)		/* copy object structure */
			LBCOPY(obj1++, &iconaddr[ICON1], sizeof(OBJECT));

#if COLORICON_SUPPORT
		win->w_ciblk = iblk = (CICONBLK *)obj1;
#else
		win->w_iblk = iblk = (ICONBLK *)obj1;
#endif

		/* start of the iconblk area    */
		for (i = 0; i < item; i++)		/* copy icon structure      */
			LBCOPY(iblk++, (VOIDPTR)iconaddr[ICON1].ob_spec, sizeof(*iblk));

		win->w_ted = ted = (TEDINFO *)iblk;

		for (i = 0; i < item; i++)
			LBCOPY(ted++, (VOIDPTR)obj2[TSTYLE].ob_spec, sizeof(TEDINFO));

		win->w_text = (char *)ted;				/* set the text pointer     */

		win->w_maxicons = item;
		return TRUE;
	} else
	{
		win->w_maxicons = 0;
		do1_alert(NOWINMEM);
		return FALSE;
	}
}


/*
 * Initalize all the windows
 */
/* 306de: 00e356f6 */
VOID ini_windows(NOTHING)
{
	register int16_t i;
	register WINDOW *win;

	d_level = 4;

	win = &winpd[0];

	for (i = 0; i < MAXWIN; i++, win++)
	{
#if (TOSVERSION >= 0x400)
		win->w_obj = NULL;
#endif
		win->w_path = NULL;				/* window path  */
		win->w_buf = NULL;
		win->w_free = FALSE;			/* window is not free yet   */
		win->w_level = 0;
		win->w_id = -1;					/* window is not open       */
		win->w_rowi = 0;
		win->w_coli = 0;
		win->w_memory = NULL;			/* clean up file memory     */

		/* Put in default sizes, in case users have old inf file */

		rc_copy(&w_sizes, &win->w_work);
		rc_copy(&w_sizes, &win->w_sizes);
	}

	path_alloc(4);						/* allocate path memory */

	win = &winpd[0];

	for (i = 0; i < MAXWIN; i++, win++)
	{
#if TOSVERSION < 0x400
		if (!alloc_obj(0, win))			/* in case memory alloc failed   */
		{
			do1_alert(NOWINMEM);
			break;
		}
#endif
		win->w_next = &winpd[i + 1];
		win->w_free = TRUE;				/* this window is available */
	}

	winpd[i - 1].w_next = NULL;
	winhead = &winpd[0];
}


#if TOSVERSION >= 0x400
VOID ret_win(P(WINDOW *) win)
PP(WINDOW *win;)
{
	if (win->w_obj)
	{
		Mfree(win->w_obj);
		win->w_obj = NULL;
	}

	win->w_free = TRUE;
}
#endif


/* 306de: 00e357e6 */
WINDOW *alloc_win(NOTHING)
{
	register WINDOW *win;

	win = winhead;

	while (win)
	{
		if (win->w_free)
		{
#if TOSVERSION >= 0x400
			if (!alloc_obj(0, win))		/* allocate objects */
			{
				do1_alert(NOWINMEM);
				break;
			}
#endif
			win->w_free = FALSE;
			return win;
		}

		win = win->w_next;
	}

	return NULL;
}


/*
 * Get the window data structure address according to handle
 */
/* 306de: 00e35816 */
WINDOW *get_win(P(int16_t) handle)
PP(int16_t handle;)
{
	register WINDOW *win;

	win = winhead;

	while (win)
	{
		if (win->w_id == handle)
			break;

		win = win->w_next;
	}

	return win;
}


/*
 * Open window
 */
/* 306de: 00e35844 */
VOID open_window(P(int16_t) handle)
PP(int16_t handle;)
{
	register WINDOW *win;

	win = get_win(handle);				/* window's size    */
	/* open it */

	win->w_obj->ob_spec = windspec;		/* set window's color   */

	do_xyfix(&win->w_sizes);

	wind_open(handle, win->w_sizes.g_x, win->w_sizes.g_y, win->w_sizes.g_w, win->w_sizes.g_h);

	wind_get(handle, WF_CURRXYWH, &win->w_sizes.g_x, &win->w_sizes.g_y, &win->w_sizes.g_w, &win->w_sizes.g_h);

	/* get the working area and set it to object */
	wind_get(handle, WF_WORKXYWH, &win->w_work.g_x, &win->w_work.g_y, &win->w_work.g_w, &win->w_work.g_h);

	rc_copy(&win->w_work, (GRECT *)&win->w_obj->ob_x);
	make_top(win);
}


/*
 * Create Window of its full size
 */
/* 306de: 00e3591e */
int16_t create_window(NOTHING)
{
	register WINDOW *win;
	int16_t handle;

	/* Do we have any window pd ?   */
	if ((win = alloc_win()) != NULL)
	{
		handle = wind_create(ALLITEMS, win->w_sizes.g_x, win->w_sizes.g_y, win->w_sizes.g_w, win->w_sizes.g_h);

		if (handle < 0)					/* release windows  */
		{
#if TOSVERSION >= 0x400
			ret_win(win);
#else
			win->w_free = TRUE;
#endif
		} else
		{
			win->w_id = handle;
			return handle;
		}
	}
	return -1;
}


/*
 * Get the top window
 */
/* 306de: 00e35976 */
WINDOW *get_top(NOTHING)
{
	return w_gfirst();

#if UNLINKED
	int16_t handle, ret;
	WINDOW *win;

	/* find out who is on top */
	wind_get(0, WF_TOP, &handle, &ret, &ret, &ret);
	if (handle > 0)
	{
		if (win = get_win(handle))
			return win;
	}
	return NULL;
#endif
}


/*
 * Close and delete a window
 */
/* 306de: 00e35982 */
VOID close_window(P(int16_t) handle, P(BOOLEAN) closeit)
PP(int16_t handle;)
PP(BOOLEAN closeit;)
{
	register WINDOW *win;

	if (closeit)						/* close window     */
		wind_close(handle);

	/* Delete window and assume it is closed */
	/* Free up the file memory */

	if ((win = get_win(handle)) != NULL)
	{
		win->w_id = -1;
		win->w_free = TRUE;
		if (win->w_memory)
		{
			Mfree(win->w_memory);
			win->w_memory = NULL;
		}
		wind_delete(handle);

		if (closeit)
		{
			while (win)
			{
				if (win->w_id != -1)
				{
					make_top(win);
					break;
				} else
				{
					win = win->w_next;
				}
			}
		}
	}
}



/*
 * Free and close all the windows and free the objects
 */
/* 306de: 00e35a02 */
VOID free_windows(NOTHING)
{
	register WINDOW *win;
	int16_t i;

	for (i = 0; i < MAXWIN; i++)
	{
		win = &winpd[i];

		if (win->w_obj)
		{
			Mfree(win->w_obj);
			win->w_obj = NULL;
		}

		/* don't close window, do it at the sh_main */

		if (win->w_id != -1)
			close_window(win->w_id, FALSE);
	}

	free_path();
}


/*
 * This is my own redraw routine
 */
/* 306de: 00e35a68 */
VOID do_redraw(P(int16_t) handle, P(GRECT *) pc, P(int16_t) which)
PP(register int16_t handle;)
PP(GRECT *pc;)
PP(int16_t which;)
{
	GRECT pt;
	register OBJECT *obj;
	WINDOW *win;
	GRECT px;

	rc_copy(pc, &px);

	wind_get(handle, WF_FIRSTXYWH, &pt.g_x, &pt.g_y, &pt.g_w, &pt.g_h);

	if (handle > 0)
	{
		win = get_win(handle);
		obj = win->w_obj;
	} else
	{
		obj = background;
	}
	
	if (which)
	{
		objc_offset(obj, which, &px.g_x, &px.g_y);
		px.g_w = obj[which].ob_width;
		px.g_h = obj[which].ob_height;
	}

	while (pt.g_w && pt.g_h)
	{
		if (rc_intersect(&px, &pt))
		{
			if (rc_intersect(&full, &pt))
				objc_draw(obj, ROOT, MAX_DEPTH, pt.g_x, pt.g_y, pt.g_w, pt.g_h);
		}
		wind_get(handle, WF_NEXTXYWH, &pt.g_x, &pt.g_y, &pt.g_w, &pt.g_h);
	}
}


/* 306de: 00e35ba0 */
VOID do_xyfix(P(GRECT *) pc)
PP(register GRECT *pc;)
{
	GRECT rect;

	rc_copy(&full, &rect);

	pc->g_w = min(pc->g_w, rect.g_w);
	pc->g_h = min(pc->g_h, rect.g_h);

	if (pc->g_x >= (rect.g_x + rect.g_w))
		pc->g_x = rect.g_x + rect.g_w;

	if (pc->g_x < rect.g_x)
		pc->g_x = rect.g_x;

	if (pc->g_y >= (rect.g_y + rect.g_h))
		pc->g_y = rect.g_y + rect.g_h;

	if (pc->g_y < rect.g_y)
		pc->g_y = rect.g_y;

	rect.g_x = pc->g_x;

	pc->g_x = (rect.g_x & 0x000f);

	if (pc->g_x < 8)
		pc->g_x = rect.g_x & 0xfff0;
	else
		pc->g_x = (rect.g_x & 0xfff0) + 16;

	pc->g_y = max(pc->g_y, rect.g_y);
}
