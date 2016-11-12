/*      DESKMENU.C              3/16/89 - 9/18/89       Derek Mui       */
/*      Put o_select and x_select into menu_verify      7/3/90  D.Mui   */
/*      Scan ascii '>' instead of whole scan code       7/12/90 D.Mui   */
/*      Two new help menu               7/12/90         D.Mui           */
/*      Display print screen if it is ST mode   7/31/90 D.Mui           */
/*      Fix the alt drive table                 8/27/90 D.Mui           */
/*      Use m_cpu to determine cache or no cache        9/19/90 D.Mui   */
/*      Fix at read_inf to read file only       7/15/91 D.Mui           */
/*      Changes to do_opt()                    07/07/92 C.Gee           */
/*        - Call mv_desk() instead of av_desk()                         */
/*        - Call mins_app() instead of ins_app()                        */
/*        - Call mdesk_pref() instead of desk_pref()                    */
/*        - Add set_video() to do_opt()                                 */
/*      Changes to tb3[] - added VIDITEM object                         */
/*      Changes to hdkeybd() - added VIDITEM object                     */


/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"

#if TOSVERSION >= 0x400
extern int32_t gl_vdo;
#endif

int16_t d_exit;		/* desktop exit flag	*/
char mentable[MAXMENU];

#if MULTILANG_SUPPORT
STATIC const int16_t *keytable;
STATIC const int16_t *contable;
#endif

/* Alternate keys table */

/* USA|UK|SPAIN|ITALY|SWEDEN */
#if MULTILANG_SUPPORT | (OS_COUNTRY == CTRY_US) | (OS_COUNTRY == CTRY_UK) | (OS_COUNTRY == CTRY_ES) | (OS_COUNTRY == CTRY_IT) | (OS_COUNTRY == CTRY_SV)
static int16_t const key1table[] = {
	0x1e00,	/* ALT-'A' */
	0x3000, /* ALT-'B' */
	0x2e00, /* ALT-'C' */
	0x2000, /* ALT-'D' */
	0x1200, /* ALT-'E' */
	0x2100, /* ALT-'F' */
	0x2200, /* ALT-'G' */
	0x2300, /* ALT-'H' */
	0x1700, /* ALT-'I' */
	0x2400, /* ALT-'J' */
	0x2500, /* ALT-'K' */
	0x2600, /* ALT-'L' */
	0x3200, /* ALT-'M' */
	0x3100, /* ALT-'N' */
	0x1800, /* ALT-'O' */
	0x1900, /* ALT-'P' */
	0x1000, /* ALT-'Q' */
	0x1300, /* ALT-'R' */
	0x1f00, /* ALT-'S' */
	0x1400, /* ALT-'T' */
	0x1600, /* ALT-'U' */
	0x2f00, /* ALT-'V' */
	0x1100, /* ALT-'W' */
	0x2d00, /* ALT-'X' */
	0x1500, /* ALT-'Y' */
	0x2c00, /* ALT-'Z' */
	0
};
#if !MULTILANG_SUPPORT
#define keytable key1table
#endif
#endif

#if MULTILANG_SUPPORT | (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_SF) | (OS_COUNTRY == CTRY_SG) | (OS_COUNTRY == CTRY_PL)
/* GERMANY|SWISSFRE|SWISSGER */
static int16_t const key2table[] = {
	0x1e00,	/* ALT-'A' */
	0x3000, /* ALT-'B' */
	0x2e00, /* ALT-'C' */
	0x2000, /* ALT-'D' */
	0x1200, /* ALT-'E' */
	0x2100, /* ALT-'F' */
	0x2200, /* ALT-'G' */
	0x2300, /* ALT-'H' */
	0x1700, /* ALT-'I' */
	0x2400, /* ALT-'J' */
	0x2500, /* ALT-'K' */
	0x2600, /* ALT-'L' */
	0x3200, /* ALT-'M' */
	0x3100, /* ALT-'N' */
	0x1800, /* ALT-'O' */
	0x1900, /* ALT-'P' */
	0x1000, /* ALT-'Q' */
	0x1300, /* ALT-'R' */
	0x1f00, /* ALT-'S' */
	0x1400, /* ALT-'T' */
	0x1600, /* ALT-'U' */
	0x2f00, /* ALT-'V' */
	0x1100, /* ALT-'W' */
	0x2d00, /* ALT-'X' */
	0x2c00, /* ALT-'Y' */
	0x1500, /* ALT-'Z' */
	0
};
#if !MULTILANG_SUPPORT
#define keytable key2table
#endif
#endif

#if MULTILANG_SUPPORT | (OS_COUNTRY == CTRY_FR)
/* FRENCH */
static int16_t const key3table[] = {
	0x1000,	/* ALT-'A' */
	0x3000, /* ALT-'B' */
	0x2e00, /* ALT-'C' */
	0x2000, /* ALT-'D' */
	0x1200, /* ALT-'E' */
	0x2100, /* ALT-'F' */
	0x2200, /* ALT-'G' */
	0x2300, /* ALT-'H' */
	0x1700, /* ALT-'I' */
	0x2400, /* ALT-'J' */
	0x2500, /* ALT-'K' */
	0x2600, /* ALT-'L' */
	0x2700, /* ALT-'M' */
	0x3100, /* ALT-'N' */
	0x1800, /* ALT-'O' */
	0x1900, /* ALT-'P' */
	0x1e00, /* ALT-'Q' */
	0x1300, /* ALT-'R' */
	0x1f00, /* ALT-'S' */
	0x1400, /* ALT-'T' */
	0x1600, /* ALT-'U' */
	0x2f00, /* ALT-'V' */
	0x2c00, /* ALT-'W' */
	0x2d00, /* ALT-'X' */
	0x1500, /* ALT-'Y' */
	0x1100, /* ALT-'Z' */
	0
};
#if !MULTILANG_SUPPORT
#define keytable key3table
#endif
#endif


