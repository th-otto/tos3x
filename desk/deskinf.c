/*      DESKINF.C               05/04/89 - 09/18/89     D.Mui           */
/*      Read in different setting of color and pattern depends on the   */
/*      color                   6/28/90                 D.Mui           */
/*      Put in default values of color and pattern      7/2/90  D.Mui   */
/*      Changed default color   7/20/90                 D.Mui           */
/*      Save_inf returns status and move the up_allwin 8/14/91  D.Mui   */
/*      Added another 0x00 to mkeys[] for VIDITEM obj  07/07/92 C.Gee   */
/*      Change all the iconblk to ciconblk      7/11/92 D.Mui           */
/*      The #E will save 10 fields instead of 4 7/17/92 D.Mui           */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"


#if TOSVERSION >= 0x400
uint16_t d_rezword; /* also in AES:geminit.c */
#endif
char afile[INFSIZE];
BOOLEAN font_save;
BOOLEAN s_defdir;
BOOLEAN s_fullpath;

STATIC WINDOW *warray[MAXWIN];			/* window structure */ /* should be local to save_inf */

#define SAVE_ATARI	128


/*
 * Default keystroke
 */
/* Added another 0x00 to the end for VIDITEM object - cjg 07/07/92 */
/* Take out two for sparrow */
static char const mkeys[MAXMENU] = {
	0x4F, 0x53, 0x4C, 0x00, 0x46, 0x42, 0x43, 0x57,
	0x45, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52,
	0x00, 0x00, 0x4D, 0x56, 0x50
};

char uhex_dig PROTO((int16_t wd));
const char *inf_xdesk PROTO((const char *pcurr));
const char *inf_parse PROTO((const char *pcurr));
VOID inf_scan PROTO((char *buffer));
int16_t hex_dig PROTO((char achar));
char *save_win PROTO((WINDOW *win, char *pcurr));
VOID tt_color PROTO((BOOLEAN put));



/*
 * Make inf path
 */
/* 306de: 00e2b580 */
VOID m_infpath(P(char *)buffer)
PP(char *buffer;)
{
	strcpy(buffer, infpath);			/* C:\NEWDESK.INF */
	buffer[0] = (isdrive() & 0x04) ? 'C' : 'A';
}


/*
 * Reverse of hex_dig()
 */
/* 306de: 00e2b5b4 */
char uhex_dig(P(int16_t) wd)
PP(register int16_t wd;)
{
	if (wd >= 0 && wd <= 9)
		return wd + '0';

	if (wd >= 0x0a && wd <= 0x0f)
		return wd + 'A' - 0x0a;

	return ' ';
}


/* 306de: 00e2b5f2 */
char *escan_str(P(const char *)pcurr, P(char *)ppstr)
PP(register const char *pcurr;)
PP(register char *ppstr;)
{
	while (*pcurr == ' ')
		pcurr++;

	while (*pcurr != '@' && *pcurr)
		*ppstr++ = *pcurr++;

	*ppstr = 0;
	pcurr++;
	return NO_CONST(pcurr);
}


/*
 * Scan off and convert the next two hex digits and return with
 * pcurr pointing one space past the end of the four hex digits
 */
/* 306de: 00e2b62a */
char *scan_2(P(const char *) pcurr, P(int16_t *) pwd)
PP(register const char *pcurr;)
PP(register int16_t *pwd;)
{
	register uint16_t temp;

	temp = 0x0;
	temp |= hex_dig(*pcurr++) << 4;
	temp |= hex_dig(*pcurr++);
	if (temp == 0x00ff)
		temp = -1;
	*pwd = temp;
	pcurr++;
	return NO_CONST(pcurr);
}


/*
 * Reverse of scan_2()
 */
/* 306de: 00e2b66e */
char *save_2(P(char *) pcurr, P(uint16_t) wd)
PP(register char *pcurr;)
PP(uint16_t wd;)
{
	*pcurr++ = uhex_dig((wd >> 4) & 0x000f);
	*pcurr++ = uhex_dig(wd & 0x000f);
	*pcurr++ = ' ';
	return pcurr;
}


