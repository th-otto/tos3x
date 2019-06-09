/*      DESKINS.C               3/18/89 - 9/18/89       Derek Mui       */
/*      Fix at ins_app, at app_free. Do the str check before freeing    */
/*      the app                 3/3/92                  D.Mui           */
/*      Change all the iconblk to ciconblk      7/11/92 D.Mui           */
/*      Change at ins_wicons and ins_icons      7/11/92 D.Mui           */
/*      Changed arrows at install desk icons    8/06/92 cjg             */
/*      and install window icons to invert                              */
/*      when selected.                                                  */


/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"




/*
 * Remove desktop icons
 */
/* 306de: 00e2ca20 */
VOID rm_icons(NOTHING)
{
	register OBJECT *obj;
	register int16_t i;
	int16_t found, collect;

	obj = background;

	found = FALSE;
	collect = FALSE;

	for (i = 1; i <= obj->ob_tail; i++)
	{
		if ((obj[i].ob_state & SELECTED) && (!(obj[i].ob_flags & HIDETREE)))
		{
			found = TRUE;
#if AES3D
			obj[i].ob_flags |= HIDETREE;
#else
			obj[i].ob_flags = HIDETREE;
#endif
			if (backid[i].i_type == XFILE || backid[i].i_type == XDIR)
				collect = TRUE;
		}
	}

	if (collect)						/* clean up memory  */
		lp_collect();

	if (found)							/* redraw screen    */
		clr_dicons();
}


/*
 * Install application
 */
