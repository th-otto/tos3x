/*	DESKMISC.C		3/16/89	- 9/13/89	Derek Mui	*/
/*	Change chk_par		9/25/89			D.Mui		*/
/*	Fix at fill_string to take out control character	7/9/91	*/
/*	Add get_fstring		7/7/92			D.Mui		*/

/************************************************************************/
/*	New Desktop for Atari ST/TT Computer				*/
/*	Atari Corp							*/
/*	Copyright 1989,1990 	All Rights Reserved			*/
/************************************************************************/

#include "desktop.h"

int16_t m_sfirst(P(const char *) path, P(int16_t) att)
PP(const char *path;)
PP(int16_t att;)
{
	int16_t ret;

	if (*path == 'c')
		return (!c_sfirst(path));
	else
	{
		desk_wait(TRUE);
		Fsetdta(&dtabuf);
		ret = Fsfirst(path, att);
		desk_wait(FALSE);
		return (ret);
	}
}


/*
 * Check path length and allocate memory
 */
int16_t c_path_alloc(P(const char *) path)
PP(const char *path;)
{
	int16_t level;

	level = ch_level(path);

	if (level > d_level)
	{
/*	  form_alert( 1, Alloc );	*/
		if (!path_alloc(level))
			return (FALSE);
	}

	return (TRUE);
}


int16_t hit_disk(P(int16_t) drive)
PP(int16_t drive;)
{
	char buffer[14];
	int16_t ret;

	strcpy(buffer, wildext);
	buffer[0] = drive;
	Fsetdta(&dtabuf);					/* set dta buffer   */
	if (ret = Fsfirst(buffer, 0x3F))
	{
		if (ret == EFILNF)				/* that's OK    */
			ret = FALSE;
	}

	return (ret ? FALSE : TRUE);
}


OBJECT *get_icon(P(int16_t) item)
PP(int16_t item;)
{
	if (item >= numicon)
		item = numicon - 1;

	return (&iconaddr[item]);
}


OBJECT *get_tree(P(int16_t) item)
PP(int16_t item;)
{
	OBJECT *obj;

	rsrc_gaddr(R_TREE, item, &obj);
	return (obj);
}



char *get_fstring(P(int16_t) item)
PP(int16_t item;)
{
	char **str;

	rsrc_gaddr(R_FRSTR, item, &str);
	return (*str);
}


char *get_string(P(int16_t) item)
PP(int16_t item;)
{
	char *str;

	rsrc_gaddr(R_STRING, item, &str);
	return (str);
}


DIR *get_dir(P(WINDOW *)win, P(int16_t) item)
PP(WINDOW *win;)
PP(int16_t item;)
{
	int16_t j;
	DIR *dir;

	j = win->w_srtitem + item - 1;
	dir = win->w_memory;
	return (&dir[j]);
}


VOID up_1(P(WINDOW *)win)
PP(WINDOW *win;)
{
	set_newview(win->w_srtitem, win);
	do_redraw(win->w_id, &full, 0);
}


/*
 * Update window because someone change the d_state
 */
VOID up_2(P(WINDOW *)win)
PP(WINDOW *win;)
{
	up_1(win);
	winfo(win);
}


/*
 * Put a file name into the win path buffer
 */
char *put_name(P(WINDOW *)win, P(const char *) name)
PP(WINDOW *win;)
PP(const char *name;)
{
	strcpy(win->w_buf, win->w_path);
	rep_path(name, win->w_buf);
	return (win->w_buf);
}



/*
 * Is the child inside the parent visible area?
 */
BOOLEAN in_parent(P(OBJECT *)obj, P(int16_t) child)
PP(OBJECT *obj;)
PP(int16_t child;)
{
	return inside(obj[child].ob_x + obj[0].ob_x, obj[child].ob_y + obj[0].ob_y, &obj[0].ob_x);
}