/* Control keys table */

/* USA|UK|SPAIN|ITALY|SWEDEN */
#if MULTILANG_SUPPORT | (OS_COUNTRY == CTRY_US) | (OS_COUNTRY == CTRY_UK) | (OS_COUNTRY == CTRY_ES) | (OS_COUNTRY == CTRY_IT) | (OS_COUNTRY == CTRY_SV)
/* USA|UK|SPAIN|ITALY|SWEDEN */
static int16_t const con1table[] = {
	0x1e01,	/* CTRL-'A' */
	0x3002, /* CTRL-'B' */
	0x2e03, /* CTRL-'C' */
	0x2004, /* CTRL-'D' */
	0x1205, /* CTRL-'E' */
	0x2106, /* CTRL-'F' */
	0x2207, /* CTRL-'G' */
	0x2308, /* CTRL-'H' */
	0x1709, /* CTRL-'I' */
	0x240a, /* CTRL-'J' */
	0x250b, /* CTRL-'K' */
	0x260c, /* CTRL-'L' */
	0x320d, /* CTRL-'M' */
	0x310e, /* CTRL-'N' */
	0x180f, /* CTRL-'O' */
	0x1910, /* CTRL-'P' */
	0x1011, /* CTRL-'Q' */
	0x1312, /* CTRL-'R' */
	0x1f13, /* CTRL-'S' */
	0x1414, /* CTRL-'T' */
	0x1615, /* CTRL-'U' */
	0x2f16, /* CTRL-'V' */
	0x1117, /* CTRL-'W' */
	0x2d18, /* CTRL-'X' */
	0x1519, /* CTRL-'Y' */
	0x2c1a, /* CTRL-'Z' */
	0
};
#if !MULTILANG_SUPPORT
#define contable con1table
#endif
#endif

#if MULTILANG_SUPPORT | (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_SF) | (OS_COUNTRY == CTRY_SG) | (OS_COUNTRY == CTRY_PL)
/* GERMANY|SWISSFRE|SWISSGER */
static int16_t const con2table[] = {
	0x1e01,	/* CTRL-'A' */
	0x3002, /* CTRL-'B' */
	0x2e03, /* CTRL-'C' */
	0x2004, /* CTRL-'D' */
	0x1205, /* CTRL-'E' */
	0x2106, /* CTRL-'F' */
	0x2207, /* CTRL-'G' */
	0x2308, /* CTRL-'H' */
	0x1709, /* CTRL-'I' */
	0x240a, /* CTRL-'J' */
	0x250b, /* CTRL-'K' */
	0x260c, /* CTRL-'L' */
	0x320d, /* CTRL-'M' */
	0x310e, /* CTRL-'N' */
	0x180f, /* CTRL-'O' */
	0x1910, /* CTRL-'P' */
	0x1011, /* CTRL-'Q' */
	0x1312, /* CTRL-'R' */
	0x1f13, /* CTRL-'S' */
	0x1414, /* CTRL-'T' */
	0x1615, /* CTRL-'U' */
	0x2f16, /* CTRL-'V' */
	0x1117, /* CTRL-'W' */
	0x2d18, /* CTRL-'X' */
	0x2c19, /* CTRL-'Y' */
	0x151a, /* CTRL-'Z' */
	0
};
#if !MULTILANG_SUPPORT
#define contable con2table
#endif
#endif

#if MULTILANG_SUPPORT | (OS_COUNTRY == CTRY_FR)
/* FRENCH */
static int16_t const con3table[] = {
	0x1001,	/* CTRL-'A' */
	0x3002, /* CTRL-'B' */
	0x2e03, /* CTRL-'C' */
	0x2004, /* CTRL-'D' */
	0x1205, /* CTRL-'E' */
	0x2106, /* CTRL-'F' */
	0x2207, /* CTRL-'G' */
	0x2308, /* CTRL-'H' */
	0x1709, /* CTRL-'I' */
	0x240a, /* CTRL-'J' */
	0x250b, /* CTRL-'K' */
	0x260c, /* CTRL-'L' */
	0x270d, /* CTRL-'M' */
	0x310e, /* CTRL-'N' */
	0x180f, /* CTRL-'O' */
	0x1910, /* CTRL-'P' */
	0x1e11, /* CTRL-'Q' */
	0x1312, /* CTRL-'R' */
	0x1f13, /* CTRL-'S' */
	0x1414, /* CTRL-'T' */
	0x1615, /* CTRL-'U' */
	0x2f16, /* CTRL-'V' */
	0x2c17, /* CTRL-'W' */
	0x2d18, /* CTRL-'X' */
	0x1519, /* CTRL-'Y' */
	0x111a, /* CTRL-'Z' */
	0
};
#if !MULTILANG_SUPPORT
#define contable con3table
#endif
#endif

/* Arrow keys table */
static int16_t const arrowtab[] = {
	0x4d36, 0x5032, 0x4838, 0x4b34, 0x4800, 0x5000, 0x4d00,	0x4b00, 0
};