/* 306de: 00e2cacc */
VOID ins_app(NOTHING)
{
	register OBJECT *obj;
	register APP *app;
	APP *sapp;
	int16_t install, ret, newapp;
	int16_t cont, setdir, dofull, where;
	int16_t type, sret, icon, graphic;
	const char *str;
	char buffer[8];
	int32_t l;

	cont = TRUE;
	obj = NULL;

	cont = x_first(&str, &type);

	while (cont)
	{
		if (type != WINICON && type != XFILE)
			goto is_1;

		app = app_xtype(str, &install);

		if (install)					/* Not an application   */
			goto is_1;

		obj = get_tree(ADINSAPP);

		icon = app->a_icon;
		/* if it is a wild card match */
		if (*scasb(app->a_name, '*') == '*')
			newapp = TRUE;				/* then we need app         */
		else
			newapp = FALSE;

		if (!autofile[0])
			ret = FALSE;
		else							/* skip the graphic flag */
			ret = streq(str, &autofile[3]);

		sret = ret ? TRUE : FALSE;
		obj[AUTOBOX].ob_state = ret ? SELECTED : NORMAL;
		obj[NORMALBOX].ob_state = ret ? NORMAL : SELECTED;

		obj[APGEM].ob_state = NORMAL;
		obj[APDOS].ob_state = NORMAL;
		obj[APPARMS].ob_state = NORMAL;
		obj[APPTP].ob_state = NORMAL;
		obj[INSAPP].ob_state = NORMAL;
		obj[INSWIN].ob_state = NORMAL;
		obj[INSFULL].ob_state = NORMAL;
		obj[INSFILE].ob_state = NORMAL;

		if (setdir)
			obj[INSAPP].ob_state = SELECTED;
		else
			obj[INSWIN].ob_state = SELECTED;

		if (dofull)
			obj[INSFULL].ob_state = SELECTED;
		else
			obj[INSFILE].ob_state = SELECTED;

		inf_sset(obj, APDFTYPE, newapp ? Nostr : &app->a_doc[2]);
		xinf_sset(obj, APNAME, g_name(str));
		inf_sset(obj, ARGS, newapp ? Nostr : app->a_argu);

		switch (app->a_type)
		{
		case PRG:
		case APPS:
			ret = APGEM;
			break;

		case TTP:
			ret = APPARMS;
			break;

		case TOS:
			ret = APDOS;
			break;

		case PTP:
			ret = APPTP;
			break;

		default:
			fill_string(str, NOTPRG);
			goto is_1;
		}

		obj[ret].ob_state = SELECTED;

		if (app->a_key)
			lbintoasc((int32_t) app->a_key, buffer);
		else
			buffer[0] = 0;

		inf_sset(obj, IKEY, buffer);

		fm_draw(ADINSAPP);
		where = 0;
	ins_2:
		ret = xform_do(obj, where);

		if (ret == APSKIP)
			goto is_1;

		if (ret == APCANCEL)
		{
			cont = FALSE;
			goto is_1;
		}

		if (ret == APOK)				/* install          */
		{								/* do we need a new one?    */
			fs_sget((LPTREE)obj, IKEY, buffer);
			asctobin(buffer, &l);		/* don't change this line   */
			if (buffer[0])
			{
				if (l > 20 || l < 1)
				{
					do1_alert(BADKEY);
				ins_3:
					obj[APOK].ob_state = NORMAL;
					inf_sset(obj, IKEY, Nostr);
					drawfld(obj, IKEY);
					drawfld(obj, APOK);
					where = IKEY;
					goto ins_2;
				}

				sapp = applist;

				while (sapp)
				{
					if (!newapp)		/* already installed    */
					{
						if (sapp == app)	/* found itself     */
							goto ins_4;	/* skip it      */
					}

					if (sapp->a_key == (uint16_t) l)
					{
						if (do1_alert(KEYUSED) == 2)
							goto ins_3;	/* Cancel   */
						else
							sapp->a_key = 0;	/* Overwrite    */
					}

				ins_4:
					sapp = sapp->a_next;
				}

			}
			/* check function key */
			if (newapp)
			{							/* get the Function key definition */
				app = app_alloc();		/* app_alloc will clean up doc  */
				if (!app)
				{
					do1_alert(NOAPP);
					goto ins_6;
				}
			} else						/* clean up the old define  */
			{
				app->a_doc[0] = 0;
			}
			
			/* get the doc icon type    */
			strcpy(buffer, "*.");
			fs_sget((LPTREE)obj, APDFTYPE, &buffer[2]);
			app_icon(buffer, -1, &app->a_dicon);

			lp_fill(str, &app->a_name);
			lp_collect();

			strcpy(app->a_doc, buffer);
			fs_sget((LPTREE)obj, ARGS, app->a_argu);
			graphic = 1;

			if (obj[APGEM].ob_state & SELECTED)
				type = PRG;

			if (obj[APPTP].ob_state & SELECTED)
				type = PTP;

			if (obj[APDOS].ob_state & SELECTED)
			{
				graphic = 0;
				type = TOS;
			}

			if (obj[APPARMS].ob_state & SELECTED)
			{
				graphic = 0;
				type = TTP;
			}

			app->a_type = type;
			app->a_icon = icon;
			app->a_pref = 0;

			if (obj[INSAPP].ob_state & SELECTED)
				app->a_pref = 1;

			if (obj[INSFULL].ob_state & SELECTED)
				app->a_pref += 2;

			if (obj[AUTOBOX].ob_state & SELECTED)
			{
				if (*str)
				{
					if ((int)strlen(str) < (PATHLEN - 4))
					{
						save_2(autofile, graphic);
						autofile[2] = ' ';
						strcpy(&autofile[3], str);
					} else
					{
						do1_alert(NOAUTO);
					}
				}
			} else
			{
				if (sret)				/* change from auto to normal   */
					strcpy(autofile, Nostr);
			}
			/* get the Function key definition */
			app->a_key = (uint16_t) l;

		}
		/* OK to install */
		if (ret == APREMOVE)
		{
			if (!newapp)
				app_free(app);
			if (*str)
				strcpy(autofile, Nostr);
		}

	is_1:
		if (cont == FALSE)
			break;

		cont = x_next(&str, &type);
	}

ins_6:
	if (obj)
		do_finish(ADINSAPP);
	else
		do1_alert(NOINSTAL);
}


/*
 * Install desktop icons
 */
