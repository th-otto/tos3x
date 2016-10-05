/*	DESKFILE.C		3/17/89 - 6/15/89	Derek Mui	*/
/*	Change read_file	9/14/89			D.Mui		*/
/*	Change the way it gets icon	9/15/89		D.Mui		*/
/*	Fix at sort by date		6/28/90		D.Mui		*/
/*	Fix at read_file to allow lower case 7/18/90	D.Mui		*/
/*	Fix at read_file for calculating volume value	4/29/91	D.Mui	*/
/*	Fix at read_file for calculating volume again	8/13/91	D.Mui	*/
/*	Change all the iconblk to ciconblk	7/11/92	D.Mui		*/

/************************************************************************/
/*	New Desktop for Atari ST/TT Computer				*/
/*	Atari Corp							*/
/*	Copyright 1989,1990 	All Rights Reserved			*/
/************************************************************************/

#include <portab.h>
#include <mobdefs.h>
#include <defines.h>
#include <osbind.h>
#include <window.h>
#include <gemdefs.h>
#include <deskusa.h>
#include <error.h>
#include <extern.h>

extern char *get_fstring();

extern APP *app_icon();

extern char *r_slash();

extern char *scasb();

extern OBJECT *get_tree();

extern WINDOW *w_gfirst();

extern char *bldstring();

extern char *strcpy();

#define CTLC 	3

int16_t pri_str(where, ptr)
int16_t where;

char *ptr;
{
	int32_t c;

	int16_t ch;

	while (*ptr)
	{
	  pr_1:if (!Bconout(where, *ptr++))/* device not present ?   */
		{
			if (do_alert(2, NOOUTPUT) == 1)
				goto pr_1;
			else
				return (FALSE);
		}

		if (Bconstat(2))
		{
			ch = (int16_t) (c = Bconin(2));
			if (ch == CTLC || ch == 'q' || ch == 'Q' || (c & 0x00ff0000L) == 0x00610000)
				return (FALSE);
		}
	}
	return (TRUE);
}


/*	Print a window directory	*/

pri_win()
{
	register WINDOW *win;

	register int16_t serial;

	int16_t max,
	 i,
	 type;

	DIR *dir;

	char buffer[80];

	char buf1[10];

	int32_t sizes;

	if (win = w_gfirst())
	{
		desk_wait(TRUE);

		serial = (trp14(0x21, 0xFFFF) & 0x10) ? TRUE : FALSE;
		dir = win->w_memory;
		max = win->w_items;
		sizes = 0;

		if (!pri_str(serial, Nextline))
			goto pri_end;

		if (!pri_str(serial, win->w_path))
			goto pri_end;

		if (!pri_str(serial, "\012\012\015"))
			goto pri_end;

		for (i = 0; i < max; i++)
		{
			sizes += dir[i].d_size;
			strcpy(Nextline, bldstring(&dir[i], buffer));
			if (!pri_str(serial, buffer))
				goto pri_end;
		}

		*((int32_t *) & buf1[0]) = sizes;
		*((int16_t *) & buf1[4]) = (int16_t) win->w_items;

		merge_str(strcpy(Nextline, buffer) - 1, get_fstring((win->w_items == 1) ? ISTR : ISTRS), buf1);

		strcat("\014", buffer);
		pri_str(serial, buffer);
	  pri_end:
		desk_wait(FALSE);
	}
}


/*	Create a new folder on the top window	*/