VOID xinf_sset(P(OBJECT *)obj, P(int16_t) item, P(char *)buf1)
PP(OBJECT *obj;)
PP(int16_t item;)
PP(char *buf1;)
{
	char buf2[20];

	fmt_str(buf1, buf2);
	inf_sset(obj, item, buf2);
}


VOID mice_state(P(int16_t) state)
PP(int16_t state;)
{
	graf_mouse(state, NULL);
}


VOID desk_wait(P(BOOLEAN) state)
PP(BOOLEAN state;)
{
	graf_mouse(state ? HOURGLASS : ARROW, NULL);
}


/*
 * Draw a single field of a dialog box
 */
VOID draw_fld(P(OBJECT *)obj, P(int16_t) which)
PP(register OBJECT *obj;)
PP(int16_t which;)
{
	GRECT t;
	int16_t x, y;

	objc_gclip(obj, which, &x, &y, &t.x, &t.y, &t.w, &t.h);
	objc_draw(obj, which, 0, t.x, t.y, t.w, t.h);
}


int16_t getcookie(P(int32_t) cookie, P(int32_t *)p_value)
PP(int32_t cookie;)
PP(int32_t *p_value;)
{
	int32_t *cookjar;

#if UNLINKED
	todo
#else
	cookjar = *(int32_t *) (0x5a0);
#endif

	if (!cookjar)
		return (FALSE);

	while (*cookjar)
	{
		if (*cookjar == cookie)
		{
			*p_value = *(cookjar + 1);
			return (TRUE);
		}

		cookjar += 2;
	}

	return (FALSE);
}


/*
 * Format a value and fill the leading space
 */
VOID f_str(P(OBJECT *)obj, P(int16_t) item, P(int32_t) value)
PP(OBJECT *obj;)
PP(int16_t item;)
PP(int32_t value;)
{
	char buffer[14];
	int16_t len1, len2;
	char *chrptr;

	lbintoas(value, buffer);
	len1 = (TEDINFO *) (obj[item].ob_spec)->te_txtlen;
	len1 -= 1;
	chrptr = (TEDINFO *) (obj[item].ob_spec)->te_ptext;
	bfill(len1, ' ', chrptr);
	len2 = strlen(buffer);
	strcpy(chrptr + (int32_t) (len1 - len2), buffer);
}


/*
 * Check the level of depth
 */
int16_t ch_level(P(const char *) path)
PP(const char *path;)
{
	int16_t level;

	level = 0;

	while (*path)
	{
		if (*path++ == '\\')
			level++;
	}

	return (level);
}


OBJECT *fm_draw(P(int16_t) item)
PP(int16_t item;)
{
	register OBJECT *addr;
	int16_t x, y, w, h;

	addr = get_tree(item);
	form_center(addr, &x, &y, &w, &h);
	objc_draw(addr, 0, 8, x, y, w, h);
	return (addr);
}


/*
 * Wait for redraw message
 */
VOID wait_msg(NOTHING)
{
	char msgbuff[16];
	int16_t event, trash;

	do
	{
		event = evnt_multi(MU_MESAG | MU_TIMER, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, msgbuff,	/* mesaage buffer   */
						   0, 0,		/* timer counter    */
						   &trash, &trash, &trash, &trash, &trash, &trash);

		if (event & MU_MESAG)
			hd_msg(msgbuff);
		else
			break;

	} while (TRUE);
}


VOID do_finish(P(int16_t) item)
PP(int16_t item;)
{
	GRECT pt;

	form_center(get_tree(item), &pt.x, &pt.y, &pt.w, &pt.h);
	form_dial(FMD_FINISH, 0, 0, 0, 0, pt.x, pt.y, pt.w, pt.h);
	wait_msg();
}


int16_t xform_do(P(OBJECT *)obj, P(int16_t) which)
PP(OBJECT *obj;)
PP(int16_t which;)
{
	int16_t ret;

	ret = form_do(obj, which) & 0x7FFF;
	obj[ret].ob_state = NORMAL;
	return (ret);
}


/*
 * Form_do and draw
 */