/* 306de: 00e2d086 */
VOID ins_icons(NOTHING)
{
	register OBJECT *obj;
	register CICONBLK *iblk;
	register OBJECT *obj1;
	register int16_t type, item, icon, style;
	int16_t ret, limit, redraw, select, xitem;
	int16_t driver, quit, which;
	char buf1[2];
	char idbuffer[2];
	char buffer[14];
	GRECT pt;

	quit = FALSE;
	xitem = item = o_item;
	obj = get_tree(ADINSDIS);
	idbuffer[1] = 0;
	obj1 = background;
	limit = numicon;					/* max number of icon   */

#if !BINEXACT
	which = ret = 0; /* quiet compiler */
#endif

	while (TRUE)
	{
		iblk = (CICONBLK *) (obj[IICON].ob_spec);
		redraw = FALSE;

		if (o_status)					/* an icon is selected  */
		{
			type = backid[item].i_type;
			strcpy(buffer, ((CICONBLK *) (obj1[item].ob_spec))->monoblk.ib_ptext);
			buf1[0] = idbuffer[0] = backid[item].i_iblk.ib_char[1];
			icon = backid[item].i_icon;
			if (icon >= numicon)
				icon = numicon - 1;
		} else							/* no icon is selected  */
		{
			buffer[0] = 0;
			idbuffer[0] = 0;
			icon = 0;
			type = DISK;
			buf1[0] = 0;
		}

		if (type == XFILE || type == XDIR)	/* file     */
		{
			style = DISABLED;
			select = NONE;
			driver = FALSE;
		} else							/* printer, disk, trashcan  */
		{
			driver = TRUE;
			style = NORMAL;
			select = EDITABLE;

			if (type == DISK)
				ret = IDRIVE;

			if (type == TRASHCAN)
				ret = ITRASH;

			if (type == PRINTER)
				ret = IPRINTER;
		}

#if AES3D
		obj[DRID].ob_flags = (obj[DRID].ob_flags & (IS3DOBJ | IS3DACT)) | select;
#else
		obj[DRID].ob_flags = select;
#endif
		obj[IDRIVE].ob_state = style;
		obj[ITRASH].ob_state = style;
		obj[IPRINTER].ob_state = style;
		obj[DRID].ob_state = style;

		if (driver)
			obj[ret].ob_state = SELECTED;

		cp_iblk(get_icon(icon), &iblk->monoblk);

		iblk->monoblk.ib_char[1] = 0;

		inf_sset(obj, DRLABEL, buffer);
		inf_sset(obj, DRID, idbuffer);

	in_5:
		fm_draw(ADINSDIS);

	in_2:
		ret = xform_do(obj, 0);

		if (ret == IUP)
		{
#if TOSVERSION >= 0x400
		cg_1:
#endif
			if (icon)
			{
				icon--;
				goto in_3;
			} else
			{
#if AES3D
				wait_up();
				XDeselect(obj, ret);
#endif
				goto in_2;
			}
		}

		if (ret == IDOWN)
		{
#if TOSVERSION >= 0x400
		cg_2:
#endif
			if ((icon + 1) < limit)
			{
				icon++;
			in_3:
#if AES3D
				XSelect(obj, ret);		/* cjg 08/06/92 */
#endif
				cp_iblk(get_icon(icon), &iblk->monoblk);
				iblk->monoblk.ib_char[1] = 0;
				objc_draw(obj, IBOX, 1, full.g_x, full.g_y, full.g_w, full.g_h);
				cl_delay();

				/* cjg 08/06/92 */
#if TOSVERSION >= 0x400
				graf_mkstate(&mk_x, &mk_y, &mk_buttons, &mk_kstate);
				if (mk_buttons)
				{
					if (ret == IUP)
						goto cg_1;

					if (ret == IDOWN)
						goto cg_2;
				}
#endif
			}
#if AES3D
			wait_up();
			XDeselect(obj, ret);
#endif
			goto in_2;
		}

		if (ret == DRCANCEL)
		{
			quit = TRUE;
			goto in_1;
		}

		if (ret == DRSKIP)				/* skip */
			goto in_1;
		/* this is install  */
		fs_sget((LPTREE)obj, DRID, idbuffer);

		if (idbuffer[0] != CHAR_FOR_CARTRIDGE)
			idbuffer[0] = toupper(idbuffer[0]);

		if (driver)						/* driver type  */
		{								/* drive icon   */
			if (!(which = inf_gindex(obj, IDRIVE, 3)))	/* driver type */
			{
				if (!idbuffer[0] || idbuffer[0] == ' ')
				{
					do1_alert(NOID);
					goto in_5;
				} else
				{
					if (type == DISK)
					{
						if (idbuffer[0] != buf1[0])
							goto in_41;
					}
				}
			}
		}

		if (o_status)					/* icon selected    */
			goto in_4;					/* don't allocate   */

	in_41:
		if ((item = av_icon()) == -1)	/* get new one  */
		{
			do1_alert(NOICON);
			goto in_1;
		}

	in_4:
		redraw = TRUE;				/* user selected OK */
		iblk = (CICONBLK *) (obj1[item].ob_spec);
		cp_iblk(get_icon(icon), &iblk->monoblk);
		backid[item].i_icon = icon;

		strcpy(iblk->monoblk.ib_ptext, ((TEDINFO *) (obj[DRLABEL].ob_spec))->te_ptext);

		if (driver)
		{
			if (!which)					/* Disk Drive   */
			{
				type = DISK;
				iblk->monoblk.ib_char[1] = idbuffer[0];
			} else
			{
				type = which == 1 ? TRASHCAN : PRINTER;
				iblk->monoblk.ib_char[1] = 0;
			}
		} else
		{
			iblk->monoblk.ib_char[1] = 0;
		}
		
		backid[item].i_type = type;
	in_1:
		if (redraw)
		{
			pt.g_x = obj1[0].ob_x + obj1[item].ob_x;
			pt.g_y = obj1[0].ob_y + obj1[item].ob_y;
			pt.g_w = obj1[item].ob_width;
			pt.g_h = obj1[item].ob_height;
			do_redraw(0, &pt, 0);
		}

		if (quit)
			break;

		xitem++;

		if (!i_next(xitem, obj1, &xitem))
			break;

		item = xitem;

	}									/* while */

	do_finish(ADINSDIS);
}