/*
 * Reverse of scan_str
 */
/* 306de: 00e2b6ac */
char *save_sstr(P(char *)pcurr, P(const char *)pstr)
PP(register char *pcurr;)
PP(register const char *pstr;)
{
	while (*pstr && pstr) /* BUG: && pstr useless when first acessing it */
		*pcurr++ = *pstr++;
	*pcurr++ = '@';
	*pcurr++ = ' ';
	return pcurr;
}


/*
 * Scan the desktop icon
 */
/* 306de: 00e2b6dc */
const char *inf_xdesk(P(const char *)pcurr)
PP(register const char *pcurr;)
{
	register int16_t ix;
	register int16_t id;
	register CICONBLK *iblk;
	OBJECT *obj;
	int16_t x, y, i, type;
	char buffer[NAMELEN];

	type = *pcurr++;

	switch (type)
	{
	case 'C':							/* cartridge    */
		if (!cart_init())
			return pcurr;

		ix = DISK;
		break;
	case 'M':							/* No drive?    */
		if (!(x = isdrive()))
			return pcurr;

		if (pcurr[13] == 'C' && !(x & 0x04))
			return pcurr;

		ix = DISK;						/* disk drive   */
		break;
	case 'T':
		ix = TRASHCAN;					/* trashcan */
		break;
	case 'X':
		ix = XFILE;						/* desktop file */
		break;
	case 'V':
		ix = XDIR;						/* desktop file */
		break;
	case 'O':
		ix = PRINTER;
		break;
	default:							/* illegal type */
		return pcurr;
	}

	if ((id = av_icon()) == -1)			/* allocate a desktop icon  */
		return pcurr;

	backid[id].i_type = ix;

	obj = background;

	pcurr++;
	pcurr = scan_2(pcurr, &x);			/* icon x       */
	pcurr = scan_2(pcurr, &y);			/* icon y       */

	app_posicon(x, y, &obj[id].ob_x, &obj[id].ob_y);

	pcurr = scan_2(pcurr, &i);			/* get the icon id  */
#if 0
	if (i >= numicon)
		i = numicon - 1;
#endif

	backid[id].i_icon = i;

	iblk = (CICONBLK *) (background[id].ob_spec);

#if COLORICON_SUPPORT
	cp_iblk(i, iblk);
#else
	cp_iblk(get_icon(i), &iblk->monoblk);
#endif

	pcurr += 3;
	iblk->monoblk.ib_char[1] = *pcurr == ' ' ? 0 : *pcurr;
	pcurr += 2;

	/* for extended file or folder */
	if (type == 'X' || type == 'V')
	{
		pcurr = lp_fill(pcurr, &backid[id].i_path);
		pcurr = escan_str(pcurr, iblk->monoblk.ib_ptext);
		if (!*((char *) (iblk->monoblk.ib_ptext)))
		{
			if (type == 'X')
				save_ext(backid[id].i_path, iblk->monoblk.ib_ptext);
			else
				save_mid(NO_CONST(backid[id].i_path), iblk->monoblk.ib_ptext);
		}
	} else
	{
		pcurr = escan_str(pcurr, iblk->monoblk.ib_ptext);
		pcurr = escan_str(pcurr, buffer);
	}

	return pcurr;
}



/*
 * Parse a single line from the DESKTOP.APP file.
 * Just scan the application
 */
