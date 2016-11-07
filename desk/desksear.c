/*      DESKSEAR.C              1/3/90 - 1/16/90        D.Mui           */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"

VOID pos_item PROTO((WINDOW *win, int16_t newi));
BOOLEAN rec_sea PROTO((char *filename));
BOOLEAN display PROTO((char *filename));
VOID mass_string PROTO((char *str1));



/*
 * Position an item within a window
 */
VOID pos_item(P(WINDOW *) win, P(int16_t) newi)
PP(register WINDOW *win;)
PP(int16_t newi;)
{
	register int16_t i, j, k;
	int32_t l;

	/******   adjust the vertical bar   ******/

	j = newi / win->w_icol;

	i = win->w_items / win->w_icol;		/* maximum number of row    */
	if (win->w_items % win->w_icol)
		i++;

	if (j >= win->w_xrow)
		j = j - win->w_xrow + 1;
	else
		j = 0;

	win->w_rowi = j;

	win->w_srtitem = j * win->w_icol;

	if (i > win->w_xrow)
	{
		l = 1000 * j;
		l = l / (i - win->w_xrow);
	} else
		l = 1000;

	win->w_obj->ob_y = win->w_work.g_y;
	wind_set(win->w_id, WF_VSLIDE, (uint16_t) l, 0, 0, 0);

	/*******  adjust the horizontal bar  ********/

	win->w_obj->ob_x = win->w_work.g_x;
	win->w_obj->ob_width = win->w_work.g_w;

	i = (newi % win->w_icol) + 1;		/* column index     */
	if (i > win->w_xcol)				/* bigger than one frame */
		j = i - win->w_xcol;			/* move left        */
	else
		j = 0;

	win->w_coli = j;					/* column index     */

	k = j * fobj.g_w;					/* move the object if necessary */
	win->w_obj->ob_x -= k;
	win->w_obj->ob_width += k;

	if (win->w_hvicons)
	{
		l = 1000 * j;
		l = l / win->w_hvicons;
	} else
	{
		l = 1000;
	}
	
	wind_set(win->w_id, WF_HSLIDE, (uint16_t) l, 0, 0, 0);
	up_2(win);
}



/*
 * Search a file
 */
VOID sea_file(P(char *)filename)
PP(char *filename;)
{
	register WINDOW *win;
	register DIR *dir;
	register int16_t i;
	const char *str;
	BOOLEAN status;
	int16_t type, change, newi;
	char buffer[16];

	desk_wait(TRUE);

	strcpy(buffer, filename);
	mass_string(filename);

	change = FALSE;

	if ((status = x_first(&str, &type)))
	{
		f_level = 0;
		f_rename = FALSE;

		while (status)
		{
			if (type == DISK || type == SUBDIR || type == XDIR)
			{
				if (*str == CHAR_FOR_CARTRIDGE)
				{
					do1_alert(NOSEA);
				} else
				{
					set_dir(str);		/* set the current directory    */
					if (rec_sea(filename))
						goto s_ex1;
				}
			}
			status = x_next(&str, &type);
		}

		if (f_rename)					/* if there is a window opened, then the */
		{								/* search must be succeed        */
			do1_alert(NOMORE);
			goto s_ex1;
		}
	} else
	{
		if ((win = w_gfirst()) != NULL)
		{
			if (win->w_path[0] == CHAR_FOR_CARTRIDGE)
			{
				do1_alert(NOSEA);
				goto s_ex2;
			}

			dir = win->w_memory;
			for (i = 0; i < win->w_items; i++)
			{
				if (wildcmp(filename, dir[i].d_name))
				{
					dir[i].d_state = SELECTED;
					if (!change)
						newi = i;

					change = TRUE;
				}
			}

			if (change)
			{
				pos_item(win, newi);
				goto s_ex2;
			}
		}								/* if window */
	}									/* else    */

	fill_string(buffer, FNOTFIND);
	goto s_ex3;

  s_ex1:								/* if not select from top window */
	if ((x_type == WINICON) && (x_status))
		clr_xwin(x_win, TRUE);
  s_ex2:
	clr_dicons();

  s_ex3:
	desk_wait(FALSE);
}


/*
 * Recursive search of a file
 */