/* 306de: 00e2d47e */
VOID cp_iblk(P(const ICONBLK *)src_iblk, P(ICONBLK *)dest_iblk)
PP(register const ICONBLK *src_iblk;)
PP(register ICONBLK *dest_iblk;)
{
	dest_iblk->ib_pmask = src_iblk->ib_pmask;
	dest_iblk->ib_pdata = src_iblk->ib_pdata;
	dest_iblk->ib_char[0] = src_iblk->ib_char[0];
	dest_iblk->ib_char[1] = src_iblk->ib_char[1];
	dest_iblk->ib_xchar = src_iblk->ib_xchar;
	dest_iblk->ib_ychar = src_iblk->ib_ychar;
}



/*
 * Install window icons
 */
/* 306de: 00e2d4b8 */
VOID ins_wicons(NOTHING)
{
	register APP *app;
	register OBJECT *obj;
	BOOLEAN ret;
	int16_t limit, index, quit, itype;
	int16_t type, install, pref, status;
	CICONBLK *iblk;
	char buffer[14];
	char buf2[14];
	const char *str;

	UNUSED(install);
	obj = get_tree(INWICON);
	limit = numicon;
	quit = FALSE;

	x_first(&str, &itype);
	status = TRUE;

	while (status)
	{									/* 7/11/92 */
		iblk = (CICONBLK *) (obj[WICON].ob_spec);
		inf_sset(obj, WNAME, Nostr);
		obj[WFOLDER].ob_state = NORMAL;
		obj[WNONE].ob_state = SELECTED;
#if AES3D
		obj[WFOLDER].ob_flags |= (SELECTABLE | RBUTTON);
		obj[WNONE].ob_flags |= (SELECTABLE | RBUTTON);
#else
		obj[WFOLDER].ob_flags = (SELECTABLE | RBUTTON);
		obj[WNONE].ob_flags = (SELECTABLE | RBUTTON);
#endif

		pref = 0;

		if (x_status)					/* something is selected    */
		{
#if AES3D
			obj[WNAME].ob_flags &= ~(SELECTABLE | EDITABLE);
#else
			obj[WNAME].ob_flags = 0;
#endif

			if (itype == FA_DIREC)
			{
				obj[WFOLDER].ob_state = SELECTED;
				obj[WNONE].ob_state = DISABLED;
				save_mid(NO_CONST(str), buf2);
			} else
			{
				obj[WFOLDER].ob_state = DISABLED;
				strcpy(buf2, g_name(str));
			}

#if AES3D
			obj[WFOLDER].ob_flags &= ~(SELECTABLE | RBUTTON);
			obj[WNONE].ob_flags &= ~(SELECTABLE | RBUTTON);
#else
			obj[WFOLDER].ob_flags = 0;
			obj[WNONE].ob_flags = 0;
#endif

			app = app_icon(buf2, itype == FA_DIREC ? FOLDER : -1, &index);
			pref = app->a_pref;
			xinf_sset(obj, WNAME, buf2);
		} else
		{
			index = 0;					/* icon index   */
#if AES3D
			obj[WNAME].ob_flags |= EDITABLE;
#else
			obj[WNAME].ob_flags = EDITABLE;
#endif
			buf2[0] = 0;
		}

		cp_iblk(get_icon(index), &iblk->monoblk);
		iblk->monoblk.ib_char[1] = 0;

		fm_draw(INWICON);

		while (TRUE)
		{
			ret = xform_do(obj, 0);
			fs_sget((LPTREE)obj, WNAME, buf2);
			unfmt_str(buf2, buffer);

			if (obj[WNONE].ob_state & SELECTED)
				type = ICONS;
			else
				type = FOLDER;

			if (ret == WUP)
			{
				if (index)
				{
					index--;
					goto k_1;
				}
#if AES3D
				wait_up();
				XDeselect(obj, ret);
#endif
				continue;
			}

			if (ret == WDOWN)
			{
				if ((index + 1) < limit)
				{
					index++;
				k_1:
#if AES3D
					XSelect(obj, ret);
#endif
					cp_iblk(get_icon(index), &iblk->monoblk);
					iblk->monoblk.ib_char[1] = 0;
					objc_draw(obj, WBOX, 1, full.g_x, full.g_y, full.g_w, full.g_h);
					cl_delay();
				}
#if AES3D
				wait_up();
				XDeselect(obj, ret);
#endif
				continue;
			}

			switch (ret)
			{
			case WCANCEL:
				quit = TRUE;
				break;

			case WREMOVE:
				if (!buffer[0])
					break;

				app = applist;

				while (app)
				{
					if (app->a_type == type)
					{
						if (streq(buffer, app->a_doc))
						{
							app_free(app);
							/* sort_show(0, TRUE); */
							break;
						}
					}
					app = app->a_next;
				}

				if (!app)				/* Not found    */
					do1_alert(NOMATCH);

				break;

			case WOK:					/* install a new one anyway */
				if (buffer[0])
				{						/* try to match the old one */
					app = applist;
					while (app)
					{
						if (app->a_type == type)
						{
							if (streq(buffer, app->a_doc))
								break;	/* found it */
						}
						app = app->a_next;
					}

					if (!app)			/* Not find     */
					{					/* try allocate one */
						if (!(app = app_alloc()))
						{
							do1_alert(NOICON);
							break;
						}
					}

					app->a_icon = 0;
					app->a_dicon = 0;
					app->a_pref = pref;
					app->a_type = type;
					strcpy(app->a_doc, buffer);
					lp_fill(Nostr, &app->a_name);
					app->a_dicon = index;
					app->a_icon = index;
					/* sort_show(0, TRUE); */

				}
				/* if there is something   */
			}
			break;
		}

		if (quit)
			break;

		if (x_status)
			status = x_next(&str, &itype);
		else
			break;

	}									/* while more      */

	do_finish(INWICON);
	sort_show(0, TRUE);
}