/* Arrow keys Message */
static int16_t const arrowimg[] = { 5, 1, 0, 4, 2, 3, 7, 6 };

static int16_t const tb1[] = { NEWFITEM, CLSFITEM, CLSWITEM, BOTTOP, EXTMENU, SELALL, 0 };
static int16_t const tb2[] = { REMVICON, SEAMENU, SHOWITEM, 0 };

static int16_t const xxxview[9] = {
	ICONITEM, TEXTITEM, 0, NAMEITEM, DATEITEM, SIZEITEM,
	TYPEITEM, NOSORT, 0
};

/* cjg - added VIDITEM - 07/07/92 */
/* Moved VIDITEM after set preferences */
/* Take out BITBLT and PRINTITEM */
int16_t const tb3[MAXMENU] = {
	OPENITEM,
	SHOWITEM,
	SEAMENU,
	DELMENU,
	NEWFITEM,
	CLSFITEM,
	CLSWITEM,
	BOTTOP,
	SELALL,
	EXTMENU,
	FORMITEM,
	ICONITEM,
	TEXTITEM,
	NAMEITEM,
	DATEITEM,
	SIZEITEM,
	TYPEITEM,
	NOSORT,
	SIZEFIT,
	SCOLTYPE,
	IDSKITEM,
	IAPPITEM,
	INSDISK,
	REMVICON,
	PREFITEM,
#ifdef VIDITEM
	VIDITEM,
#endif
	READINF,
	MEMMENU,
	SAVEITEM,
#ifdef PRINTITEM
	PRINTITEM,
#endif
#ifdef BITBLT
	BITBLT,
#endif
};

/* Alternate Number Table */

static int16_t const altnum[] = { 0x7800, 0x7900, 0x7a00, 0x7c00, 0x7d00, 0x7b00, 0 };

BOOLEAN o_status;							/* for o_select */
WINDOW *o_win;
int16_t o_type;
int16_t o_item;


int16_t loop_find PROTO((int16_t input, const int16_t *table, int16_t *index));
VOID sel_all PROTO((NOTHING));
VOID hd_keybd PROTO((uint16_t key));
VOID foption PROTO((WINDOW *win));
VOID do_scroll PROTO((int16_t *msgbuff));
VOID do_opt PROTO((int16_t msgbuff));
VOID hd_menu PROTO((int16_t *msgbuff));



/*
 * Change one individual key
 */
 
/* 306de: 00e2dcfa */
VOID ch_key(P(int16_t) i)
PP(int16_t i;)
{
	OBJECT *obj;
	register int16_t l;
	register char *str;

	obj = menu_addr;
	str = (char *)obj[tb3[i]].ob_spec;
	l = strlen(str);
	if (mentable[i])
	{
		strcpy(&str[l - 3], "[ ]");
		str[l - 2] = mentable[i];
	} else
	{
		strcpy(&str[l - 3], "   ");
	}
}


/*
 * Put keys into the menus
 */
/* 306de: 00e2dd98 */
VOID put_keys(NOTHING)
{
	register int16_t i;

	for (i = 0; i < MAXMENU; i++)
		ch_key(i);
}


/*
 * Loop to find matching array
 */
/* 306de: 00e2ddbc */
int16_t loop_find(P(int16_t) input, P(const int16_t *) table, P(int16_t *)index)
PP(int16_t input;)
PP(const int16_t *table;)
PP(int16_t *index;)
{
	register int16_t i;

	i = 0;

	while (table[i])
	{
		if (input == table[i])
		{
			*index = i;
			return TRUE;
		} else
		{
			i++;
		}
	}

	return FALSE;
}


/*
 * Enable or disable the menu item according to current status
 */
/* 306de: 00e2de02 */
VOID menu_verify(NOTHING)
{
	int16_t enable;
	register int16_t i;
	WINDOW *win;
	int16_t type;
	const char *str;

	UNUSED(win);

	/* If there is a window opened/closed check to see if there is anything selected */
	o_select();
	x_select();

	enable = winhead->w_id == -1 ? FALSE : TRUE;

	i = 0;
	while (tb1[i])
		menu_ienable(menu_addr, tb1[i++], enable);

	i = 0;

	if (x_status)
	{
		enable = TRUE;
		if (x_type == WINICON)
			i = 1;
	} else
	{
		enable = FALSE;
	}
	
	while (tb2[i])
		menu_ienable(menu_addr, tb2[i++], enable);

	enable = x_first(&str, &type);

	menu_ienable(menu_addr, DELMENU, enable);
	menu_ienable(menu_addr, IAPPITEM, enable);

	enable = FALSE;

	if (w_gfirst())
	{
		if (w_gnext())
			enable = TRUE;

		menu_ienable(menu_addr, SEAMENU, TRUE);
		menu_ienable(menu_addr, SHOWITEM, TRUE);
	}

	menu_ienable(menu_addr, BOTTOP, enable);
	ch_cache(FALSE);
#ifdef PRINTITEM /* take out for sparrow */
	menu_ienable(menu_addr, PRINTITEM, gl_restype <= 3 ? TRUE : FALSE);
#endif
}


/*
 * Select all objects inside the window
 */
/* 306de: 00e2df94 */
VOID sel_all(NOTHING)
{
	register WINDOW *win;
	register DIR *dir;
	register int16_t i;
	char buffer[NAMELEN];

	if ((win = w_gfirst()) != NULL)
	{
		clr_dicons();
		dir = win->w_memory;
		strcpy(buffer, g_name(win->w_path));
		for (i = 0; i < win->w_items; i++)
		{
			if (wildcmp(buffer, dir[i].d_name))
				dir[i].d_state = SELECTED;
		}

		up_2(win);
	}
}


