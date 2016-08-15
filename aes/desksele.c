/*	DESKSELE.C		10/9/89			Derek Mui	*/
/*	Change all the iconblk to ciconblk	7/11/92	D.Mui		*/

/************************************************************************/
/*	New Desktop for Atari ST/TT Computer				*/
/*	Atari Corp							*/
/*	Copyright 1989,1990 	All Rights Reserved			*/
/************************************************************************/

#include <portab.h>
#include <mobdefs.h>
#include <defines.h>
#include <window.h>
#include <gemdefs.h>
#include <deskusa.h>
#include <osbind.h>
#include <extern.h>

EXTERN WINDOW *w_gfirst();

EXTERN WINDOW *w_gnext();

EXTERN BYTE *put_name();

WORD x_status;							/* for x_select         */

WORD x_index;							/* For x_next and x_first   */

WORD x_type;							/* ditto            */

WINDOW *x_win;							/* diito            */

WORD x_cur;								/* x_del            */

WORD d_dir;								/* count how many folders are selected  */

				/* inside the window            */

EXTERN WORD o_type;

EXTERN WORD o_item;

EXTERN WORD o_status;

EXTERN WINDOW *o_win;

EXTERN DIR *get_dir();


/*	Turn off the current selected object	*/

x_del()
{
	DIR *dir;

	if (x_type == WINICON)
	{
		dir = x_win->w_memory;
		dir[x_cur].d_state = NORMAL;
	} else
		background[x_cur].ob_state = NORMAL;
}


/*	My own object find	*/

WORD m_objfind(obj, mx, my, mode)
REG OBJECT *obj;

REG WORD mx,
	my;

WORD mode;								/* TRUE == window mode  */
{
	REG i,
	 limit;

	WORD status,
	 offx,
	 offy;

	WORD x1,
	 y1;

	CICONBLK *iblk;

	GRECT pt;

	status = -1;

	offx = obj[0].ob_x;
	offy = obj[0].ob_y;
	limit = obj[0].ob_tail;

	for (i = 1; i <= limit; i++)
	{
		if (obj[i].ob_flags & HIDETREE)
			continue;

		if (obj[i].ob_type == G_CICON)	/* icon ?   */
		{
			x1 = obj[i].ob_x + offx;
			y1 = obj[i].ob_y + offy;

			iblk = (CICONBLK *) (obj[i].ob_spec);
			rc_copy(&iblk->monoblk.ib_xicon, &pt);
			pt.x += x1;
			pt.y += y1;

			if (!inside(mx, my, &pt))
			{
				rc_copy(&iblk->monoblk.ib_xtext, &pt);
				pt.x += x1;
				pt.y += y1;
				if (inside(mx, my, &pt))
					status = i;
			} else
				status = i;
		} else							/* text mode    */
		{
			rc_copy(&obj[i].ob_x, &pt);
			pt.x += offx;
			pt.y += offy;
			if (inside(mx, my, &pt))
				status = i;
		}

		if ((status != -1) && (mode))
			break;

	}									/* for */

	if (status == -1)					/* if fails then try the root   */
	{
		if (inside(mx, my, &obj[0].ob_x))
			status = 0;
	}

	return (status);
}


/*	Find out which object is selected based on the object structures */
/*	Type can only be DESKICON or WINICON				*/

WORD o_select()
{
	REG WINDOW *win;

	REG OBJECT *obj;

	REG WORD i,
	 j;

	o_type = WINICON;					/* Try the window   */

	win = w_gfirst();

	while (win)
	{
		obj = win->w_obj;
		j = win->w_obj->ob_tail;

		for (i = 1; i <= j; i++)
		{
			if ((obj[i].ob_state & SELECTED) && (obj[i].ob_flags != HIDETREE))
			{
				if (in_parent(obj, i))	/* in side parent's box? */
				{
					o_item = i;
					o_win = win;
					o_status = TRUE;
					return (o_status = TRUE);
				}
			}
		}

		win = w_gnext();
	}

	o_type = DESKICON;					/* Now try desktop */
	o_win = (WINDOW *) 0;
	return (o_status = i_next(1, background, &o_item));
}


/*	Find out if anything is selected		*/