/* 306de: 00e2b8fe */
const char *inf_parse(P(const char *)pcurr)
PP(register const char *pcurr;)
{
	register APP *app;
	register int16_t type;

	pcurr -= 2;							/* important    */

	switch (*pcurr++)
	{
	case 'Y':
		type = PTP;						/* prg takes parameter  */
		break;
	case 'G':							/* GEM App File     */
		type = PRG;
		break;
	case 'F':							/* DOS File no parms    */
		type = TOS;
		break;
	case 'P':							/* DOS App needs parms  */
		type = TTP;
		break;
	case 'D':							/* folder subdir    */
		type = FOLDER;
		break;
	case 'N':							/* text file        */
		type = TEXT;
		break;
	case 'I':							/* icon         */
		type = ICONS;
		break;
	default:
		return pcurr;
	}

	if (!(app = app_alloc()))			/* allocate a app   */
		return pcurr;

	app->a_type = type;					/* prg takes parameter  */

	pcurr++;
	pcurr = scan_2(pcurr, &app->a_icon);	/* get the icon number  */
	pcurr = scan_2(pcurr, &app->a_dicon);

	if (*pcurr != ' ')					/* default definiton    */
	{
		app->a_pref = hex_dig(*pcurr);

		if (*(pcurr + 1) != ' ')		/* function key     */
		{
			pcurr++;
			scan_2(pcurr++, (int16_t *)&app->a_key);
		}
	}

	pcurr += 2;

	pcurr = lp_fill(pcurr, &app->a_name);
	pcurr = escan_str(pcurr, app->a_doc);
	pcurr++;

	if (*pcurr != 0x0d)
		pcurr = escan_str(pcurr, app->a_argu);

	/********** Kludge **********/
	/* make the old inf works with the new one  */
	if (app->a_type == TOS && !app->a_name[0])
		app->a_type = TEXT;

	/***************************/
	return pcurr;
}


/*
 *	Initialize the application list by reading in the DESKTOP.APP
 *	file, either from memory or from the disk if the shel_get
 *	indicates no message is there.
 */

/*
 * A horrible hack: Falcon TOS has a different menu setup than
 * everyone else (including MultiTOS). In order to maintain
 * compatibility, we re-arrange things when reading/writing
 * .inf files. The table below gives the mapping from MultiTOS
 * to Falcon TOS.
 * ++ERS 1/8/93
 */

#if TOSVERSION >= 0x400
static int16_t const inf_permute[MAXMENU + 2] = {
	0, 1, 2, 3, 4,
	5, 6, 7, 8, 9,
	10, 11, 12, 13, 14,
	15, 16, 17, 18, 19,
	20, 21, 22, 23, 24,
	26, 27, 28, -1, -1,
	25
};
#endif

