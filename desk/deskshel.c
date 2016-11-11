/*      DESKSHELL.C             4/24/89 - 6/15/89       Derek Mui       */
/*      Take out vdi_handle     6/28/89                                 */
/*      Modify exec_file, run_it        9/13/89         D.Mui           */
/*      Check undo key during printing  9/26/89         D.Mui           */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"

BOOLEAN xch_tail PROTO((const char *ptr, const char *argu, char *tail));
VOID show_file PROTO((const char *file));
VOID pr_setup PROTO((NOTHING));
BOOLEAN printit PROTO((const char *str));


/* 306de: 00e32994 */
BOOLEAN xch_tail(P(const char *)ptr, P(const char *)argu, P(char *)tail)
PP(const char *ptr;)
PP(const char *argu;)
PP(char *tail;)
{
	if ((int)(strlen(ptr) + strlen(argu) + 1) >= PATHLEN)
	{
		do1_alert(FCNOMEM);
		return FALSE;
	} else
	{
		strcpy(&tail[1], argu);			/* put in first argus   */
		strcat(&tail[1], ptr);			/* patch in file name   */
		tail[0] = strlen(&tail[1]);
		return TRUE;
	}
}


/* 306de: 00e32a0a */
BOOLEAN ch_tail(P(const char *)ptr, P(char *)tail)
PP(const char *ptr;)
PP(char *tail;)
{
	return xch_tail(ptr, Nostr, tail);
}


/* 306de: 00e32a26 */
VOID show_file(P(const char *)file)
PP(const char *file;)
{
	menu_bar(menu_addr, FALSE);
	dv_hide_c();
	dv_enter_cur();
	xvq_chcells(&d_nrows);
	d_nrows--;
	showfile(file, FALSE);
	dv_exit_cur();
	dv_show_c(0);
	menu_bar(menu_addr, TRUE);
	form_dial(FMD_FINISH, 0, 0, 0, 0, full.g_x, full.g_y, full.g_w, full.g_h);
	wait_msg();
}


/* 306de: 00e32ac0 */
VOID pr_setup(NOTHING)
{
	inf_sset(get_tree(PRINTFIL), PFILE, Nostr);
	fm_draw(PRINTFIL);
}


/*
 * Print one single file
 */
/* 306de: 00e32af2 */
BOOLEAN printit(P(const char *)str)
PP(const char *str;)
{
	char *ptr;
	register OBJECT *obj;

	UNUSED(ptr);
	obj = get_tree(PRINTFIL);
	xinf_sset(obj, PFILE, g_name(str));
	draw_fld(obj, PFILE);
	return showfile(str, TRUE);
}


/*
 * Loop for print files
 */
/* 306de: 00e32b48 */
VOID print_file(NOTHING)
{
	BOOLEAN ret;
	int16_t type;
	BOOLEAN print, ret1;
	const char *str;
	GRECT pt;

	ret1 = build_rect(background, &pt, d_xywh[6], d_xywh[9]);

	ret = x_first(&str, &type);			/* get the first file   */
	print = FALSE;

	desk_wait(TRUE);

	while (ret)
	{
		if ((type == XFILE) || (type == WINICON))
		{
			if (!print)
			{
				print = TRUE;
				pr_setup();
			}

			if (!printit(str))
				break;
		}

		x_del();						/* deselecte the file   */

		if (!ch_undo())
			break;

		ret = x_next(&str, &type);
	}

	if (print)
		do_finish(PRINTFIL);

	if (x_type == DESKICON)				/* update the desktop   */
	{
		if (ret1)
			do_redraw(0, &pt, 0);
	} else
	{
		up_2(x_win);
	}
	
	desk_wait(FALSE);
}


/*
 * Launch a file
 */
/* 306de: 00e32c44 */
VOID launch_pref(NOTHING)
{
	BOOLEAN graphic;
	register OBJECT *obj;
	char *ptr;

	obj = get_tree(DLAUNCH);
	inf_sset(obj, RUNNAME, Nostr);
	if (fmdodraw(DLAUNCH, 0) == LAUNCHOK)
	{
		fs_sget((LPTREE)obj, RUNNAME, path1);
		if (path1[0])
		{
			graphic = (obj[LGRAPHIC].ob_state & SELECTED) ? TRUE : FALSE;
			ptr = scasb(path1, ' ');
			if (*ptr)					/* point to the blank   */
			{
				*ptr++ = 0;
				if (!ch_tail(ptr, comtail))
					return;
			} else
			{
				comtail[0] = 0;
			}
			
			if (path1[1] == ':')
				path1[0] = toupper(path1[0]);

			run_it(path1, comtail, graphic, s_defdir);
		}
	}
}


/*
 * Set the current directory
 * The path should look like A:\FILENAME or A:\FOLDER\FILENAME
 */