/*
 * Handle the keyboard
 */
/* 3ß6de: 00e2e024 */
VOID hd_keybd(P(uint16_t) key)
PP(uint16_t key;)
{
	OBJECT *obj;
	register WINDOW *win;
	register int16_t item;
	int16_t msgbuff[8];
	char buffer[NAMELEN];
	int16_t i;
	char *str;
	APP *app;

	UNUSED(str);
	UNUSED(obj);
	
#if MULTILANG_SUPPORT
	switch (st_keybd)
	{
	case CTRY_DE:
	case CTRY_SF:
	case CTRY_SG:
		keytable = key2table;
		contable = con2table;
		break;
	case CTRY_FR:
		keytable = key3table;
		contable = con3table;
		break;
	case CTRY_US:					/* usa */
	case CTRY_UK:					/* uk   */
	case CTRY_ES:					/* spain */
	case CTRY_IT:					/* italy */
	case CTRY_SV:					/* sweden */
	default:
		keytable = key1table;
		contable = con1table;
		break;
	}
#endif

	if ((app = app_key(key)))
	{
		exec_file(app->a_name, (WINDOW *) 0, 0, Nostr);
		return;
	}

	win = get_top();

	item = toupper(key);

	for (i = 0; i < MAXMENU; i++)
	{
		if (!mentable[i])
			continue;

		if (item == toupper(mentable[i]))
		{
			switch (i)
			{
			case 0:						/* Open item    */
			case 1:						/* Show item    */
			case 2:						/* Search   */
			case 3:						/* Delete   */
			case 4:						/* Folder making    */
			case 5:						/* Close directory  */
			case 6:						/* Close top window */
			case 7:						/* Bottom to top    */
			case 8:						/* Select all       */
			case 9:						/* Set Extension    */
			case 10:					/* Format Disk      */
				do_file(tb3[i]);
				return;

			case 11:					/* Icon mode        */
			case 12:					/* Text mode        */
			case 13:					/* Sort by Name     */
			case 14:					/* Sort by Date     */
			case 15:					/* Sort by Size     */
			case 16:					/* Sort by Type     */
			case 17:					/* No Sort      */
			case 18:					/* Size to fit      */
			case 19:					/* Window Style     */
				do_view(tb3[i]);
				return;

			case 20:					/* Install disk icon    */
			case 21:					/* Install application  */
			case 22:					/* Install Device   */
			case 23:					/* Remove icons     */
			case 24:					/* Desktop Preference   */
			case 25:					/* read inf file    */
			case 26:					/* desktop config   */
			case 27:					/* save desktop.inf */
			case 28:					/* print screen     */
			case 29:					/* set blt      */
#if MAXMENU > 30
			case 30:					/* cjg - 06/19/92  Set Video    */
#endif
				do_opt(tb3[i]);
				return;
			}
			break;
		}
	}

#if TOSVERSION >= 0x400
	/* 7/16/92 */
	if (((gl_vdo >> 16) & 0xFFFFL) <= 0x02)
#endif
	{
		if (loop_find(key, altnum, &i))
		{
			if (restable[i])
			{
				if (app_reschange(i + 1))
				{
					d_exit = L_CHGRES;
					return;
				}
			}
		}
	}

	/* open a disk drive */
	if (loop_find(key, keytable, &i))
	{
		if (win)
			clr_xwin(win, TRUE);
	hx_1:
		strcpy(buffer, wildext);
		buffer[0] = i + 'A';
		open_disk(0, buffer, TRUE);
		return;
	}

	if (loop_find(key, contable, &i))
	{
		if (ch_drive(i + 'A'))
		{
			if (win)					/* replace top window   */
			{
				strcpy(win->w_buf, win->w_path);
				strcpy(buffer, g_name(win->w_path));
				strcpy(win->w_path, wildext);
				win->w_path[0] = i + 'A';
				rep_path(buffer, win->w_path);
				if (!up_win(win, FALSE))
					strcpy(win->w_path, win->w_buf);
				return;
			} else						/* open a window    */
				goto hx_1;
		}
	}

	if (win)							/* there is a window    */
	{
		if (loop_find(key, arrowtab, &i))
		{
			msgbuff[0] = WM_ARROWED;
			msgbuff[3] = win->w_id;
			msgbuff[4] = arrowimg[i];
			do_scroll(msgbuff);
			return;
		}
	}

	if (item == 0x3e)					/* >    */
	{
		pri_win();
		return;
	}

	if (key == 0x6200)					/* help     */
	{
		if (fmdodraw(HELP1, -1) == MOREHELP)
			fmdodraw(HELP2, -1);
		return;
	}

	if (item == 0x0D)					/* return       */
	{
		clr_allwin();					/* deselect all items   */
		clr_dicons();
	}

	if (item == 0x1B)					/* escape       */
	{
		if (win)						/* update top window    */
			up_win(win, TRUE);
	}
}


/*
 * Set file option
 */