/* 306de: 00e2ba0c */
VOID inf_scan(P(char *)buffer)
PP(char *buffer;)
{
	register int16_t i, tmp;
	register WINDOW *pws;
	register const char *pcurr;
	APP *app;
	char *ptmp;
	int32_t unused1;
	int16_t envr;
	int16_t j;
	int32_t stmp;
	char temp;
	char *ptr;

	UNUSED(unused1);
	UNUSED(stmp);
	UNUSED(ptmp);
	UNUSED(tmp);
	UNUSED(app);
	
	i = 0;								/* for window index */
	pcurr = buffer;
	font_save = FALSE;

	/* put in default keystroke */
	for (j = 0; j < MAXMENU; j++)
		mentable[j] = mkeys[j];

	while (*pcurr)						/* 0 is marked as end of buffer */
	{
		if (*pcurr++ == '#')
		{
			temp = *pcurr;
			pcurr += 2;
			switch (temp)
			{
			case 'K':
				/* clean up keyboard table  */
				for (j = 0; j < MAXMENU; j++)
					mentable[j] = 0;
				/* key board table  */
				/* read in key equivalent */
				/* use MAXMENU+2 instead of MAXMENU for compatibility with MultiTOS
				 * and other TOS's; that's also why we need to permute things
				 * (see table above)
				 */
#if TOSVERSION >= 0x400
				for (j = 0; j < MAXMENU + 2; j++)
				{
					if (*pcurr == '@')
						break;

					pcurr = scan_2(pcurr, &envr);
					if (inf_permute[j] >= 0)
						mentable[inf_permute[j]] = envr;
				}
#else
				for (j = 0; j < MAXMENU; j++)
				{
					if (*pcurr == '@')
						break;

					pcurr = scan_2(pcurr, &envr);
					mentable[j] = envr;
				}
#endif
				break;

			case 'Z':					/* auto boot file   */
				pcurr = escan_str(pcurr, autofile);
				break;

			case 'C':
			case 'X':
			case 'V':
			case 'M':
			case 'T':
			case 'O':
				pcurr -= 2;
				pcurr = inf_xdesk(pcurr);
				break;

			case 'W':					/* Window       */
				if (i >= MAXWIN)
					break;

				if (!winpd[i].w_free)
					break;

				pws = &winpd[i++];		/* horizontal slide bar */
				pcurr = scan_2(pcurr, &pws->w_coli);
				/* vertical slide bar   */
				pcurr = scan_2(pcurr, &pws->w_rowi);

				/* window's x position */
				pcurr = scan_2(pcurr, &pws->w_sizes.g_x);
				if (pws->w_sizes.g_x >= gl_ncols)
#if TOSVERSION >= 0x400
					pws->w_sizes.g_x = gl_ncols - 4;
#else
					pws->w_sizes.g_x /= 2;
#endif

				pws->w_sizes.g_x *= gl_wchar;

				/* window's y position */
				pcurr = scan_2(pcurr, &pws->w_sizes.g_y);
				if (pws->w_sizes.g_y >= gl_nrows)
#if TOSVERSION >= 0x400
					pws->w_sizes.g_y = gl_nrows - 1;
#else
					pws->w_sizes.g_y /= 2;
#endif

				pws->w_sizes.g_y *= gl_hchar;

				/* window's width */
				pcurr = scan_2(pcurr, &pws->w_sizes.g_w);
				if (pws->w_sizes.g_w > gl_ncols)
					pws->w_sizes.g_w /= 2;
				pws->w_sizes.g_w *= gl_wchar;
				if (pws->w_sizes.g_w < (7 * gl_wbox))
					pws->w_sizes.g_w = 7 * gl_wbox;

				/* window's height  */
				pcurr = scan_2(pcurr, &pws->w_sizes.g_h);
				if (pws->w_sizes.g_h >= gl_nrows)
					pws->w_sizes.g_h /= 2;
				pws->w_sizes.g_h *= gl_hchar;
				if (pws->w_sizes.g_h < (7 * gl_hbox))
					pws->w_sizes.g_h = 7 * gl_hbox;

				do_xyfix(&pws->w_sizes);
				rc_copy(&pws->w_sizes, &pws->w_work);

				pcurr = scan_2(pcurr, &envr);

				ptr = scasb(pcurr, '@');
				*ptr = 0;				/* take out the @ */

				if (!c_path_alloc(pcurr))
				{
					i--;
					goto i_12;
				}

				*ptr = '@';
				pcurr = escan_str(pcurr, pws->w_path);
			i_12:
				*ptr = '@';
				break;

			case 'Q':					/* Set window and desk color pattern    */
				for (j = 0; j < 3; j++)
				{
					pcurr = scan_2(pcurr, &deskp[j]);
					pcurr = scan_2(pcurr, &winp[j]);
				}
				tt_color(TRUE);
				break;

			case 'E':					/* environment string   */
				pcurr = scan_2(pcurr, &envr);
				s_view = (envr & 0x80) != 0;
				s_sort = (envr & 0x60) >> 5;
				cdele_save = (envr & 0x10) != 0;
				ccopy_save = (envr & 0x08) != 0;
				write_save = envr & 0x01;
				pcurr = scan_2(pcurr, &envr);
				cbit_save = (envr & 0x10) ? TRUE : FALSE;
				pref_save = envr & 0x0F;	/* screen resoultion    */

				/* This is the extended stuff   */

				if (*pcurr == 0x0d)		/* if it is the end */
					break;				/* then skip        */

				pcurr = scan_2(pcurr, &envr);
				font_save = (envr & 0x0F) ? TRUE : FALSE;

				pcurr = scan_2(pcurr, &envr);

				if (envr & 0x01)
					s_sort = S_NO;

				s_cache = (envr & 0x02) ? TRUE : FALSE;
				s_stofit = (envr & 0x04) ? TRUE : FALSE;
				s_defdir = (envr & 0x08) ? TRUE : FALSE;
				s_fullpath = (envr & 0x10) ? TRUE : FALSE;
				break;

			default:					/* must be the last one */
				pcurr = inf_parse(pcurr);
				break;
			}
		}
	}
}