/*
 * Install all the available drives
 */
/* 306de: 00e2d86c */
VOID ins_drive(NOTHING)
{
	register int16_t i, id;
	BOOLEAN install, free;
	register OBJECT *obj;
	int32_t map;
#if STR_IN_RSC
	const char *Device;
#endif

	bfill(32, 0, dr);

	obj = background;
	free = FALSE;

	map = Drvmap();
	/* let check which one is installed */
	for (i = 1; i <= maxicon; i++)
	{
		if ((!(obj[i].ob_flags & HIDETREE)) && backid[i].i_type == DISK)
		{
			id = backid[i].i_iblk.ib_char[1];
			if (id <= '`' && id >= 'A')
			{
				dr[id - 'A'] = 1;
				if (!((map >> (id - 'A')) & 0x01))
				{
					free = TRUE;
#if AES3D
					obj[i].ob_flags |= HIDETREE;
#else
					obj[i].ob_flags = HIDETREE;
#endif
				}
			}
		}
	}
	/* now install the icons    */
	install = FALSE;

#if STR_IN_RSC
	Device = get_fstring(DEVICE);
#endif

	if (cart_init())
	{
		if (make_icon((int16_t) (CHAR_FOR_CARTRIDGE), 0, DISK, Device) != -1)
			install = TRUE;
	}

	for (i = 0; i < 32; i++)
	{
		if (!dr[i] && (map & 0x1))
		{
			if (make_icon((int16_t) (i + 'A'), 0, DISK, Device) != -1)
			{
				install = TRUE;
			} else
			{
				do1_alert(NOICON);
				break;
			}
		}

		map >>= 1;
	}

	if (install || free)
		do_redraw(0, &full, 0);
}


/*
 * Delay the icon scrolling
 */
/* 306de: 00e2d9e8 */
VOID cl_delay(NOTHING)
{
	int32_t i, j;

	j = 100000;
	i = evnt_dclick(0, 0);
	if (i)
		j = j / i;
	for (i = 0; i < j; i++)
	{
	}
}