newfolder(win)
register WINDOW *win;
{
	char namenew[14];

	char nameold[14];

	char name[14];

	int16_t i,
	 ret,
	 update;

	register OBJECT *obj;

	char buf[2];

	if (win->w_path[0] == 'c')
	{
		do_alert(1, STBADCOP);
		return;
	}

	if (!c_path_alloc(win->w_path))
		return;

	strcpy(win->w_path, win->w_buf);
	obj = get_tree(ADMKDBOX);

	while (TRUE)
	{
		update = FALSE;
		name[0] = 0;
		inf_sset(obj, MKNAME, name);
		fm_draw(ADMKDBOX);				/* draw the box     */
		if (xform_do(obj, 0) == MKCNCL)
			break;

		inf_sget(obj, MKNAME, name);
		unfmt_str(name, namenew);
		if (!namenew[0])				/* no name  */
			break;

		rep_path(namenew, win->w_buf);	/* append the name  */

		ret = m_sfirst(win->w_buf, 0x10);

		if (ret)						/* Error    */
		{
			if (ret == EFILNF)			/* File Not found */
				goto n_1;

			if (ret == EACCDN)			/* access denied */
			{
				buf[0] = win->w_buf[0];
				buf[1] = 0;
				fill_string(buf, STDISKFULL);
			}

			break;
		} else							/* file exists  */
		{
			if (do1_alert(STFOEXIST) == 2)
				break;
			else
				continue;				/* retry    */
		}
	  n_1:
		desk_wait(TRUE);

	  n_2:if (Dcreate(win->w_buf))
		{
			if (fill_string(win->w_buf, CNTCRTDR) == 2)
				goto n_2;
		} else
			update = TRUE;

		desk_wait(FALSE);

		break;
	}									/* while    */

	do_finish(ADMKDBOX);

	if (update)
		up_allwin(win->w_path, TRUE);
}



/*	Sort the files		*/

sort_file(win, mode)
WINDOW *win;

int16_t mode;
{
	register DIR *dir;

	register int16_t n;

	register int16_t gap,
	 i,
	 j,
	 ret;

	DIR buff;

	register DIR *dir2;

	register DIR *dir1;

	char *ps1,
	*ps2;

	dir = win->w_memory;
	n = win->w_items;

	for (gap = n / 2; gap > 0; gap /= 2)
	{
		for (i = gap; i < n; i++)
		{
			for (j = i - gap; j >= 0; j -= gap)
			{
				if (mode == S_NO)
					goto ss_1;

				if ((dir[j + gap].d_att & SUBDIR) && (dir[j].d_att & SUBDIR))
					goto ss_1;

				if (dir[j + gap].d_att & SUBDIR)
					goto ss_2;

				if (dir[j].d_att & SUBDIR)
					break;
			  ss_1:
				dir1 = &dir[j];
				dir2 = &dir[j + gap];
				ps1 = &dir[j].d_name[0];
				ps2 = &dir[j + gap].d_name[0];

				switch (mode)
				{
				case S_NO:
					if (dir1->d_order > dir2->d_order)
						ret = 1;
					else
						ret = -1;

					break;

				case S_SIZE:
					if (dir2->d_size > dir1->d_size)
						ret = 1;
					else if (dir2->d_size < dir1->d_size)
						ret = -1;
					else
						ret = strchk(ps1, ps2);

					break;

				case S_TYPE:
					if (ret = strchk(scasb(ps1, '.'), scasb(ps2, '.')))
						break;

				case S_NAME:
					ret = strchk(ps1, ps2);
					break;

				case S_DATE:
					ret = 0;
					if (dir2->d_date > dir1->d_date)
						ret = 1;

					if (dir2->d_date < dir1->d_date)
						ret = -1;

					if (!ret)
					{
						if ((dir2->d_time >> 5) > (dir1->d_time >> 5))
							ret = 1;
					}
				}						/* switch    */

				if (ret <= 0)
					break;

			  ss_2:buff = dir[j];
				dir[j] = dir[j + gap];
				dir[j + gap] = buff;
			}
		}
	}
}


/*	Set up all the files pointer 	*/
/*	Scroll up or down		*/

int16_t set_newview(index, win)
int16_t index;