BOOLEAN rec_sea(P(char *)filename)
PP(register char *filename;)
{
	DTA *ldtabuf;
	DTA *savedta;
	register int16_t status, ret;

	if (!(ldtabuf = (DTA *)Malloc((int32_t) sizeof(DTA))))
	{
		do1_alert(FCNOMEM);
		return (TRUE);
	}

	savedta = (DTA *)Fgetdta();			/* save the dta buffer  */
	Fsetdta(ldtabuf);					/* set the new dta  */

	status = FALSE;						/* not found        */

	desk_wait(TRUE);
	/* look for matching    */
	ret = Fsfirst(filename, 0x31);

	while (!ret)
	{									/* matched      */
		if (wildcmp(filename, ldtabuf->dirfile.d_name))
		{
			if (display(filename))
			{
				if (do1_alert(SNEXT) == 1)
					goto s_more;
			}

			status = TRUE;
			goto s_exit;
		}

		ret = Fsnext();
	}

	/* Look for folder  */
  s_more:
	desk_wait(TRUE);

	Fsetdta(ldtabuf);					/* set the new dta  */
	ret = Fsfirst(getall, 0x31);

	while (!ret)
	{
		if ((ldtabuf->dirfile.d_att & SUBDIR) && (ldtabuf->dirfile.d_name[0] != '.'))
		{
			path2[0] = '.';
			path2[1] = '\\';
			strcpy(&path2[2], ldtabuf->dirfile.d_name);
			Dsetpath(path2);
			f_level++;
			if (f_level > COPYMAXDEPTH)
			{
				do1_alert(STFO8DEE);
				status = TRUE;
				break;
			}

			status = rec_sea(filename);
			f_level--;

			if (!status)				/* if not found then back up one path */
				Dsetpath("..");
			else
				break;					/* quit */

		}
		/* if SUBDIR   */
		ret = Fsnext();
	}									/* while       */

  s_exit:
	Mfree(ldtabuf);
	Fsetdta(savedta);
	return status;
}


/*
 * Display a window and highlight the items
 */
BOOLEAN display(P(char *)filename)
PP(char *filename;)
{
	int32_t size;
	register char *addr;
	register WINDOW *win;
	register DIR *dir;
	register int16_t i;
	int16_t newi;
	BOOLEAN ret;
	int16_t first;

	size = Malloc(0xFFFFFFFFL);
	if (size)
	{
		addr = (char *)Malloc(size);
		Dgetpath(addr, 0);
		i = strlen(addr) + 20;
		Mfree(addr);					/* free the memory      */
		addr = (char *)Malloc((int32_t) i);
		addr[0] = Dgetdrv() + 'A';		/* get the default drive    */
		addr[1] = ':';
		Dgetpath(&addr[2], 0);
		strcat(addr, bckslsh);
		strcat(addr, getall);

#if !BINEXACT
		newi = 0; /* quiet compiler */
#endif
		if (f_rename)
		{
			if (c_path_alloc(addr))
			{
				win = w_gfirst();
				strcpy(win->w_path, addr);
				Mfree(addr);
				first = TRUE;

				if (open_subdir(win, 0, FALSE, FALSE, FALSE))
				{
					dir = win->w_memory;
					for (i = 0; i < win->w_items; i++)
					{
						if (wildcmp(filename, dir[i].d_name))
						{
							if (first)
							{
								newi = i;
								first = FALSE;
							}
							dir[i].d_state = SELECTED;
						}
					}
				} else
					return (FALSE);
			} else
			{
				Mfree(addr);
				return (FALSE);
			}
		} else
		{
			f_rename = TRUE;
			if ((win = alloc_win()) != NULL)		/* check for window */
			{
#if 0
				win->w_sizes.x = full.x;
				win->w_sizes.y = full.y;
				win->w_sizes.w = full.w;
				win->w_sizes.h = full.h / 3;
				rc_copy( &win->w_sizes, &win->w_work );
#endif

				win->w_free = TRUE;		/* return window    */
				ret = open_disk(0, addr, FALSE);
			} else
			{
				ret = FALSE;
			}
			
			Mfree(addr);

			if (!ret)
				return (FALSE);

			win = w_gfirst();
			dir = win->w_memory;
			first = TRUE;

			for (i = 0; i < win->w_items; i++)
			{
				if (wildcmp(filename, dir[i].d_name))
				{
					if (first)
					{
						newi = i;
						first = FALSE;
					}
					dir[i].d_state = SELECTED;
				}
			}
			wait_msg();
		}
		pos_item(win, newi);
		return (TRUE);

	}
	/* No memory   */
	return (FALSE);
}


/*
 * Put in wild card in the name string
 */
VOID mass_string(P(char *)str1)
PP(char *str1;)
{
	register int16_t i;
	char buffer[6];
	i = 0;

	while (*str1)
	{
		if (*str1 == '.')
		{
			if (i < 8)
			{
				*str1++ = '*';
				strcpy(buffer, str1);
			} else
			{
				strcpy(buffer, str1 + 1);
			}
			
			i = strlen(buffer);
			if (i < 3)
				buffer[i++] = '*';

			buffer[i] = 0;
			*str1++ = '.';
			strcpy(str1, buffer);
			return;
		} else
			i++;

		str1++;
	}

	strcpy(str1, getall);				/*  *.* */
}