/* 306de: 00e2e34e */
VOID foption(P(WINDOW *)win)
PP(register WINDOW *win;)
{
	register OBJECT *obj;
	char buffer[NAMELEN];
	char buf2[NAMELEN];
	char buf1[NAMELEN];

	obj = get_tree(FILEOPTI);
	save_ext(win->w_path, buf2);
	strcpy(buf1, buf2);
	xinf_sset(obj, OPTNAME, buf2);
	if (fmdodraw(FILEOPTI, 0) == OPTOK)
	{
		fs_sget((LPTREE)obj, OPTNAME, buffer);
		unfmt_str(buffer, buf2);
		if (!buf2[0])
			strcpy(buf2, getall);

		if (!streq(buf1, buf2))
		{
			rep_path(buf2, win->w_path);
			up_win(win, FALSE);
		}
	}
}


/* 306de: 00e2e44e */
VOID do_scroll(P(int16_t *)msgbuff)
PP(int16_t *msgbuff;)
{
	register int16_t act;
	register WINDOW *win;
	int16_t bdown, x, y;

	act = msgbuff[4];

	if ((win = get_win(msgbuff[3])) != NULL)
	{
		switch (act)
		{
		case WA_DNPAGE:						/* page down */
		case WA_UPPAGE:						/* page up */
			srl_row(win, win->w_xrow, act == WA_DNPAGE ? SDOWN : SUP);
			break;

		case WA_DNLINE:						/* arrow down */
		case WA_UPLINE:						/* arrow up */
#if TOSVERSION >= 0x400
			srl_row(win, 1, act == WA_UPLINE ? SUP : SDOWN);
			UNUSED(x);
			UNUSED(y);
			UNUSED(bdown);
#else
			do {
				srl_row(win, 1, act == WA_UPLINE ? SUP : SDOWN);
				graf_mkstate(&x, &y, &bdown, &x);
			} while (bdown);
#endif
			break;

		case WA_RTPAGE:						/* page left or right */
		case WA_LFPAGE:
			srl_col(win, win->w_xcol, act == WA_LFPAGE ? SRIGHT : SLEFT); /* hhmm? */
			break;

		case WA_RTLINE:
		case WA_LFLINE:						/* scroll left */
#if TOSVERSION >= 0x400
			srl_col(win, 1, act == WA_LFLINE ? SRIGHT : SLEFT); /* hhmm? */
#else
			do {
				srl_col(win, 1, act == WA_LFLINE ? SRIGHT : SLEFT); /* hhmm? */
				graf_mkstate(&x, &y, &bdown, &x);
			} while (bdown);
#endif
			break;
		}
	}
}



/*
 * Do the option menu
 */
/* 306de: 00e2e55e */
VOID do_opt(P(int16_t) msgbuff)
PP(int16_t msgbuff;)
{
	register OBJECT *obj;
	int16_t ret, button, handle;
	char buffer[16];
	const char *str;

	UNUSED(obj);
	
	switch (msgbuff)
	{
	case IDSKITEM:						/* install disk icon    */
		if (x_status)
		{
			if (x_type == DESKICON)
				goto opt1;
			else
				goto opt2;
		}

		ret = do1_alert(ICONTYPE);
		if (ret == 2)
		{
		opt2:
			ins_wicons();
		}
		if (ret == 1)
		{
		opt1:
			ins_icons();
		}
		break;

	case IAPPITEM:
		if (x_first(&str, &button))
#if POPUP_SUPPORT
			mins_app();
#else
			ins_app();
#endif
		break;

	case INSDISK:
		ins_drive();
		return;

	case REMVICON:						/* remove icon      */
		rm_icons();
		break;

	case PREFITEM:						/* desktop preference   */
#if POPUP_SUPPORT
		mdesk_pref();					/* cjg 07/07/92 */
#else
		desk_pref();
#endif
		break;

	case READINF:
		strcpy(path1, "C:\\*.INF");
		path1[0] = (isdrive() & 0x04) ? 'C' : 'A';
		strcpy(buffer, &infpath[3]);
#if STR_IN_RSC
		fsel_exinput(path1, buffer, &button, get_fstring(SINF));
#else
		fsel_exinput(path1, buffer, &button, Sinf);
#endif
		if (button)
		{
			desk_wait(TRUE);
			rep_path(buffer, path1);
			handle = Fopen(path1, 0x00);
			if (handle > 0)
			{
				bfill(INFSIZE, 0, afile);
				Fread(handle, (int32_t) INFSIZE, afile);
				Fclose(handle);
				if (afile[0] == '#' && afile[1] == 'a')
				{
					shel_put(afile, INFSIZE);	/* copy to the aes buffer */
					d_exit = L_READINF;
					wait_msg();
					strcpy(inf_path, path1);	/* new inf path */
				} else
				{
					do1_alert(ILLMAT);
				}
			} else
			{
				fill_string(path1, FNOTFIND);
			}
			desk_wait(FALSE);
		}
		break;

	case MEMMENU:
#if POPUP_SUPPORT
		mv_desk();						/* cjg - 07/07/92 */
#else
		av_desk();
#endif
		break;

	case SAVEITEM:						/* save desktop     */
		if (do1_alert(SAVETOP) == 1)
			save_inf(TRUE);
		break;

#ifdef PRINTITEM								/* take out for sparrow */
	case PRINTITEM:						/* print screen     */
		if (!(menu_addr[PRINTITEM].ob_state & DISABLED))
		{
			if (do1_alert(PRINTTOP) == 1)
			{
				desk_wait(TRUE);
				av_hardcopy();
				desk_wait(FALSE);
			}
		}
		break;
#endif

#ifdef BITBLT
	case BITBLT:
		if (m_cpu != 30)
			cbit_save = !cbit_save;
		else
			s_cache = !s_cache;
		ch_cache(TRUE);
		break;
#endif

#ifdef VIDITEM
	case VIDITEM:
		if (set_video())
			d_exit = L_CHGRES;
		break;
#endif
	}
}