int16_t fmdodraw(P(int16_t) item, P(int16_t) which)
PP(int16_t item;)
PP(int16_t which;)
{
	OBJECT *obj;
	int16_t ret;

	obj = fm_draw(item);
	ret = xform_do(obj, which);
	do_finish(item);
	return (ret);
}



/*
 * This routine convert binary number to ascii value
 */
VOID lbintoasc(P(int32_t) longval, P(char *)buffer)
PP(register int32_t longval;)
PP(register char *buffer;)
{
	register int16_t i;
	uint16_t digit;
	int16_t j, k;
	int32_t divten;
	char buf1[12];

	i = 0;
	k = 0;
	if (!longval)
		buffer[k++] = '0';
	else
	{
		while (longval)
		{
			divten = longval / 10;
			digit = (int) (longval - (divten * 0x0AL));
			buf1[i++] = '0' + digit;
			longval = divten;
		}

		for (j = i - 1; j >= 0; j--)
			buffer[k++] = buf1[j];
	}

	buffer[k] = 0;
}


char *r_slash(P(char *)path)
PP(register char *path;)
{
	char *start;
	start = path;

	while (*path)
		path++;

	while (*path != '\\')
	{
		if (path == start)
			break;

		path--;
	}
	return (path);
}


/*
 * The path should look like A:\*.*
 */
int16_t xcut_path(P(char *)path, P(char *)buffer, P(int16_t) cut)
PP(register char *path;)
PP(char *buffer;)
PP(int16_t cut;)
{
	int16_t i;
	char *chrptr;

	buffer[0] = 0;

	i = ch_level(path);

	if (i <= 1)							/* only one level depth     */
		return (FALSE);					/* failed           */

	path = r_slash(path);				/* reverse to next level    */
	strcpy(buffer, path);
	path--;

	while (*path != '\\')
		path--;

	if (cut)
		strcpy(path, buffer);
	else
	{
		chrptr = strscn(path + 1, buffer, '\\');
		*chrptr = 0;
	}

	return (TRUE);
}


/*
 * Cut one directory inside the path
 * The path should look like A:\*.*
 */
int16_t cut_path(P(char *)path)
PP(register char *path;)
{
	char buffer[14];

	return xcut_path(path, buffer, TRUE);
}


/*
 * Concat a path
 * A:\aaa\*.*  -> A:\aaa\bbb\*.*
 */
VOID cat_path(P(char *)name, P(char *)path)
PP(char *name;)
PP(register char *path;)
{
	char buffer[20];
	char *start;

	start = path;
	path = r_slash(path);
	strcpy(buffer, path);
	path++;
	rep_path(name, start);
	strcat(path, buffer);
}


/*
 * Replace path
 * A:\aaa\*.* -> A:\aaa\bbb
 */
VOID rep_path(char *name, char *path)
char *name;
char *path;
{
	path = r_slash(path);
	path++;
	strcpy(path, name);
}


/*
 * Perform an alert box message
 */
int16_t do_alert(P(int16_t) button, P(int16_t) item)
PP(int16_t button:)
PP(int16_t item;)
{
	return (form_alert(button, get_string(item)));
}


int16_t do1_alert(P(int16_t) item)
PP(int16_t item;)
{
	return (do_alert(1, item));
}


VOID rc_center(P(GRECT *)rec1, P(GRECT *)rec2)
PP(GRECT *rec1;)
PP(GRECT *rec2;)
{
	rec2->x = rec1->x + (rec1->w / 2);
	rec2->y = rec1->y + (rec1->h / 2);
}



VOID my_itoa(P(uint16_t) number, P(char *)pnumstr)
PP(uint16_t number;)
PP(register char *pnumstr;)
{
	register int16_t ii;

	for (ii = 0; ii < 2; pnumstr[ii++] = '0')
		;
	pnumstr[2] = '\0';
	merge_str(((number > 9) ? pnumstr : pnumstr + 1), "%W", &number);
}