/*
 * Read in a desktop.inf file and parse the string
 */
/* 306de: 00e2beac */
VOID read_inf(NOTHING)
{
	int16_t handle;
	register int32_t size1;
	char buffer[20];
	register APP *app;

	shel_get(afile, INFSIZE);

	if (afile[0] != '#')
	{
		bfill(INFSIZE, 0, afile);
		if (isdrive())
		{
			m_infpath(buffer);			/* open newdesk.inf */
			if ((handle = Fopen(buffer, 0)) < 0)
			{							/* try desktop.inf  */
				strcpy(&buffer[3], infdata);
				if ((handle = Fopen(buffer, 0)) < 0)
					goto re_1;
			}

			size1 = Fread(handle, (int32_t) INFSIZE, afile);
			if (size1 == INFSIZE)		/* buffer full  */
				size1--;

			Fclose(handle);
		} else
		{
		re_1:
#if BINEXACT
			size1 = rom_ram(3, (intptr_t)afile, 0); /* BUG: extra parameter */
#else
			size1 = rom_ram(3, (intptr_t)afile);
#endif
		}

		afile[size1] = 0;
	}

	shel_put(afile, INFSIZE);			/* copy to the aes buffer   */

	/* init default desktop and window color and pattern values */
	/* if you change these, change the ones in geminit.c, too   */

#if (AESVERSION >= 0x330) | !BINEXACT
	deskp[0] = (IP_4PATT << 4) | BLACK;
	deskp[1] = (IP_SOLID << 4) | GREEN;
	deskp[2] = (IP_SOLID << 4) | LCYAN;
	winp[0] = (IP_SOLID << 4) | WHITE;
	winp[1] = (IP_SOLID << 4) | WHITE;
	winp[2] = (IP_SOLID << 4) | WHITE;
#else
	deskp[0] = (IP_4PATT << 4) | BLACK;
	deskp[1] = (IP_4PATT << 4) | GREEN;
	deskp[2] = (IP_4PATT << 4) | GREEN;
	winp[0] = (IP_4PATT << 4) | WHITE;
	winp[1] = (IP_4PATT << 4) | WHITE;
	winp[2] = (IP_4PATT << 4) | WHITE;
#endif

	tt_color(TRUE);						/* stuff color into system var  */

	inf_scan(afile);					/* analyze the inf file     */

	app = applist;

	while (app)
	{
		if (app->a_type == PTP)
		{
			if (streq(app->a_name, "*.GTP"))
				return;
		}

		app = app->a_next;
	}

	if ((app = app_alloc()) != NULL)
	{
		app->a_type = PTP;
		app->a_key = 0;
		app->a_icon = 3;
		app->a_dicon = 4;
		lp_fill("*.GTP", &app->a_name);
	}
}


/*
 *	Convert a single hex ASCII digit to a number
 */
/* 306de: 00e2c064 */
int16_t hex_dig(P(char) achar)
PP(register char achar;)
{
	if (achar >= '0' && achar <= '9')
		return achar - '0';

	if (achar >= 'A' && achar <= 'F')
		return achar - 'A' + 10;

	return 0;
}


/* 306de: 00e2c0a8 */
char *save_win(P(WINDOW *)win, P(char *)pcurr)
PP(register WINDOW *win;)
PP(register char *pcurr;)
{
	char *ptmp;

	*pcurr++ = '#';
	*pcurr++ = 'W';
	*pcurr++ = ' ';

	pcurr = save_2(pcurr, win->w_coli);	/* horizontal slide bar  */
	pcurr = save_2(pcurr, win->w_rowi);
	pcurr = save_2(pcurr, win->w_sizes.g_x / gl_wchar);
	pcurr = save_2(pcurr, win->w_sizes.g_y / gl_hchar);
	pcurr = save_2(pcurr, win->w_sizes.g_w / gl_wchar);
	pcurr = save_2(pcurr, win->w_sizes.g_h / gl_hchar);
	pcurr = save_2(pcurr, 0);

	if (win->w_id != -1)
	{
		ptmp = win->w_path;
		while (*ptmp)
			*pcurr++ = *ptmp++;
	}

	*pcurr++ = '@';
	*pcurr++ = 0x0d;
	*pcurr++ = 0x0a;

	return pcurr;
}