/*
 * Do the sorting menu
 */
/* 306de: 00e2e812 */
VOID do_view(P(int16_t) msgbuff)
PP(int16_t msgbuff;)
{
	int16_t mode, i;

#if !BINEXACT
	i = 0; /* quiet compiler */
#endif
	switch (msgbuff)
	{
	case SCOLTYPE:
		col_pa_pref();
		return;

	case SIZEFIT:						/* size to fit  */
		s_stofit = !s_stofit;
		sort_show(0, TRUE);
		goto v_4;

	case ICONITEM:						/* show as icon */
		mode = S_ICON;
		goto v_1;

	case TEXTITEM:						/* show as text */
		mode = S_TEXT;
	v_1:
		i = 0;
		if (mode == s_view)				/* No change    */
			break;

		s_view = mode;
		sort_show(0, TRUE);
		break;

	case NAMEITEM:
		mode = S_NAME;
		goto v_2;

	case DATEITEM:
		mode = S_DATE;
		goto v_2;

	case SIZEITEM:
		mode = S_SIZE;
		goto v_2;

	case TYPEITEM:
		mode = S_TYPE;
		goto v_2;

	case NOSORT:
		mode = S_NO;
	v_2:
		i = 3;
		if (mode == s_sort)				/* No change    */
			break;
		s_sort = mode;
		sort_show(s_sort, FALSE);
		break;
	}

  	while (xxxview[i])
		menu_icheck(menu_addr, xxxview[i++], 0);

	menu_icheck(menu_addr, msgbuff, 1);
v_4:
	menu_icheck(menu_addr, SIZEFIT, s_stofit);
}


/*
 * Do the file menu
 */
/* 306de: 00e2e958 */
VOID do_file(P(int16_t) msgbuff)
PP(int16_t msgbuff;)
{
	OBJECT *obj;
	int16_t ret, type;
	register WINDOW *win;
	char buffer[NAMELEN];
	char buf2[NAMELEN];
	const char *str;
	const char *str1;

	win = get_top();

	switch (msgbuff)
	{
	case OPENITEM:
		if (o_status)
			open_item(o_item, o_type, o_win);
		else
			launch_pref();
		break;

	case SHOWITEM:
		show_item();
		break;

	case SEAMENU:						/* search file menu */
		if (x_status || w_gfirst())
		{
			obj = get_tree(SEAFILE);
			inf_sset(obj, SFILE, Nostr);
			if (fmdodraw(SEAFILE, SFILE) == SEAOK)
			{
				fs_sget((LPTREE)obj, SFILE, buffer);
				unfmt_str(buffer, buf2);
				if (buf2[0])
					sea_file(buf2);
			}
		}
		break;

	case DELMENU:
		if (!(menu_addr[DELMENU].ob_state & DISABLED))
		{
			if (x_status)
			{
				if (x_type == DESKICON)	/* desktop icons    */
				{
					ret = do1_alert(DELITEM);
					if (ret == 1)		/* remove       */
					{
						rm_icons();
						break;
					}

					if (ret == 3)		/* cancel       */
						break;
				}
				file_op(Nostr, OP_DELETE);
			}
		}
		break;

	case NEWFITEM:
		if (win)
			newfolder(win);
		break;

	case CLSFITEM:
		if (win)
			close_path(win);			/* close one path   */
		break;

	case CLSWITEM:						/* close the top window */
		close_top();
		break;

	case BOTTOP:						/* bring bot win to top */
		bottop();
		break;

	case SELALL:
		sel_all();
		break;

	case EXTMENU:
		if (win)
			foption(win);
		break;

	case FORMITEM:
		str = "A";

		if (x_status && x_type == DESKICON)
		{
			ret = x_first(&str1, &type);
			while (ret)
			{
				if (type == DISK)
				{
					if (*str1 == 'A' || *str1 == 'B')
					{
						str = str1;
						break;
					}
				}
				ret = x_next(&str1, &type);
			}
		}

		fc_start(str, CMD_FORMAT);
		clr_dicons();
		break;
	}
}


/*
 * Handle Menu
 */
/* 306de: 00e2eb92 */
VOID hd_menu(P(int16_t *)msgbuff)
PP(int16_t *msgbuff;)
{
	switch (msgbuff[3])
	{
	case DESKMENU:
		/* roton(); */
		fmdodraw(ADDINFO, 0);
		/* rotoff(); */
		break;

	case FILEMENU:
		do_file(msgbuff[4]);
		break;

	case VIEWMENU:
		do_view(msgbuff[4]);
		break;

	case OPTNMENU:
		do_opt(msgbuff[4]);
		break;
	}

	menu_tnormal(menu_addr, msgbuff[3], 1);
}


/*
 * Handle all the different messages
 */