/*
 *	Routine to format DOS style time.
 *
 *	15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 *	<     hh     > <    mm    > <   xx  >
 *	hh = binary 0-23
 *	mm = binary 0-59
 *	xx = binary seconds \ 2 
 * 
 *	put into this form 12:45 pm
 *
 *	IF(EUROTIME)THEN Military Time Format 
 */
VOID fmt_time(P(uint16_t) time, P(char *)ptime)
PP(register uint16_t time;)
PP(register char *ptime;)
{
	register int16_t pm, val;

	val = ((time & 0xf800) >> 11) & 0x001f;

	if (st_time)
	{
		my_itoa(val, &ptime[0]);
	} else
	{
		if (val >= 12)
		{
			if (val > 12)
				val -= 12;
			pm = TRUE;
		} else
		{
			if (val == 0)
				val = 12;
			pm = FALSE;
		}

		my_itoa(val, &ptime[0]);
	}

	my_itoa(((time & 0x07e0) >> 5) & 0x003f, &ptime[2]);

	if (st_time)
		strcpy(&ptime[4], "  ");
	else
		strcpy(&ptime[4], pm ? "pm" : "am");
}


/*
 *	Routine to format DOS style date.
 *	
 *	15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 *	<     yy          > < mm  > <  dd   >
 *	yy = 0 - 99 (1980 - 2079)
 *	mm = 1 - 12
 *	dd = 1 - 31
 *
 *	IF(EURODATE)THEN Swap Day and Month thusly: dd/mm/yy
 *	IF(SWEDDATE)THEN Swap Day and Month thusly: yy/mm/dd
 *	Changed 7/20/92 To put in st_dchar as seperator
 */
VOID fmt_date(P(uint16_t) date, P(char *)pdate)
PP(uint16_t date;)
PP(register char *pdate;)
{
	switch (st_date)
	{
	case 3:
		my_itoa(((80 + ((date >> 9) & 0x007f)) % 100), &pdate[0]);
		my_itoa(date & 0x001f, &pdate[3]);
		my_itoa((date & 0x01e0) >> 5, &pdate[6]);
		break;
	case 2:							/* swedish date */
		my_itoa(((80 + ((date >> 9) & 0x007f)) % 100), &pdate[0]);
		my_itoa((date & 0x01e0) >> 5, &pdate[3]);
		my_itoa(date & 0x001f, &pdate[6]);
		break;
	case 1:							/* europane date */
		my_itoa(date & 0x001f, &pdate[0]);
		my_itoa((date & 0x01e0) >> 5, &pdate[3]);
		my_itoa(((80 + ((date >> 9) & 0x007f)) % 100), &pdate[6]);
		break;
	default:
		my_itoa((date & 0x01e0) >> 5, &pdate[0]);	/* MM */
		my_itoa(date & 0x001f, &pdate[3]);	/* DD */
		my_itoa(((80 + ((date >> 9) & 0x007f)) % 100), &pdate[6]);
	}

	pdate[5] = pdate[2] = (char) st_dchar;
}


/*
 * Build string
 */