WORD x_select()
{
	REG WINDOW *win;

	REG DIR *dir;

	REG WORD i;

	WORD j;

	x_win = (WINDOW *) 0;
	x_type = DESKICON;

	win = w_gfirst();

	while (win)
	{
		dir = win->w_memory;
		for (i = 0; i < win->w_items; i++)
		{
			if (dir[i].d_state & SELECTED)
			{
				x_win = win;
				x_type = WINICON;
				return (x_status = TRUE);
			}
		}
		win = w_gnext();
	}

	return (x_status = i_next(1, background, &j));
}


/*	Extended dir structure search next	*/

x_next(name, type)
BYTE **name;

WORD *type;
{
	REG DIR *dir;

	REG WORD i;

	WORD limit;

	BYTE *str;

	if (x_type == WINICON)
	{
		dir = x_win->w_memory;
		limit = x_win->w_items;

		for (i = x_index; i < limit; i++)
		{
			if (dir[i].d_state & SELECTED)
			{
				str = put_name(x_win, &dir[i].d_name[0]);
				if (dir[i].d_att & SUBDIR)
				{
					strcat(wilds, str);
					d_dir++;
					*type = SUBDIR;
				} else
					*type = x_type;

				*name = str;
				x_cur = i;
				x_index = i + 1;
				return (TRUE);
			}
		}
		return (FALSE);
	} else
		return (d_sdesk(name, type));
}

/*	Object oriented search first				*/
/*	Returns icon type, WINICON, XFILE, DISK, SUBDIR		*/
/*	Extended dir search first				*/

WORD x_first(name, type)
BYTE **name;

WORD *type;
{
	REG WINDOW *win;

	REG WORD i,
	 j;

	DIR *dir;

	d_dir = 0;

	x_type = DESKICON;					/* try desktop first    */
	x_win = (WINDOW *) 0;

	if (i_next(1, background, &x_index))
		return (x_next(name, type));

	win = w_gfirst();

	while (win)							/* inside a window? */
	{
		dir = win->w_memory;
		j = win->w_items;

		for (i = 0; i < j; i++)
		{
			if (dir[i].d_state & SELECTED)
			{
				x_index = i;
				x_type = WINICON;
				x_win = win;
				return (x_next(name, type));
			}
		}

		win = w_gnext();
	}

	return (FALSE);
}


/*	Find out what the user has clicked on based on the mx and my	*/

WORD i_find(mx, my, winout, item, type)
REG WORD mx,
	my;

WINDOW **winout;

WORD *item;

WORD *type;
{
	REG WINDOW *win;

	REG WORD which;

	win = w_gfirst();

	while (win)
	{									/* inside a window boundary */
		if (inside(mx, my, &win->w_sizes))
		{
			*winout = win;
			*type = WINICON;
			*item = 0;
			/* inside the work area     */
			if (inside(mx, my, &win->w_work))
			{
				if ((which = m_objfind(win->w_obj, mx, my, TRUE)) != -1)
					*item = which;
			}

			return (TRUE);
		}

		win = w_gnext();
	}

	/* try desktop      */
	if ((which = m_objfind(background, mx, my, FALSE)) != -1)
	{
		*item = which;
		*type = DESKICON;
		*winout = (WINDOW *) 0;
		return (TRUE);
	}

	return (FALSE);
}


/*	Search for next selected OBJECT		*/

WORD i_next(start, obj, itemout)
WORD start;

REG OBJECT *obj;

WORD *itemout;
{
	REG WORD limit,
	 i;

	limit = obj->ob_tail;

	for (i = start; i <= limit; i++)
	{
		if ((!(obj[i].ob_flags & HIDETREE)) && (obj[i].ob_state & SELECTED))
		{
			*itemout = i;
			return (TRUE);
		}
	}

	return (FALSE);
}

/*	Search the selected desktop object	*/
/*	Used by x_next only			*/

d_sdesk(name, type)
BYTE **name;

WORD *type;
{
	WORD temp,
	 i;

	REG IDTYPE *itype;

	while (i_next(x_index, background, &x_index))
	{
		itype = &backid[x_index];
		temp = itype->i_type;

		if (temp == DISK)
		{
			strcpy(wildext, path1);
			path1[0] = itype->i_cicon.monoblk.ib_char[1];
			*name = path1;
			*type = temp;
			goto xd_1;
		} else if (itype->i_path)
		{
			*name = itype->i_path;
			*type = temp;
			if (temp == XDIR)
				d_dir++;

		  xd_1:x_cur = x_index++;
			return (TRUE);
		} else
			x_index++;
	}

	return (FALSE);
}