register WINDOW *win;
{
	register int16_t i,
	 k,
	 items,
	 vicons;

	DIR *dir;

	register OBJECT *obj;

	OBJECT *obj1;

	int16_t len,
	 type;

	char *text;

	obj = win->w_obj;					/* get all the icons source */
	obj->ob_next = 0xFFFF;
	/* No objects           */
	if (!(items = win->w_items))
	{
		obj->ob_head = 0xFFFF;
		obj->ob_tail = 0xFFFF;
		return;
	} else
	{
		obj->ob_head = 1;
		obj->ob_tail = 1;
	}

	obj++;								/* points to the rigth object   */

	dir = win->w_memory;

	vicons = win->w_vicons + 1;

	obj1 = get_tree(TEXTLINES);
	len = strlen(((TEDINFO *) (obj1[TSTYLE].ob_spec))->te_ptext);
	text = win->w_text;

	for (k = 1, i = index; ((i < items) && (k < vicons)); i++, k++, obj++)
	{
		if (s_view == S_ICON)
		{
			app_icon(&dir[i].d_name[0], (dir[i].d_att & 0x10) ? FOLDER : -1, &type);
			cp_iblk(type, (CICONBLK *) (obj->ob_spec));
			((CICONBLK *) (obj->ob_spec))->monoblk.ib_char[1] = 0;
			((CICONBLK *) (obj->ob_spec))->monoblk.ib_ptext = &dir[i].d_name[0];
		} else
		{
			bldstring(&dir[i], text);
			text += len;
			*(text - 1) = 0;
		}

		obj->ob_flags &= ~HIDETREE;
		obj->ob_states = dir[i].d_state;
		obj->ob_next = k + 1;

	}									/* for loop    */

	obj--;								/* go back to last one      */
	obj->ob_next = 0;					/* points back to parent    */

	win->w_obj->ob_tail = k - 1;		/* set up the parents   */

	win->w_srtitem = index;

	obj++;								/* finish up the rest   */

	for (; k < vicons; k++, obj++)
		obj->ob_flags |= HIDETREE;
}


/*	Read the files into a window	*/

int16_t read_files(win, attr)
register WINDOW *win;

int16_t attr;
{
	register int32_t items,
	 volume,
	 sizes;

	register DIR *addr;

	char buffer[14];

	char *path;

	int16_t ret,
	 i;

	items = 0;
	sizes = 0;

	Fsetdta(&dtabuf);					/* set dta buffer   */

	/*  Check to see if the drive is OK     */
	/*  Look for all subdir file        */

	path = win->w_path;

	save_ext(path, buffer);

	i = 0;
	while (buffer[i])
	{
		buffer[i] = toupper(buffer[i]);
		i++;
	}

	rep_path(getall, path);				/* search everything    */

	if (path[0] == 'c')
		ret = !cart_sfirst(&dtabuf);
	else
		ret = Fsfirst(path, 0x31);		/* Error    */

	if (ret)
	{
		if (ret != EFILNF)				/* Fatal error  */
		{
			rep_path(buffer, path);
			return (FALSE);
		}
	}

	if (win->w_memory)					/* Memory allocated?    */
	{
		Mfree(win->w_memory);			/* Free it      */
		win->w_memory = (char *) 0;
	}

	if (ret == EFILNF)					/* file not found   */
		goto r_2;

	addr = Malloc(0xFFFFFFFFL);			/* we should have memory */

	if (!addr)							/* No memory        */
	{
		do1_alert(FCNOMEM);
		goto r_1;
	}
	/* How many files we can store */

	volume = ((int32_t) addr) / (int32_t) (sizeof(DIR));

	addr = Malloc(addr);

	win->w_memory = (char *) addr;

	if (!volume)
		goto r_2;

	do
	{									/* volume label     */
		if (dtabuf.dirfile.d_att & VOLUME)
			goto r_3;
		/* directory file   */
		if (dtabuf.dirfile.d_att & 0x10)
		{
			if (dtabuf.dirfile.d_name[0] == '.')
			{
				if ((dtabuf.dirfile.d_name[1] == '.') || (!dtabuf.dirfile.d_name[1]))
					goto r_3;
			}
		} else
		{								/* file match ?     */
			if (!wildcmp(buffer, dtabuf.dirfile.d_name))
				goto r_3;
		}

		if (items <= volume)
		{
			dtabuf.dirfile.d_order = items;
			dtabuf.dirfile.d_state = NORMAL;
			*addr++ = dtabuf.dirfile;
			sizes += dtabuf.dirfile.d_size;
			items++;
		} else
			break;

	  r_3:
		if (path[0] == 'c')
			ret = !cart_snext();
		else
			ret = Fsnext();

	} while (!ret);

  r_2:
	rep_path(buffer, path);

	/* Free up some memory      */

	if (items)
		Mshrink(win->w_memory, (int32_t) ((char *) addr - (char *) win->w_memory));
	else
	{
		Mfree(win->w_memory);
		win->w_memory = (char *) 0;
	}

  r_1:
	win->w_items = (uint16_t) items;		/* total number of files    */
	sort_file(win, s_sort);
	return (TRUE);
}