char * bldstring(P(DIR *)dir, P(char *)dst)
PP(DIR *dir;)
PP(register char *dst;)
{
	register char *src;
	char buf[14];
	register int16_t i;
	int16_t t, len;
	char *buffer;

	buffer = buf;

	if (dir->d_att & READ)
		*dst++ = 0x7F;
	else
	{
		*dst++ = (dir->d_att & SUBDIR) ? 0x07 : ' ';
	}

	*dst++ = ' ';

	src = &dir->d_name[0];

	for (i = 0; i < 8; i++)
	{
		if ((*src) && (*src != '.'))
			*dst++ = *src++;
		else
			break;
	}

	for (; i < 9; i++)					/* fill with blanks */
		*dst++ = ' ';

	if (*src == '.')
		src++;

	for (i = 0; i < 3; i++)
		*dst++ = (*src) ? *src++ : ' ';

	bfill(11, ' ', dst++);

	if (dir->d_att & SUBDIR)
	{
		buffer[0] = ' ';
		buffer[1] = 0;
	} else
		lbintoasc(dir->d_size, buffer);

	len = strlen(buffer);

	strcpy(dst + (10 - len), buffer);

	dst += 10;
	*dst++ = ' ';
	*dst++ = ' ';

	fmt_date(dir->d_date, buffer);
	dst = strcpy(dst, buffer) - 1;
#if 0
	*dst++ = buffer[0];
	*dst++ = buffer[1];
	*dst++ = (char) st_dchar;
/*	*dst++ = '-';	*/
	*dst++ = buffer[2];
	*dst++ = buffer[3];
	*dst++ = (char) st_dchar;
/*	*dst++ = '-';	*/
	*dst++ = buffer[4];
	*dst++ = buffer[5];
#endif
	*dst++ = ' ';
	*dst++ = ' ';

	fmt_time(dir->d_time, buffer);
	*dst++ = buffer[0];
	*dst++ = buffer[1];
	*dst++ = ':';
	*dst++ = buffer[2];
	*dst++ = buffer[3];
	*dst++ = ' ';
	*dst++ = buffer[4];
	*dst++ = buffer[5];
	*dst++ = ' ';
	*dst++ = ' ';
	return (dst);
}


const char *g_name(P(const char *) file)
PP(const char *file;)
{
	const char *tail;

	tail = r_slash(file);
	if (*tail == '\\')
		tail++;

	return tail;
}


/*
 * save the extension of the path
 * A:\*.*
 */
VOID save_ext(P(char *)path, P(char *)buffer)
PP(char *path;)
PP(char *buffer;)
{
	strcpy(buffer, g_name(path));
}


/*
 * save the middle dir just before the ext
 * A:\dir\*.*
 */
VOID save_mid(P(char *)path, P(char *)buffer)
PP(char *path;)
PP(char *buffer;)
{
	xcut_path(path, buffer, FALSE);
}


/*
 * Check for if the source is the parent of
 * the destination
 */
int16_t chk_par(P(char *)srcptr, P(char *)dstptr)
PP(register char *srcptr;)
PP(register char *dstptr;)
{
	if (*srcptr != *dstptr)				/* Not the same device  */
		return (TRUE);

	if (!streq(srcptr, dstptr))
	{									/* not the same path    */
		while (*srcptr)
		{
			if (*srcptr != *dstptr)
			{
				if (*srcptr == '*')		/* source is the parent */
					break;				/* return FALSE     */
				else
					return (TRUE);
			}

			if (!*dstptr)				/* destination is shorter */
				return (TRUE);

			srcptr++;
			dstptr++;
		}								/* while */
	}

	return (FALSE);
}


int16_t fill_string(P(char *)string, P(int16_t) item)
PP(char *string;)
PP(int16_t item;)
{
	char *ptr, *ptr1;

	ptr1 = ptr = g_name(string);

	while (*ptr1)
	{
		if ((*ptr1 == '[') || (*ptr1 == ']') || (*ptr1 == '|'))
			strcpy(ptr1, ptr1 + 1);
		else
			ptr1++;
	}

	merge_str(g_buffer, get_string(item), &ptr);
	return (form_alert(1, g_buffer));
}


/*
 * change from ascii to binary value
 * TRUE is OK otherwise it is FALSE
 */
BOOLEAN asctobin(P(char *)ptr, P(int32_t *)value)
PP(char *ptr;)
PP(int32_t *value;)
{
	int32_t n, n1;
	uint16_t i;

	for (n = 0, i = 0, n1 = 0; *ptr; ptr++)
	{
		if ((*ptr >= '0') && (*ptr <= '9'))
		{
			n1 = (10 * n1) + (*ptr - '0');
			if (n1 >= n)
				n = n1;
			else
				return (FALSE);
		} else
			return (FALSE);
	}

	*value = n;
	return (TRUE);
}