/* 306de: 00e2ec12 */
VOID hd_msg(P(int16_t *)msgbuff)
PP(int16_t *msgbuff;)
{
	register int16_t handle;
	register WINDOW *win;
	register OBJECT *obj;
	int16_t shrink, x, y, w, h;
	GRECT pt;
	GRECT *pc;

	UNUSED(w);
	UNUSED(h);
	UNUSED(y);
	
	if (msgbuff[0] == MN_SELECTED)
	{
		hd_menu(msgbuff);
	} else
	{
		pc = (GRECT *)&msgbuff[4];				/* pc == msgbuff[4,5,6,7]   */
		handle = msgbuff[3];

		if (!(win = get_win(handle)))
			return;

		obj = win->w_obj;

		switch (msgbuff[0])
		{
		case WM_REDRAW:
		case WM_USER:
			do_redraw(handle, pc, 0);
			return;

		case WM_TOPPED:
		case WM_NEWTOP:
			clr_allwin();
			wind_set(handle, WF_TOP, 0, 0, 0, 0);
			make_top(win);
			return;
		}

		if (handle != get_top()->w_id)
			return;

		switch (msgbuff[0])
		{
		case WM_CLOSED:				/* close one path   */
			close_path(win);
			break;

		case WM_ARROWED:
			do_scroll(msgbuff);
			break;

		case WM_HSLID:
			srl_hzbar(win, msgbuff[4]);
			break;

		case WM_VSLID:
			srl_verbar(win, msgbuff[4]);
			break;

		case WM_MOVED:					/* set the current x,y,w,h  */
			do_xyfix((GRECT *)&msgbuff[4]);
			wind_set(handle, WF_CURRXYWH, msgbuff[4], msgbuff[5], msgbuff[6], msgbuff[7]);

			/* set the position offset  */
			obj[0].ob_x += msgbuff[4] - win->w_sizes.g_x;
			obj[0].ob_y += msgbuff[5] - win->w_sizes.g_y;
			rc_copy(pc, &win->w_sizes);

			wind_get(handle, WF_WORKXYWH, &msgbuff[4], &msgbuff[5], &msgbuff[6], &msgbuff[7]);

			rc_copy(pc, &win->w_work);
			break;

		case WM_FULLED:
			wind_get(handle, WF_CURRXYWH, &msgbuff[4], &msgbuff[5], &msgbuff[6], &msgbuff[7]);

			/* already in full size     */
			if (rc_equal((GRECT *)&msgbuff[4], &full))
			{							/* so shrink it         */
				wind_get(handle, WF_PREVXYWH, &msgbuff[4], &msgbuff[5], &msgbuff[6], &msgbuff[7]);
				form_dial(2, pc->g_x, pc->g_y, pc->g_w, pc->g_h, full.g_x, full.g_y, full.g_w, full.g_h);
			} else
			{
				rc_copy(&full, pc);		/* calculate the center point   */
				rc_copy(&win->w_sizes, &pt);
				rc_center(&pt, &pt);
				/* form grow box        */
				form_dial(1, pt.g_x, pt.g_y, pt.g_w, pt.g_h, pc->g_x, pc->g_y, pc->g_w, pc->g_h);
			}

		case WM_SIZED:
			do_xyfix((GRECT *)&msgbuff[4]);
			wind_set(handle, WF_CURRXYWH, msgbuff[4], msgbuff[5], msgbuff[6], msgbuff[7]);

			rc_copy(pc, &win->w_sizes);

			wind_get(handle, WF_WORKXYWH, &obj->ob_x, &obj->ob_y, &obj->ob_width, &obj->ob_height);

			shrink = FALSE;

			if (obj->ob_width < win->w_work.g_w && obj->ob_height <= win->w_work.g_h)
				shrink = TRUE;

			if (obj->ob_height < win->w_work.g_h && obj->ob_width <= win->w_work.g_w)
				shrink = TRUE;

			rc_copy((GRECT *)&obj->ob_x, &win->w_work);

			ch_path(win);

			if (shrink)
				x = win->w_xcol;

			if (s_stofit)				/* size to fit      */
				view_fixmode(win);

			view_adjust(win);			/* adjust parameter */

			if (s_stofit && shrink)
			{
				if (x != win->w_xcol)
					do_redraw(win->w_id, &full, 0);
			}
			break;

		default:
			break;
		}
	}
}


/*
 * Handle all the events
 */
/* 306de: 00e2f07c */
VOID actions(NOTHING)
{
	int16_t event, kstate, kreturn, button, clicks;
	int16_t mx, my;
	int16_t msgbuff[8];

	d_exit = L_NOEXIT;

	while (d_exit == L_NOEXIT)
	{
		menu_verify();
		q_write();						/* update inf file  */
		event = evnt_multi(MU_MESAG | MU_BUTTON | MU_KEYBD, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, msgbuff,	/* mesaage buffer   */
					0, 0,		/* timer counter    */
					&mx, &my, &button, &kstate, &kreturn, &clicks);

		wind_update(TRUE);

		if (event & MU_MESAG)
			hd_msg(msgbuff);

		if (event & MU_BUTTON)
			hd_button(clicks, kstate, mx, my);

		if (event & MU_KEYBD)
			hd_keybd(kreturn);

		wind_update(FALSE);

	}									/* while */
}


/*
 * Find out available memory
 */
/* 306de: 00e2f15c */
int32_t av_mem(NOTHING)
{
	register char *start;
	register char *addr;
	register char **new;
	int32_t total, size;

	total = 0;
	start = (char *) 0;

	while (TRUE)
	{
		size = Malloc(0xFFFFFFFFL);
		if (size >= sizeof(char *))
		{
			addr = (char *)Malloc(size);
			total += size;
			if (!start)
				start = addr;
			else
				*new = addr;

			new = (char **)addr;
		} else
		{
			if (start)
				*new = NULL;
			break;
		}
	}

	if (start)
	{
		new = (char **)start;

		while (new)
		{
			addr = *new;
			Mfree(new);
			new = (char **)addr;
		}
	}

	return total;
}