/*
 * Save a desktop.inf file
 */
/* 306de: 00e2c186 */
BOOLEAN save_inf(P(BOOLEAN) todisk)
PP(BOOLEAN todisk;)
{
	register APP *start;
	register WINDOW *win;
	register char *pcurr;
	register int16_t envr, i;
	int16_t j, w, len, h, handle;
	char *buf;
	APP *app;
	OBJECT *obj;
	int32_t size;
	char infname[16];
	char buf1[2];

	if ((size = Malloc(0xFFFFFFFFL)))		/* get some memory  */
	{
		if (size < INFSIZE)
			goto if_1;

		buf = pcurr = (char *)Malloc(size);
	} else
	{
	if_1:
		do1_alert(FCNOMEM);
		return FALSE;
	}

	desk_wait(TRUE);

	bfill(INFSIZE, 0, afile);			/* clean up buffer  */

	shel_get(buf, SAVE_ATARI);			/* get the control panel stuff  */

	pcurr += SAVE_ATARI;

	if (autofile[0])
	{
		*pcurr++ = '#';
		*pcurr++ = 'Z';
		*pcurr++ = ' ';
		pcurr = save_sstr(pcurr, autofile);
		*pcurr++ = 0x0d;
		*pcurr++ = 0x0a;
	}
	/* save keyboard    */
	*pcurr++ = '#';
	*pcurr++ = 'K';
	*pcurr++ = ' ';

	/* Falcon TOS is different from all other TOSes in its menu structure
	 * so we permute when writing out the .INF and use MAXMENU+2 instead
	 * of MAXMENU
	 */
#if TOSVERSION >= 0x400
	for (i = 0; i < MAXMENU + 2; i++)
	{
		if (inf_permute[i] < 0)
			pcurr = save_2(pcurr, 0);
		else
			pcurr = save_2(pcurr, mentable[inf_permute[i]]);
	}
#else
	for (i = 0; i < MAXMENU; i++)
	{
		pcurr = save_2(pcurr, mentable[i]);
	}
#endif
	*pcurr++ = '@';
	*pcurr++ = 0x0d;
	*pcurr++ = 0x0a;

	/* save evironment */
	*pcurr++ = '#';
	*pcurr++ = 'E';
	*pcurr++ = ' ';
	envr = 0x0;
	envr |= write_save;
	envr |= s_view ? 0x80 : 0x00;
	envr |= (s_sort << 5) & 0x60;
	envr |= cdele_save ? 0x10 : 0x00;
	envr |= ccopy_save ? 0x08 : 0x00;
	pcurr = save_2(pcurr, envr);

	envr = 0x0;							/* set resolution prefence  */
	envr |= pref_save;
	envr |= (cbit_save << 4);			/* High order bit       */
	pcurr = save_2(pcurr, envr);

	pcurr = save_2(pcurr, font_save);

	envr = 0x0;
	if (s_sort == S_NO)
		envr |= 0x01;

	if (s_cache)
		envr |= 0x02;

	if (s_stofit)
		envr |= 0x04;

	if (s_defdir)
		envr |= 0x08;

	if (s_fullpath)
		envr |= 0x10;

	pcurr = save_2(pcurr, envr);		/* save the settings    */

	/* 7/17/92  */

#if TOSVERSION >= 0x400
	pcurr = save_2(pcurr, (d_rezword >> 8));
	pcurr = save_2(pcurr, (d_rezword & 0x00FF));
	pcurr = save_2(pcurr, 0);
	pcurr = save_2(pcurr, 0);
	pcurr = save_2(pcurr, 0);
	pcurr = save_2(pcurr, 0);
#endif

	*pcurr++ = 0x0d;
	*pcurr++ = 0x0a;

	/* save the window and desk spec    */

	*pcurr++ = '#';
	*pcurr++ = 'Q';
	*pcurr++ = ' ';

	tt_color(FALSE);

	for (i = 0; i < 3; i++)
	{
		pcurr = save_2(pcurr, deskp[i]);	/* save the desktop backgrounds */
		pcurr = save_2(pcurr, winp[i]);
	}

	*pcurr++ = 0x0d;
	*pcurr++ = 0x0a;

	/********* save the opened window first	************/

	win = w_gfirst();

	i = -1;

	while (win)
	{
		i++;
		warray[i] = win;
		win = w_gnext();
	}
	/* save windows */
	for (j = i; j >= 0; j--)
		pcurr = save_win(warray[j], pcurr);

	/*******************************************************/

	win = winhead;

	while (win)
	{
		if (win->w_id == -1)
			pcurr = save_win(win, pcurr);

		win = win->w_next;
	}


	start = applist;					/* search the last one  */

	while (start)
	{
		if (!start->a_next)
			break;

		start = start->a_next;
	}

	app = start;

	while (app)
	{
		*pcurr++ = '#';

		switch (app->a_type)
		{
		case PTP:
			*pcurr++ = 'Y';
			break;
		case TTP:
			*pcurr++ = 'P';
			break;
		case PRG:
			*pcurr++ = 'G';
			break;
		case TOS:
			*pcurr++ = 'F';
			break;
		case FOLDER:
			*pcurr++ = 'D';
			break;
		case TEXT:
			*pcurr++ = 'N';
			break;
		case ICONS:
			*pcurr++ = 'I';
			break;
		}

		*pcurr++ = ' ';

		pcurr = save_2(pcurr, app->a_icon);
		pcurr = save_2(pcurr, app->a_dicon);

		*pcurr++ = uhex_dig(app->a_pref);
		pcurr = save_2(pcurr, app->a_key);

		pcurr = save_sstr(pcurr, app->a_name);
		pcurr = save_sstr(pcurr, app->a_doc);
		pcurr = save_sstr(pcurr, app->a_argu);
		*pcurr++ = 0x0d;
		*pcurr++ = 0x0a;

		if (app == applist)
			break;

		start = applist;

		while (start)
		{
			if (start->a_next == app)
			{
				app = start;
				break;
			}
			start = start->a_next;
		}
	}

	/* save the desktop disk drive  */

	obj = background;

	w = r_dicon.g_w;
	h = r_dicon.g_h;

	for (i = 1; i <= maxicon; i++)
	{
		if (obj[i].ob_flags & HIDETREE)
			continue;

		*pcurr++ = '#';

		switch (backid[i].i_type)
		{
		case PRINTER:
			*pcurr++ = 'O';
			break;
		case DISK:
#if COLORICON_SUPPORT
			if (backid[i].i_cicon.monoblk.ib_char[1] == CHAR_FOR_CARTRIDGE)
#else
			if (backid[i].i_iblk.ib_char[1] == CHAR_FOR_CARTRIDGE)
#endif
				*pcurr++ = 'C';
			else
				*pcurr++ = 'M';
			break;
		case XFILE:					/* file on desktop  */
			*pcurr++ = 'X';
			break;
		case XDIR:					/* DIR on desktop   */
			*pcurr++ = 'V';
			break;
		case TRASHCAN:
			*pcurr++ = 'T';
			break;
		}

		*pcurr++ = ' ';

		/* save the position     */

		pcurr = save_2(pcurr, obj[i].ob_x / w);
		pcurr = save_2(pcurr, obj[i].ob_y / h);
		pcurr = save_2(pcurr, backid[i].i_icon);
		pcurr = save_2(pcurr, 0xFFFF);
		*pcurr = ((CICONBLK *) (obj[i].ob_spec))->monoblk.ib_char[1];
		if (!*pcurr)
			*pcurr = ' ';

		pcurr++;

		*pcurr++ = ' ';

		if (backid[i].i_path)
			pcurr = save_sstr(pcurr, backid[i].i_path);

		pcurr = save_sstr(pcurr, ((CICONBLK *) (obj[i].ob_spec))->monoblk.ib_ptext);

		if (!backid[i].i_path)
			pcurr = save_sstr(pcurr, Nostr);

		/* pcurr = save_sstr( pcurr, Nostr); */
		*pcurr++ = 0x0d;
		*pcurr++ = 0x0a;
	}

	*pcurr = 0;

	len = strlen(buf);

	if (len > INFSIZE)
	{
		do1_alert(FCNOMEM);
		goto if_2;
	}

	strcpy(afile, buf);					/* copy to my buffer    */

	shel_put(afile, INFSIZE);			/* copy to the aes buffer   */

	if (todisk)
	{
		m_infpath(infname);				/* always write newdesk.inf */
	if_3:								/* create a file, read & write  */
		handle = Fcreate(infname, 0x0);

		if (handle < 0)
		{
			if (fill_string(infname, CNTCRTFL) == 2)
				goto if_3;				/* repeat   */
			else
				goto if_2;
		}

		size = Fwrite(handle, (int32_t) len, afile);
		Fclose(handle);
		/* check for full disk */
		if (size < (int32_t) len)
		{
			buf1[0] = infname[0];
			buf1[1] = 0;
			fill_string(buf1, STDISKFULL);
			Fdelete(infname);
		}

#if 0
		up_allwin(infname, FALSE);	/* rebuild any window on the INF drive */
#endif

		/* update the buffer */
		if (streq(infname, inf_path))
		{
			q_change = FALSE;
			strcpy(q_addr, afile);
		}

	}
	/* todisk */
  if_2:
	Mfree(buf);

	if (todisk)
		up_allwin(infname, FALSE);		/* rebuild any window on the INF drive */

	desk_wait(FALSE);
	return TRUE;
}