/* 306de: 00e32d40 */
BOOLEAN set_dir(P(const char *) path)
PP(const char *path;)
{
	register char *ptr;
	register int16_t ret;

	if (path[0] == CHAR_FOR_CARTRIDGE)
		return TRUE;

	ret = TRUE;

	if (path[0] == '\\')				/* at the current root  */
	{
		ret = Dsetpath(bckslsh);
	} else
	{									/* there is drive id    */
		if (path[1] == ':')
		{
			if (hit_disk((int16_t) path[0]))
			{
				Dsetdrv((int16_t) (toupper(path[0]) - 'A'));
				ptr = r_slash(path);
				if (&path[2] == ptr)
					ret = Dsetpath(bckslsh);
				else
				{
					*ptr = 0;
					ret = Dsetpath(&path[2]);
					*ptr = '\\';
				}
			}
		} else							/* only file name !!!   */
		{
			ret = 0;					/* OK           */
		}
	}

	return ret ? FALSE : TRUE;
}


/*
 * Run an application include doing dialogue box
 */
/* 306de: 00e32e1e */
VOID exec_file(P(const char *)infile, P(WINDOW *)win, P(int16_t) item, P(const char *)intail)
PP(const char *infile;)
PP(WINDOW *win;)
PP(int16_t item;)
PP(const char *intail;)
{
	int16_t type, install;
	BOOLEAN graphic;
	BOOLEAN dofull, setdir;
	int16_t which;
	register APP *app;
	OBJECT *obj;
	char buffer[NAMELEN];
	const char *tail;
	const char *file;

	UNUSED(buffer);
	app = app_xtype(infile, &install);
	type = app->a_type;
	/* installed document   */
	if ((install) && (app->a_name[0]))
	{
		tail = infile;					/* infile becomes tail  */
		file = app->a_name;
	} else
	{
		tail = intail[0] ? &intail[1] : Nostr;
		file = infile;
	}

	if (streq(file, app->a_name))
	{
		dofull = (app->a_pref & 0x2) ? TRUE : FALSE;
		setdir = (app->a_pref & 0x1) ? TRUE : FALSE;
	} else
	{
		dofull = s_fullpath;
		setdir = s_defdir;
	}

	if (!dofull)						/* file name only   */
		tail = g_name(tail);

	if (!xch_tail(tail, app->a_argu, comtail))
		return;

	graphic = TRUE;

	switch (type)
	{
	case TEXT:							/* print    */
		if ((which = do1_alert(STNOAPPL)) == 2)
		{
			desk_wait(TRUE);
			pr_setup();
			printit(file);
			desk_wait(FALSE);
			do_finish(PRINTFIL);
		} else if (which == 1)			/* show     */
		{
			show_file(file);
		}
		return;

	case TTP:							/* TOS takes parameter  */
		graphic = FALSE;
		/* fall through */
		
	case PTP:							/* program takes parameter  */
		if ((install) || (*tail))
			break;

		obj = get_tree(ADOPENAP);
		tail = g_name(file);
		xinf_sset(obj, APPLNAME, tail);
		inf_sset(obj, APPLPARM, app->a_argu);

		if (fmdodraw(ADOPENAP, 0) == APPLCNCL)
			return;

		tail = ((TEDINFO *) (obj[APPLPARM].ob_spec))->te_ptext;

		if (!ch_tail(tail, comtail))
			return;
		break;

	case TOS:
		graphic = FALSE;
		break;

	case PRG:
	case APPS:
		break;

	default:
		do1_alert(NOTYPE);
		return;
	}

	do_box(win, item, (win) ? FALSE : TRUE, TRUE, TRUE);
	run_it(file, comtail, graphic, setdir);
}


/*
 * Run the application
 */
/* 306de: 00e33064 */
VOID run_it(P(const char *)file, P(char *)tail, P(BOOLEAN) graphic, P(BOOLEAN) setdir)
PP(const char *file;)
PP(char *tail;)
PP(BOOLEAN graphic;)
PP(BOOLEAN setdir;)
{
	if (m_sfirst(file, FA_ARCH|FA_DIREC|FA_RDONLY))			/* search the file */
	{
		fill_string(NO_CONST(file), FNOTFIND);
		return;
	}

	desk_wait(TRUE);

	if ((int)strlen(tail) >= PATHLEN || (int)strlen(file) >= PATHLEN)
	{
		do1_alert(NOBUF);
		goto ex_1;
	}

	if (setdir)							/* set according to application */
	{
	ex_3:
		if (!set_dir(file))
			goto ex_2;
	} else								/* set as top window    */
	{
		if (winhead->w_id == -1)
			goto ex_3;

		if (!set_dir(winhead->w_path))
		{
		ex_2:
			do1_alert(NOSETDIR);
		ex_1:
			desk_wait(FALSE);
			return;
		}
	}

	if (!save_inf(FALSE))				/* prepare to launch application */
		goto ex_1;

	wind_set(0, WF_NEWDESK, 0, 0, 0, 0);

	if (*file == CHAR_FOR_CARTRIDGE)
	{
		if (cart_init())
		{
			sh_iscart = TRUE;
		} else
		{
			do1_alert(NOCART);
			return;
		}
	} else
	{
		sh_iscart = FALSE;
	}
	
	tail[(int)strlen(tail) + 1] = 0xD; /* what for? */
	shel_write(TRUE, graphic, 0, file, tail);
	d_exit = L_LAUNCH;
}