#if !POPUP_SUPPORT
/* 306de: 00e2f1ea */
VOID av_desk(NOTHING)
{
	register OBJECT *obj;
	register int32_t f;
	register int16_t fi, i, menui, which;
	register APP *app;
	int16_t len;
	APP *xapp;
	char *str;
	char temp[MAXMENU];
	OBJECT *obj1;
	char buf[2];

	obj = get_tree(SSYSTEM);
	obj1 = menu_addr;

	for (i = 0; i < MAXMENU; i++)		/* save a copy  */
		temp[i] = mentable[i];

	((TEDINFO *) (obj[MKSTR].ob_spec))->te_ptext = (char *)obj1[tb3[0]].ob_spec;
	buf[1] = 0;
	buf[0] = mentable[0];
	inf_sset(obj, MKKEY, buf);
	menui = 0;

	obj[AP1].ob_state = NORMAL;
	obj[WI1].ob_state = NORMAL;
	obj[DEFFULL].ob_state = NORMAL;
	obj[DEFFILE].ob_state = NORMAL;

	if (s_defdir)						/* set the full path    */
		obj[AP1].ob_state = SELECTED;	/* application      */
	else
		obj[WI1].ob_state = SELECTED;	/* application      */

	if (s_fullpath)
		obj[DEFFULL].ob_state = SELECTED;
	else
		obj[DEFFILE].ob_state = SELECTED;

	f_str(obj, SDMEMORY, av_mem());
	*(((TEDINFO *) (obj[SDKEY].ob_spec))->te_ptext) = '\0';

	/* save the original pointer    */
	str = ((TEDINFO *) (obj[SDFILE].ob_spec))->te_ptext;
	len = strlen(str);
	*str = 0;

	app = (APP *) 0;
	which = SDDOWN;

	f = 0;
	fm_draw(SSYSTEM);					/* save the string  */
#if !BINEXACT
	fi = 0; /* quite compiler */
#endif
	goto ad_2;

	while (TRUE)
	{
	ad_5:
		which = xform_do(obj, MKKEY);

		if (which == MKUP || which == MKDOWN || which == SDOK)
		{
			fs_sget((LPTREE)obj, MKKEY, buf);
			buf[0] = toupper(buf[0]);
			if (buf[0])
			{
				for (i = 0; i < MAXMENU; i++)
				{
					if (i == menui)
						continue;

					if (mentable[i] == buf[0])
					{
						if (do1_alert(KEYUSED) == 1)	/* Overwrite */
						{
							mentable[i] = 0;
							continue;
						}

						if (which == SDOK)
							drawfld(obj, SDOK);

						goto p_2;
					}
				}
			}

			mentable[menui] = buf[0];
		}

	ad_2:
		switch (which)
		{
		case CLRKEYS:
			for (i = 0; i < MAXMENU; i++)
				mentable[i] = 0;
			put_keys();
			goto p_11;

		case MKUP:						/* check for keys   */
			if (menui)
			{
				menui--;
				goto p_11;
			}
			break;

		case MKDOWN:
			if ((menui + 1) < MAXMENU)
			{
				menui++;
			p_11:
				ch_key(menui);
				((TEDINFO *) (obj[MKSTR].ob_spec))->te_ptext = (char *)obj1[tb3[menui]].ob_spec;
				drawfld(obj, MKSTR);
			p_2:
				buf[1] = 0;
				buf[0] = mentable[menui];
				inf_sset(obj, MKKEY, buf);
				drawfld(obj, MKKEY);
			}
			break;

		case SDCANCEL:					/* restore the menu key table */
			for (i = 0; i < MAXMENU; i++)
				mentable[i] = temp[i];
			goto ad_1;

		case SDOK:
			if (obj[AP1].ob_state & SELECTED)
				s_defdir = TRUE;
			else
				s_defdir = FALSE;

			if (obj[DEFFULL].ob_state & SELECTED)
				s_fullpath = TRUE;
			else
				s_fullpath = FALSE;
			goto ad_1;

		case SDUP:
			f--;
			if (f)
				goto ad_3;
			else
				f++;
			break;

		case SDDOWN:
			f++;
			if (f <= 20)
			{
		 ad_3:
		 		if (!(xapp = app_key(ftab[f])))
					goto ad_2;
				else
				{
					app = xapp;
					fi = 0;
					((TEDINFO *) (obj[SDFILE].ob_spec))->te_ptext = NO_CONST(app->a_name);
					drawfld(obj, SDFILE);
					f_str(obj, SDKEY, f);
					drawfld(obj, SDKEY);
				}
			} else
			{
				f--;
			}
			break;

		case SDLEFT:
		case SDRIGHT:
			if (!app)
				continue;

			if (which == SDLEFT)
			{
				if (fi > 0)
					fi--;
				else
					continue;
			} else
			{
				fi++;
				if ((int)strlen(&app->a_name[fi]) < len)
				{
					fi--;
					continue;
				}
			}

			((TEDINFO *) (obj[SDFILE].ob_spec))->te_ptext = NO_CONST(&app->a_name[fi]);
			drawfld(obj, SDFILE);
			goto ad_5;
		}

		cl_delay();
	}

ad_1:								/* restore the pointer  */
	put_keys();
	((TEDINFO *) (obj[SDFILE].ob_spec))->te_ptext = str;
	*str = 1;
	do_finish(SSYSTEM);
}
#endif