/*
 * Position the desktop icon
 */
/* 306de: 00e2c86c */
VOID app_posicon(P(int16_t) colx, P(int16_t) coly, P(int16_t *)px, P(int16_t *)py)
PP(int16_t colx;)
PP(int16_t coly;)
PP(register int16_t *px;)
PP(register int16_t *py;)
{
	register int16_t x, y, w, h;

	w = r_dicon.g_w;
	h = r_dicon.g_h;
	x = colx * w + full.g_x;
	y = coly * h + full.g_y;
	app_mtoi(x, y, px, py);
}


/*
 * Transform mouse position into icon position
 */
/* 306de: 00e2c8b6 */
VOID app_mtoi(P(int16_t) newx, P(int16_t) newy, P(int16_t *)px, P(int16_t *)py)
PP(int16_t newx;)
PP(int16_t newy;)
PP(register int16_t *px;)
PP(register int16_t *py;)
{
	register int16_t x, y, w, h;
	int16_t xm, ym;
	int16_t maxx, maxy;

	w = r_dicon.g_w;
	h = r_dicon.g_h;
	/* maximum x coordinate */
	maxx = (full.g_w - dicon.g_w) / w;

	x = newx / w;
	xm = newx % w;
	if (xm > (w / 2))
		x += 1;

	if (x > maxx)
		x = maxx;

	*px = x * w;						/* new x coordinate */

	if (newy < full.g_y)
		newy = full.g_y;

	maxy = (full.g_h - dicon.g_h) / h;

	newy -= full.g_y;

	y = newy / h;
	ym = newy % h;
	if (ym > (h / 2))
		y += 1;

	if (y > maxy)
		y = maxy;

	*py = y * h;
}


/*
 * Put or get color and pattern
 */
/* 306de: 00e2c98e */
VOID tt_color(P(BOOLEAN) put)
PP(BOOLEAN put;)
{
	int16_t j;

	j = 2;								/* assume more than 4 colors */

	if (d_maxcolor == 2)
		j = 0;

	if (d_maxcolor == 4)
		j = 1;

	if (put)
	{
		windspec = winp[j];
		background[0].ob_spec = (uint16_t) deskp[j];
	} else
	{
		deskp[j] = background[0].ob_spec;
		winp[j] = windspec;
	}
}
