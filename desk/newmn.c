/* DESKMN.C
 * =================================================================
 * DESCRIPTION: New Desk Menu Functions using Popups.
 *
 * DATE
 * 06/15/92  - cjg Created - preserved original routines.
 * 06/24/92  - cjg - Install Application works
 *                 - Desktop Configuration works
 *                 - Set Preferences works
 *                 - Set Video works - requires more data for new modes
 * 06/25/92  - cjg - install popup for defining keystrokes.
 * 07/09/92  - cjg - Set Video - 
 *                   If we're in TRUE COLOR, disable 80 column menu item
 *                   If we're in 80 column mode, disable TRUE COLOR menu item
 * 07/10/92  - cjg - Removed changes for 07/09/92
 * 07/13/92  - cjg - put in modecode routines - needs quite a few things
 *                   from kbad and townsend
 * 07/14/92  - cjg - Checked if the pen number for an object that is 3D
 *                   is greater than or equal to the number of pens.
 *                   if it is, then set pen number to pen zero.
 *                 - Don't allow 40 COLUMN MONOCHROME
 *                 - Don't allow 80 COLUMN, VGA, TRUE COLOR
 * 07/15/92  - cjg - The No Budget, IOU State, DAY 15
 *                 - _VDO Cookie for SPARROW Video is 3 for the upper word
 * 07/17/92  - cjg - Set_Video() returns TRUE and shoves set_mode() into
 *                   'd_rezword' if we want to change resolutions.
 *                 - Returns FALSE if no change or if we are not
 *                   running SPARROW video.
 *                 - Uses cookie to keep and maintain original
 *                   method of changing resolutions on an ST/TT
 * 07/21/92  - cjg - Sparrow Set_Video()
 *                 - If in Sparrow mode, ALWAYS come up first in
 *                   the Sparrow Video Dialog Box.
 *                 - Even if we're a Monochrome monitor
 * 07/31/92  - cjg - Many Changes...read ....
 *                 - use VgetMonitor() to determine type of monitor
 *                 - Removed Sparrow Video Check, must always be on
 *                   a Sparrow
 *                 - Remove TT High
 *                 - ST/TT Dialog Box
 *                   -Do not initialize to any mode
 *                   -Monochrome monitor, disable everything
 *                   -Redid the layout for this dialog box.
 * 08/03/92  - cjg - Hide TTLOW and TTMED on a TV
 *                 - fixed ST Compatibility set_mode variables
 *                 - Added a mem-check() to test if enough memory
 *                   is available for switching resolutions.
 * 08/05/92  - cjg - Change all Deselect() to XDeselect()
 *                 - Change all Select() to XSelect()
 *                 - Remove TTMED and TTLOW
 * 08/06/92  - cjg - Changed XDeselect() and XSelect() to
 *                   redraw from the root and to increase the
 *                   redraw area by 2 pixels.
 *                 - Changed Selecting Titles to drawfld()
 *                 - Enabled Arrow Buttons to INVERT when Selected.
 *                 - Buttons remain selected until mouse button is released.
 * 08/11/92  - cjg - When a function key is already used, and cancel is pressed
 *                   deselect the OK button properly.
 * 08/11/92  - cjg - Fixed bug with objc_find() inside ChkTouchButton()
 * 08/17/92  - cjg - Fixed bug - System Configuration
 *                 - Used wrong object number..
 * 09/21/92  - cjg - Configuration Popup - Look for longest string first
 * 09/24/92  - cjg - Fixed bug with MKUP and MKDOWN pausing for a scroll.
 */


/* INCLUDES
 * ==================================================================
 */

#include "desktop.h"

#if POPUP_SUPPORT /* whole file */

#include "falcon.h"

/* EXTERNS
 * ==================================================================
 */




/* DEFINES
 * ==================================================================
 */
#define Srealloc( a )   gemdos(0x15, a)
#define VgetSize( a )   xbios(91, a)
#define VcheckMode(a)   trap14(95, a)
#define VgetMonitor()   xbios(89)
#define VsetMode(a)     xbios(88,a)

/* Mouse Rectangle Structure...*/
typedef struct _mrets
{
	int16_t x;
	int16_t y;
	int16_t buttons;
	int16_t kstate;
} MRETS;



/* GLOBALS
 * ==================================================================
 */
STATIC MENU Menu;
STATIC MENU MData;

STATIC OBJECT mtree[MAXMENU + 2];				/* cjg - 07/10/92 moved to global */


/* PROTOTYPES
 * ================================================================
 */

int16_t ChkTouchButton PROTO((OBJECT *tree, int16_t button));
int16_t DoMKItems PROTO((OBJECT *dtree, int16_t button, int16_t *item));
int16_t DoKeyCheck PROTO((int16_t item));
BOOLEAN get_that_size PROTO((int16_t newmode));
VOID init_vtree PROTO((NOTHING));


/* FUNCTIONS
 * ================================================================
 */

/* ChkTouchButton
 * ================================================================
 * Given the tree and the button, the mouse is tracked until an up button.
 * This is used for buttons that are Touch Exits
 * RETURNS: TRUE - the mouse was released and is within the button.
 *          FALSE - the mouse was released and is NOT within the button.
 */
int16_t ChkTouchButton(P(OBJECT *)tree, P(int16_t) button)
PP(OBJECT *tree;)
PP(int16_t button;)
{
	MRETS mk;
	int16_t dummy;
	int16_t obj;
	GRECT rect;

	UNUSED(rect);
	wind_update(BEG_MCTRL);
	XSelect(tree, button);

	graf_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
	while (mk.buttons)
	{
		obj = objc_find(tree, ROOT, MAX_DEPTH, mk.x, mk.y);
		if (obj != button)
		{
			if (tree[button].ob_state & SELECTED)
				XDeselect(tree, button);
		} else
		{
			if (!(tree[button].ob_state & SELECTED))
				XSelect(tree, button);
		}
		graf_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
	}
	evnt_button(1, 1, 0, &dummy, &dummy, &dummy, &dummy);
	obj = objc_find(tree, ROOT, MAX_DEPTH, mk.x, mk.y);
	if (obj != button)
		XDeselect(tree, button);

	wind_update(END_MCTRL);
	return (obj == button);
}




/* mv_desk()
 * ===============================================================
 * Desktop Configuration modified to include Pop-ups.
 */
VOID mv_desk(NOTHING)
{
	register OBJECT *obj;
	register int32_t f;
	register int16_t fi, i, which;
	int16_t menui;
	register APP *app;
	int16_t len;
	APP *xapp;
	char *str;
	char temp[MAXMENU];
	OBJECT *obj1;
	char buf[2];
	OBJECT *dtree;
	int16_t dstart;
	int16_t istart;
	GRECT brect;
	int16_t flag;
	GRECT trect;
	char dcolor;
	MRETS mk;
	int16_t skip_flag;
	int32_t old_f;							/* cjg 09/24/92 */

	UNUSED(dcolor);
	UNUSED(trect);
	UNUSED(flag);
	UNUSED(brect);
	
	skip_flag = TRUE;
	obj = get_tree(MNSYSTEM);			/* Get config tree  */
	obj1 = menu_addr;					/* Get Menu Bar Tree */

	dtree = get_tree(MNMENU);			/* Get Popup menu Tree */


	for (i = 0; i < MAXMENU; i++)		/* save a copy of keystrokes */
		temp[i] = mentable[i];

	/* Initialize the First Menu Item Keystroke */
	((TEDINFO *) (obj[MKITEM].ob_spec))->te_ptext = (char *)obj1[tb3[0]].ob_spec;
	buf[1] = 0;
	buf[0] = mentable[0];
	inf_sset(obj, MKKEYS, buf);
	menui = 0;

	/* Initialize the Popup Menus - CheckMarks and Object States */
	menu_icheck(dtree, SDAPP, FALSE);
	menu_icheck(dtree, SDTOP, FALSE);
	menu_icheck(dtree, SIFULL, FALSE);
	menu_icheck(dtree, SIFILE, FALSE);

	dtree[SDAPP].ob_state = NORMAL;		/* Clear the menu states */
	dtree[SDTOP].ob_state = NORMAL;
	dtree[SIFULL].ob_state = NORMAL;
	dtree[SIFILE].ob_state = NORMAL;

	/* Initialize the starting object and button text of the default menu */
	if (s_defdir)						/* set the full path    */
	{
		dstart = SDAPP;					/* Application */
		obj[SDBUTTON].ob_spec = (intptr_t)get_fstring(FSAPP);
	} else
	{
		dstart = SDTOP;					/* Top Window  */
		obj[SDBUTTON].ob_spec = (intptr_t)get_fstring(FSTOP);
	}
	menu_icheck(dtree, dstart, 1);


	/* Initialize the starting object and button text of the path menu */
	if (s_fullpath)						/* Set input parameter  */
	{
		istart = SIFULL;				/* Full Path   */
		obj[SIBUTTON].ob_spec = (intptr_t)get_fstring(FSFULL);
	} else
	{
		istart = SIFILE;				/* File Name   */
		obj[SIBUTTON].ob_spec = (intptr_t)get_fstring(FSFILE);
	}
	menu_icheck(dtree, istart, 1);


	/* Initialize the Free Memory String */
	f_str(obj, MKMEMORY, av_mem());
	*(((TEDINFO *) (obj[MFKEY].ob_spec))->te_ptext) = '\0';

	/* save the original pointer    */
	str = ((TEDINFO *) (obj[MFFILE].ob_spec))->te_ptext;
	len = strlen(str);
	*str = 0;

	app = (APP *) 0;
	which = MFDOWN;

	old_f = f = 0;
	fm_draw(MNSYSTEM);					/* save the string  */


	for (f = 1; f <= 20; f++)			/* cjg 09/24/92 */
	{
		if (app_key(ftab[f]))			/* ALL THIS.... */
		{
			old_f = f;
			break;
		}
	}
	f = 0;								/* cjg 09/24/92 */

	fi = 0;
	goto ad_2;

	while (TRUE)
	{
	ad_5:
		which = form_do(obj, MKKEYS) & 0x7fff;
		obj[which].ob_state &= ~SELECTED;

		if (which == MKUPS || which == MKDOWNS || which == MOK)
		{
			fs_sget((LPTREE)obj, MKKEYS, buf);
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

						if (which == MOK)
							drawfld(obj, MOK);

						goto p_2;
					}
				}
			}

			mentable[menui] = buf[0];
		}

	ad_2:
		switch (which)
		{
		case MKITEM:
			if (DoMKItems(obj, MKITEM, &menui))
				goto p_11;
			break;

		case MKCLEAR:
			if (ChkTouchButton(obj, MKCLEAR))
			{

				for (i = 0; i < MAXMENU; i++)
					mentable[i] = 0;

				put_keys();
				XDeselect(obj, MKCLEAR);
				goto p_11;
			}
			break;

		case MKUPS:					/* check for keys   */

		cg_2:
			if (menui)
			{
				XSelect(obj, which);	/* cjg 08/06/92 */
				menui--;
				goto p_11;
			}
			wait_up();
			XDeselect(obj, which);		/* cjg 08/06/92 */
			break;

		case MKDOWNS:

		cg_3:
			if ((menui + 1) < MAXMENU)
			{
				XSelect(obj, which);	/* cjg 08/06/92 */
				menui++;
			p_11:
				ch_key(menui);
				((TEDINFO *) (obj[MKITEM].ob_spec))->te_ptext = (char *)obj1[tb3[menui]].ob_spec;
				drawfld(obj, MKITEM);
			p_2:
				buf[1] = 0;
				buf[0] = mentable[menui];
				inf_sset(obj, MKKEYS, buf);
				drawfld(obj, MKKEYS);
				cl_delay();

				/* cjg 08/06/92 - Check if the buttons are still down */
				graf_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
				if (mk.buttons)
				{

					if (which == MKUPS)
						goto cg_2;

					if (which == MKDOWNS)
						goto cg_3;
				}
			}
			wait_up();
			XDeselect(obj, which);		/* cjg 08/06/92 */
			break;

		case SDBUTTON:					/* Default Dir: */
			DoPopup(obj, SDBUTTON, SDTITLE, dtree, SDBASE, dstart, &dstart, FALSE, SDAPP, FSAPP, FALSE);
			break;

		case SIBUTTON:					/* Parameter: */
			DoPopup(obj, SIBUTTON, SITITLE, dtree, SIBASE, istart, &istart, FALSE, SIFULL, FSFULL, FALSE);
			break;

		case MCANCEL:					/* restore the menu key table */
			for (i = 0; i < MAXMENU; i++)
				mentable[i] = temp[i];

			goto ad_1;

		case MOK:
			s_defdir = dstart == SDAPP ? TRUE : FALSE;
			s_fullpath = istart == SIFULL ? TRUE : FALSE;
			goto ad_1;

		case MFUP:

		cg_4:
			f--;

			if (f)
				goto ad_3;
			else
			{
				if (old_f)				/* cjg 09/24/92 */
					f = old_f;
				else
					f++;
			}
			wait_up();
			XDeselect(obj, which);		/* cjg 08/06/92 */
			break;

		case MFDOWN:

		cg_5:
			f++;

			if (f <= 20)
			{

			ad_3:
				if (!(xapp = app_key(ftab[f])))
				{
					goto ad_2;
				} else
				{

					if (!skip_flag)		/* Silly thing  */
					{
						old_f = f;		/* cjg 09/24/92 */
						XSelect(obj, which);	/* cjg 08/06/92 */
					}
					app = xapp;
					fi = 0;
					((TEDINFO *) (obj[MFFILE].ob_spec))->te_ptext = NO_CONST(app->a_name);
					drawfld(obj, MFFILE);
					f_str(obj, MFKEY, f);
					drawfld(obj, MFKEY);

					if (!skip_flag)		/* Silly thing - cjg 08/06/92 */
					{
						cl_delay();
						/* cjg 08/06/92 - Check if the buttons are still down */
						graf_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
						if (mk.buttons)
						{
							if (which == MFUP)
								goto cg_4;
							if (which == MFDOWN)
								goto cg_5;
						}
					}
				}
			} else
			{
				if (old_f)				/* cjg - 09/24/92 */
					f = old_f;
				else
					f--;
			}


			if (!skip_flag)				/* Silly thing cjg -08/06/92 */
			{
				wait_up();
				XDeselect(obj, which);	/* cjg 08/06/92 */
			}
			skip_flag = FALSE;
			break;

		case MFLEFT:
		case MFRIGHT:

		cg_1:
			if (!app)
			{
				wait_up();
				XDeselect(obj, which);	/* cjg 08/06/92 */
				continue;
			}

			if (which == MFLEFT)
			{
				if (fi > 0)
				{
					fi--;
				} else
				{
					wait_up();
					XDeselect(obj, which);	/* cjg 08/06/92 */
					continue;
				}
			} else
			{
				fi++;
				if (strlen(&app->a_name[fi]) < len)
				{
					fi--;
					wait_up();
					XDeselect(obj, which);	/* cjg 08/06/92 */
					continue;
				}
			}

			XSelect(obj, which);		/* cjg 08/06/92 */
			((TEDINFO *) (obj[MFFILE].ob_spec))->te_ptext = NO_CONST(&app->a_name[fi]);
			drawfld(obj, MFFILE);

			/* cjg 08/06/92 - Check if the buttons are still down */
			graf_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
			if (mk.buttons)
				goto cg_1;

			wait_up();
			XDeselect(obj, which);		/* cjg 08/06/92 */
			goto ad_5;

		}

		cl_delay();
	}

  ad_1:								/* restore the pointer  */
	put_keys();
	((TEDINFO *) (obj[MFFILE].ob_spec))->te_ptext = str;
	*str = 1;
	do_finish(MNSYSTEM);
}




/* mins_app()
 * ===============================================================
 * Install Application using Popups
 */
VOID mins_app(NOTHING)
{
	register OBJECT *obj;
	register APP *app;
	APP *sapp;
	int16_t install, ret, newapp;
	BOOLEAN cont;
	BOOLEAN setdir;
	BOOLEAN dofull;
	int16_t where;
	int16_t type, sret, icon, graphic;
	const char *str;
	char buffer[8];
	int32_t l;

	GRECT rect;
	OBJECT *dtree;
	GRECT brect;
	GRECT trect;
	int16_t flag;
	int16_t istart;						/* Current obj - Install As */
	int16_t bstart;						/* Current Obj - Boot Status */
	int16_t fstart;						/* Current Obj - File Type  */
	int16_t dstart;						/* Current Obj - Default Dir */
	int16_t pstart;						/* Current Obj - Parameter  */
	int16_t i;
	int16_t done;

	UNUSED(flag);
	UNUSED(trect);
	UNUSED(brect);
	UNUSED(rect);
	
	cont = TRUE;
	obj = NULL;
	dtree = get_tree(MNMENU);			/* Get menu Tree */

	cont = x_first(&str, &type);

	while (cont)
	{
		if (type != WINICON && type != XFILE)
			goto is_1;

		app = app_xtype(str, &install);

		if (install)					/* Not an application   */
			goto is_1;

		obj = get_tree(MNINSAPP);


		icon = app->a_icon;
		/* if it is a wild card match   */
		if (*scasb(app->a_name, '*') == '*')
			newapp = TRUE;				/* then we need app         */
		else
			newapp = FALSE;

		if (!autofile[0])
			ret = FALSE;
		else							/* skip the graphic flag */
			ret = streq(str, &autofile[3]);

		/* Handle Boot Status */
		sret = ret ? TRUE : FALSE;
		dtree[MPBAUTO].ob_state = NORMAL;
		dtree[MPBNORML].ob_state = NORMAL;
		menu_icheck(dtree, MPBAUTO, FALSE);
		menu_icheck(dtree, MPBNORML, FALSE);
		if (ret)
		{
			bstart = MPBAUTO;
			obj[MPBBUTTN].ob_spec = (intptr_t)get_fstring(FSAUTO);
		} else
		{
			bstart = MPBNORML;
			obj[MPBBUTTN].ob_spec = (intptr_t)get_fstring(FSNORMAL);
		}
		menu_icheck(dtree, bstart, TRUE);


		/* Clear the Function Key Menu */
		for (i = MPINONE; i <= MPIF20; i++)
		{
			dtree[i].ob_state = NORMAL;
			menu_icheck(dtree, i, FALSE);
		}


		if (streq(str, app->a_name))
		{
			dofull = (app->a_pref & 0x2) ? TRUE : FALSE;
			setdir = (app->a_pref & 0x1) ? TRUE : FALSE;
		} else
		{
			dofull = s_fullpath;
			setdir = s_defdir;
		}


		/* Setup the Default Dir Menu */
		dtree[SDAPP].ob_state = NORMAL;
		dtree[SDTOP].ob_state = NORMAL;
		menu_icheck(dtree, SDAPP, FALSE);
		menu_icheck(dtree, SDTOP, FALSE);
		if (setdir)
		{
			dstart = SDAPP;
			obj[MPDBUTTN].ob_spec = (intptr_t)get_fstring(FSAPP);
		} else
		{
			dstart = SDTOP;
			obj[MPDBUTTN].ob_spec = (intptr_t)get_fstring(FSTOP);
		}
		menu_icheck(dtree, dstart, TRUE);


		/* Set the Parameter Menu */
		dtree[SIFULL].ob_state = NORMAL;
		dtree[SIFILE].ob_state = NORMAL;
		menu_icheck(dtree, SIFULL, FALSE);
		menu_icheck(dtree, SIFILE, FALSE);
		if (dofull)
		{
			pstart = SIFULL;
			obj[MPPBUTTN].ob_spec = (intptr_t)get_fstring(FSFULL);
		} else
		{
			pstart = SIFILE;
			obj[MPPBUTTN].ob_spec = (intptr_t)get_fstring(FSFILE);
		}
		menu_icheck(dtree, pstart, TRUE);


		/* Set the Document type */
		inf_sset(obj, MPDTYPE, newapp ? Nostr : &app->a_doc[2]);

		/* Set the Application Name */
		xinf_sset(obj, MPFNAME, g_name(str));

		/* Set the Arguments */
		inf_sset(obj, MPARGS, newapp ? Nostr : app->a_argu);


		/* Clear Application Type Menu */
		for (i = MPFTOS; i <= MPFGMTTP; i++)
		{
			dtree[i].ob_state = NORMAL;
			menu_icheck(dtree, i, FALSE);
		}
		switch (app->a_type)
		{
		case PRG:
		case APPS:
			ret = fstart = MPFGEM;
			obj[MPFBUTTN].ob_spec = (intptr_t)get_fstring(FSGEM);
			break;

		case TTP:
			ret = fstart = MPFTTP;
			obj[MPFBUTTN].ob_spec = (intptr_t)get_fstring(FSTOSTTP);
			break;

		case TOS:
			ret = fstart = MPFTOS;
			obj[MPFBUTTN].ob_spec = (intptr_t)get_fstring(FSTOS);
			break;

		case PTP:						/* Gem takes parameters */
			ret = fstart = MPFGMTTP;
			obj[MPFBUTTN].ob_spec = (intptr_t)get_fstring(FSGEMTTP);
			break;

		default:
			fill_string(NO_CONST(str), NOTPRG);
			goto is_1;
		}
		menu_icheck(dtree, ret, TRUE);

		/* Set the Function Key */
		if (app->a_key)
		{
			if (app->a_key < 1)			/* cjg 08/06/92 */
				app->a_key = 1;

			if (app->a_key > 20)		/* cjg 08/06/92 */
				app->a_key = 20;

			lbintoasc((int32_t) app->a_key, buffer);
			istart = MPIF1 + app->a_key - 1;

			obj[MPIBUTTN].ob_spec = (intptr_t)get_fstring(FSF1 + app->a_key - 1);
		} else
		{
			istart = MPINONE;
			buffer[0] = 0;
			obj[MPIBUTTN].ob_spec = (intptr_t)get_fstring(FSNONE);
		}
		menu_icheck(dtree, istart, TRUE);

		fm_draw(MNINSAPP);
		where = 0;
	ins_2:
		done = 0;
		while (!done)
		{
			ret = form_do(obj, where) & 0x7fff;
			obj[ret].ob_state &= ~SELECTED;

			switch (ret)
			{
			case MPSKIP:
				done = TRUE;
				break;

			case MPCANCEL:
				cont = FALSE;
				done = TRUE;
				break;

			case MPIBUTTN:				/* Install As: */
				DoPopup(obj, MPIBUTTN, MPITITLE, dtree, MPIBASE, MPINONE, &istart, FALSE, MPINONE, FSNONE, TRUE);
				break;

			case MPBBUTTN:				/* Boot Status: */
				DoPopup(obj, MPBBUTTN, MPBTITLE, dtree, MPBBASE, bstart, &bstart, FALSE, MPBAUTO, FSAUTO, FALSE);
				break;


			case MPFBUTTN:				/* File Type: */
				DoPopup(obj, MPFBUTTN, MPFTITLE, dtree, MPFBASE, fstart, &fstart, FALSE, MPFTOS, FSTOS, FALSE);
				break;


			case MPDBUTTN:				/* Default Dir: */
				DoPopup(obj, MPDBUTTN, MPDTITLE, dtree, SDBASE, dstart, &dstart, FALSE, SDAPP, FSAPP, FALSE);
				break;

			case MPPBUTTN:				/* Parameter: */
				DoPopup(obj, MPPBUTTN, MPPTITLE, dtree, SIBASE, pstart, &pstart, FALSE, SIFULL, FSFULL, FALSE);
				break;

			case MPOK:
				done = TRUE;			/*Install */
				l = (uint16_t) (istart - MPINONE);	/* Get 1 - 20 */
				if (istart >= MPIF1 && istart <= MPIF20)
				{
					sapp = applist;

					while (sapp)
					{
						if (!newapp)	/* already installed    */
						{
							if (sapp == app)	/* found itself     */
								goto ins_4;	/* skip it      */
						}

						if (sapp->a_key == (uint16_t) l)
						{
							if (do1_alert(KEYUSED) == 2)
							{
								/* Clear the Function Key Menu */
								for (i = MPINONE; i <= MPIF20; i++)
								{
									dtree[i].ob_state = NORMAL;
									menu_icheck(dtree, i, FALSE);
								}

								/* Set the Function Key */
								if (app->a_key)
								{
									lbintoasc((int32_t) app->a_key, buffer);
									istart = MPIF1 + app->a_key - 1;
									obj[MPIBUTTN].ob_spec = (intptr_t)get_fstring(FSF1 + app->a_key - 1);
								} else
								{
									istart = MPINONE;
									buffer[0] = 0;
									obj[MPIBUTTN].ob_spec = (intptr_t)get_fstring(FSNONE);
								}
								menu_icheck(dtree, istart, TRUE);

								drawfld(obj, MPIBUTTN);

								/* cjg 08/11/92 */
								obj[MPOK].ob_state |= SELECTED;
								XDeselect(obj, MPOK);
								where = 0;
								goto ins_2;
							} else
							{
								sapp->a_key = 0;	/* Overwrite    */
							}
						}
					ins_4:
						sapp = sapp->a_next;
					}
				}
				/* check function key */
				if (newapp)
				{						/* get the Function key definiton */
					app = app_alloc();	/* app_alloc will clean up doc  */
					if (!app)
					{
						do1_alert(NOAPP);
						goto ins_6;
					}
				} else					/* clean up the old define  */
				{
					app->a_doc[0] = 0;
				}
				
				/* get the doc icon type    */
				strcpy(buffer, "*.");
				fs_sget((LPTREE)obj, MPDTYPE, &buffer[2]);
				app_icon(buffer, -1, &app->a_dicon);

				strcpy(app->a_doc, buffer);
				fs_sget((LPTREE)obj, MPARGS, app->a_argu);
				graphic = 1;

				switch (fstart)
				{
				case MPFGEM:
					type = PRG;
					break;

				case MPFGMTTP:
					type = PTP;
					break;

				case MPFTOS:
					graphic = 0;
					type = TOS;
					break;

				case MPFTTP:
					graphic = 0;
					type = TTP;
					break;
				}

				app->a_type = type;
				app->a_icon = icon;
				app->a_pref = 0;

				if (dtree[SDAPP].ob_state & CHECKED)
					app->a_pref = 1;

				if (dtree[SIFULL].ob_state & CHECKED)
					app->a_pref += 2;

				if (dtree[MPBAUTO].ob_state & CHECKED)
				{
					if (*str)
					{
						if (strlen(str) < (PATHLEN - 4))
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
					if (sret)			/* change from auto to normal   */
						strcpy(autofile, Nostr);
				}
				/* get the Function key definiton */
				app->a_key = (uint16_t) l;

				lp_fill(str, &app->a_name);
				lp_collect();
				break;

			case MPREMOVE:
				done = TRUE;
				if (*str)				/* changed 3/3/92   */
					strcpy(autofile, Nostr);

				if (!newapp)
					app_free(app);
				break;

			default:
				break;
			}
		}
	is_1:
		if (cont == FALSE)
			break;

		cont = x_next(&str, &type);
	}

ins_6:
	if (obj)
		do_finish(MNINSAPP);
	else
		do1_alert(NOINSTAL);
}





/* mdesk_pref()
 * ===============================================================
 * Desk Preferences using Pop-Ups
 */
VOID mdesk_pref(NOTHING)
{
	register OBJECT *obj;
	register int16_t cyes, i, flag;
	int16_t overwrite;
	int16_t font;
	OBJECT *dtree;
	int16_t dstart;
	int16_t cstart;
	int16_t ostart;
	int16_t button;

	UNUSED(font);
	UNUSED(overwrite);
	UNUSED(flag);
	UNUSED(i);
	UNUSED(cyes);
	
	obj = get_tree(MNSETPREF);
	dtree = get_tree(MNMENU);

	if (cdele_save)
	{
		dstart = SPYES;
		obj[SPDBUTTN].ob_spec = (intptr_t)get_fstring(FSYES);
	} else
	{
		dstart = SPNO;
		obj[SPDBUTTN].ob_spec = (intptr_t)get_fstring(FSNO);
	}

	if (!write_save)					/* Overwrite is backwards... */
	{
		ostart = SPYES;
		obj[SPOBUTTN].ob_spec = (intptr_t)get_fstring(FSYES);
	} else
	{
		ostart = SPNO;
		obj[SPOBUTTN].ob_spec = (intptr_t)get_fstring(FSNO);
	}

	if (ccopy_save)
	{
		cstart = SPYES;
		obj[SPCBUTTN].ob_spec = (intptr_t)get_fstring(FSYES);
	} else
	{
		cstart = SPNO;
		obj[SPCBUTTN].ob_spec = (intptr_t)get_fstring(FSNO);
	}

	fm_draw(MNSETPREF);
	button = 0;
	do
	{
		button = form_do(obj, 0) & 0x7fff;
		obj[button].ob_state &= ~SELECTED;

		dtree[SPYES].ob_state = NORMAL;
		dtree[SPNO].ob_state = NORMAL;

		switch (button)
		{
		case MNSPOK:
			cdele_save = (dstart == SPYES);
			ccopy_save = (cstart == SPYES);
			write_save = (ostart != SPYES);

		case MNSPCAN:
			break;

		case SPDBUTTN:
			menu_icheck(dtree, dstart, TRUE);
			DoPopup(obj, SPDBUTTN, SPDTITLE, dtree, SPBASE, dstart, &dstart, FALSE, SPYES, FSYES, FALSE);
			break;

		case SPCBUTTN:
			menu_icheck(dtree, cstart, TRUE);
			DoPopup(obj, SPCBUTTN, SPCTITLE, dtree, SPBASE, cstart, &cstart, FALSE, SPYES, FSYES, FALSE);
			break;

		case SPOBUTTN:
			menu_icheck(dtree, ostart, TRUE);
			DoPopup(obj, SPOBUTTN, SPOTITLE, dtree, SPBASE, ostart, &ostart, FALSE, SPYES, FSYES, FALSE);
			break;
		}
	} while (button != MNSPOK && button != MNSPCAN);

	do_finish(MNSETPREF);
}




/* set_video()
 * ===============================================================
 * Set Video Preferences using Pop-Ups( 2Color - True Color )
 * and sets the initial parameters based upon the VsetMode() call.
 * 
 * RETURNS: TRUE - We are in SPARROW Video and want to change the rez
 *          FALSE - all others
 * RULES:
 * 1) if the Video Cookie value is == 3, we are in SPARROW Video
 *
 * 2) if we are on an ST/TT Shifter, display ONLY the ST/TT Dialog Box.
 *
 * 3) If we are SPARROW Video, ALWAYS come up first in the Sparrow
 *    Video Dialog Box.
 *
 * 5) TT High is not possible with the Sparrow Shifter
 * 6) TT LOW and TT MED are faked using the set_mode() command
 *    BUT the ST COMPATIBLE flag is NOT SET!
 *
 * 7) If we have a SPARROW Shifter, and the current rez is an ST compatible
 *    resolution, Display the Sparrow Video Dialog Box FIRST still.
 */
BOOLEAN set_video(NOTHING)
{
	register OBJECT *obj;
	register int16_t cyes, i, flag;
	int16_t overwrite, font;
	OBJECT *dtree;
	int16_t button;
	int16_t cstart, old_cstart;
	int16_t lstart, old_lstart;
	int16_t dstart, old_dstart;
	OBJECT *vtree;
	int16_t output;
	int16_t done;
	int16_t monitor_type;
	uint16_t mode_code;
	uint16_t old_mode_code;
	int16_t color_flag;
	int16_t column_flag;
	int16_t vga_flag;
	int16_t pal_flag;
	int16_t overscan_flag;
	int16_t st_flag;
	int16_t vertical_flag;
	BOOLEAN double_flag;					/* 1 = double, 0 = interlaced mode */
	int32_t p_value;
	char buffer[10];

	UNUSED(buffer);
	UNUSED(p_value);
	UNUSED(overwrite);
	UNUSED(cyes);
	UNUSED(font);
	
	obj = get_tree(MNSETVID);
	dtree = get_tree(MNMENU);

	/* Check if we're on a MONOCHROME MONITOR.
	 * If so, we can ONLY display ST HIGH 
	 *
	 * 0 = ST monochrome monitor;
	 * 1 = ST color monitor;
	 * 2 = VGA monitor;
	 * 3 = Television;
	 * The ST color monitor and Television are considered equivalent
	 * for our purposes....
	 */
	monitor_type = VgetMonitor();

	obj[SVCBUTTN].ob_state &= ~DISABLED;
	obj[SVLBUTTN].ob_state &= ~DISABLED;
	obj[SVDBUTTN].ob_state &= ~DISABLED;

	obj[SVCBUTTN].ob_flags |= TOUCHEXIT;
	obj[SVLBUTTN].ob_flags |= TOUCHEXIT;
	obj[SVDBUTTN].ob_flags |= TOUCHEXIT;

	/* 07/21/92 - cjg - display Sparrow Video Dialog Box first */
	/* But, if monochrome, just disable everything... */
	if (!monitor_type)
	{
		obj[SVCBUTTN].ob_state |= DISABLED;
		obj[SVLBUTTN].ob_state |= DISABLED;
		obj[SVDBUTTN].ob_state |= DISABLED;

		obj[SVCBUTTN].ob_flags &= ~TOUCHEXIT;
		obj[SVLBUTTN].ob_flags &= ~TOUCHEXIT;
		obj[SVDBUTTN].ob_flags &= ~TOUCHEXIT;
	}
	old_mode_code = mode_code = VsetMode(-1);
	color_flag = mode_code & VIDEL_BPPMASK;	/* # of bits per pixel */
	column_flag = mode_code & VIDEL_80COL;	/* 40 or 80 column     */

	vga_flag = mode_code & VIDEL_VGA;			/* VGA or TV/COLOR     */
	pal_flag = mode_code & VIDEL_PAL;			/* PAL or NTSC         */
	overscan_flag = mode_code & VIDEL_OVERSCAN;	/* Overscan or not     */
	st_flag = mode_code & VIDEL_COMPAT;			/* ST mode or NOT      */
	vertical_flag = mode_code & VIDEL_VERTICAL;	/* ON or OFF           */


	/* Determine whether to display Double Line or Interlace
	 * Check if vga_flag == VGA
	 */
	double_flag = vga_flag == VIDEL_VGA ? TRUE : FALSE;


	/* Initialize Colors, Columns and Double Line/Interlace */
	for (i = VID2; i <= VIDTRUE; i++)
		menu_icheck(dtree, i, FALSE);

	/* Initialize the number of colors available */
	cstart = color_flag + VID2;

	obj[SVCBUTTN].ob_spec = (intptr_t)get_fstring(color_flag + FS2C);
	menu_icheck(dtree, cstart, TRUE);


	/* Initialize the number of columns */
	menu_icheck(dtree, SVL40, FALSE);
	menu_icheck(dtree, SVL80, FALSE);

	if (column_flag == VIDEL_80COL)
	{
		lstart = SVL80;
		obj[SVLBUTTN].ob_spec = (intptr_t)get_fstring(FS80);
	} else
	{
		lstart = SVL40;
		obj[SVLBUTTN].ob_spec = (intptr_t)get_fstring(FS40);
	}
	menu_icheck(dtree, lstart, TRUE);


	/* Get Free String for Title - Double Line or Interlace
	 * If we are on a VGA monitor, use double...
	 * If we are on a Television, use Interlace
	 */
	if (double_flag)
		((TEDINFO *) (obj[SVDTITLE].ob_spec))->te_ptext = get_fstring(DLTEXT);
	else
		((TEDINFO *) (obj[SVDTITLE].ob_spec))->te_ptext = get_fstring(ITEXT);

	/* Initialize the double/interlace to ON or OFF */
	menu_icheck(dtree, SVION, FALSE);
	menu_icheck(dtree, SVIOFF, FALSE);

	if (vertical_flag)
	{
		dstart = SVION;
		obj[SVDBUTTN].ob_spec = (intptr_t)get_fstring(FSON);
	} else
	{
		dstart = SVIOFF;
		obj[SVDBUTTN].ob_spec = (intptr_t)get_fstring(FSOFF);
	}
	menu_icheck(dtree, dstart, TRUE);

	button = 0;
	output = 0;
	done = FALSE;

	old_cstart = cstart;
	old_lstart = lstart;
	old_dstart = dstart;


	fm_draw(MNSETVID);
	do
	{
		button = form_do(obj, 0) & 0x7fff;
		obj[button].ob_state &= ~SELECTED;

		switch (button)
		{
			/* SParrow Video OK - NO ST COMPATIBILITY
			 * Change ONLY if one of the 3 variables has
			 * changed AND we are NOT on a monochrome monitor.
			 * Or, if we are in STMODE and want to get
			 * out of it...
			 */
		case SVOK:
			if ((cstart != old_cstart ||
				 lstart != old_lstart ||
				 (start != old_dstart ||
				 (old_mode_code & VIDEL_COMPAT)) && monitor_type)
			{
				st_flag = FALSE;
				mode_code = (cstart - VID2) |
							(lstart == SVL80 ? VIDEL_80COL : VIDEL_40COL) |
							vga_flag | pal_flag | overscan_flag | st_flag |
							(dstart == SVION ? VIDEL_VERTICAL : 0);
				/* Check if there is enough memory
				 * to switch to the new mode
				 * If not, go back to form_do...
				 */
				if (!get_that_size(mode_code))
				{
					XDeselect(obj, SVOK);
					continue;
				}
			} else
			{
				button = SVCANCEL;
			}
			/* fall through */
		case SVCANCEL:
			done = TRUE;
			do_finish(MNSETVID);
			break;

		case SVMODES:					/* Compatibility Modes */
			do_finish(MNSETVID);
		dialog1:
			vtree = get_tree(MNVIDEO);
			init_vtree();
			output = fmdodraw(MNVIDEO, 0);
			vtree[output].ob_state &= ~SELECTED;

			/* Must be in non-monochrome mode */
			if (output == SVTTOK)
			{
				button = SVCANCEL;
				if (monitor_type)
				{
					/* Change rez via modecode */
					st_flag = FALSE;
					mode_code = vga_flag | pal_flag | overscan_flag;

					vtree = get_tree(MNVIDEO);
					flag = inf_gindex((LPTREE)vtree, SVSTLOW, 3);
					if (flag != -1)
					{
						switch (flag)
						{
						case 0:		/* STLOW */
							st_flag = VIDEL_COMPAT;
							vertical_flag = vga_flag == VIDEL_VGA ? VIDEL_VERTICAL : 0;
							mode_code |= VIDEL_COMPAT | VIDEL_4BPP | VIDEL_40COL | vertical_flag;
							break;

						case 1:		/* STMED */
							st_flag = VIDEL_COMPAT;
							vertical_flag = vga_flag == VIDEL_VGA ? VIDEL_VERTICAL : 0;
							mode_code |= VIDEL_COMPAT | VIDEL_2BPP | VIDEL_80COL | vertical_flag;
							break;

						case 2:		/* STHIGH */
							st_flag = VIDEL_COMPAT;
							vertical_flag = vga_flag == VIDEL_VGA ? 0 : VIDEL_VERTICAL;
							mode_code |= VIDEL_COMPAT | VIDEL_1BPP | VIDEL_80COL | vertical_flag;
							break;
#if 0
						case 3:		/* TTLOW - NOTE: NON-ST Mode Flag */
							vertical_flag = vga_flag == VIDEL_VGA ? 0 : VIDEL_VERTICAL;
							mode_code |= VIDEL_8BPP | VIDEL_40COL | vertical_flag;
							break;

						case 4:		/* TTMED - NOTE: NON-ST Mode Flag */
							vertical_flag = vga_flag == VIDEL_VGA ? 0 : VIDEL_VERTICAL;
							mode_code |= VIDEL_4BPP | VIDEL_80COL | vertical_flag;
							break;
#endif

						}

						/* If built mode_code does not AND
						 * with current setting, change rez!
						 */

						flag++;
						if (mode_code & VIDEL_COMPAT)
						{
							gl_restype = flag;
							pref_save = flag;
							gl_rschange = TRUE;
							d_exit = L_CHGRES;
							/*   d_rezword = mode_code;    */
						}

						if (mode_code ^ old_mode_code)
						{
							if (get_that_size(mode_code))
							{
								button = SVTTOK;
							} else
							{
								XDeselect(vtree, SVTTOK);
								goto dialog1;
							}

						}

					}
				}
				done = TRUE;
			} else
			{
				st_flag = FALSE;
				fm_draw(MNSETVID);
			}
			break;

		case SVCBUTTN:					/* Number of Colors Popup */

			/* Don't allow 80col, VGA, True Color */
			if (vga_flag == VIDEL_VGA && lstart == SVL80)
				dtree[VIDTRUE].ob_state |= DISABLED;
			else
				dtree[VIDTRUE].ob_state &= ~DISABLED;

			/* Don't allow 40col, 2 Color */
			if (lstart == SVL40)
				dtree[VID2].ob_state |= DISABLED;
			else
				dtree[VID2].ob_state &= ~DISABLED;

			DoPopup(obj, SVCBUTTN, SVCTITLE, dtree, VIDBASE, cstart, &cstart, FALSE, VID2, FS2C, FALSE);
			break;

		case SVLBUTTN:					/* Number of Columns Popup */

			/* Don't allow 80col, VGA, TRUE Color */
			if (vga_flag == VIDEL_VGA && cstart == VIDTRUE)
				dtree[SVL80].ob_state |= DISABLED;
			else
				dtree[SVL80].ob_state &= ~DISABLED;

			/* Don't Allow 40col Monochrome */
			if (cstart == VID2)
				dtree[SVL40].ob_state |= DISABLED;
			else
				dtree[SVL40].ob_state &= ~DISABLED;
			DoPopup(obj, SVLBUTTN, SVLTITLE, dtree, SVLBASE, lstart, &lstart, FALSE, SVL40, FS40, FALSE);
			break;

		case SVDBUTTN:					/* Double/Interlace - ON/OFF */
			DoPopup(obj, SVDBUTTN, SVDTITLE, dtree, SVIBASE, dstart, &dstart, FALSE, SVION, FSON, FALSE);
			break;
		}

	} while (!done);


	/* Change the Resolution! - Pass mode_code to DEREK! */
	if (button != SVCANCEL)
	{									/* valid the mode code */
#if TOSVERSION >= 0x400
		if ((gl_vdo & 0x30000L) == 0x30000L)
			mode_code = VcheckMode(mode_code);

		d_rezword = mode_code;
#endif
		return TRUE;

	}
	return (FALSE);
}



/* XSelect()
 * ==================================================================
 * Select an object and redraw
 */
VOID XSelect(P(OBJECT *)tree, P(int16_t) obj)
PP(OBJECT *tree;)
PP(int16_t obj;)
{
	GRECT trect;

	/* If already selected, return... */
	if (tree[obj].ob_state & SELECTED)
		return;

	tree[obj].ob_state |= SELECTED;
	trect = *(GRECT *) & tree[(obj)].ob_x;
	objc_offset(tree, obj, &trect.g_x, &trect.g_y);
	trect.g_x -= 2;						/* cjg 08/06/92 */
	trect.g_y -= 2;
	trect.g_w += 4;
	trect.g_h += 4;
	objc_draw(tree, obj, MAX_DEPTH, trect.g_x, trect.g_y, trect.g_w, trect.g_h);
}


/* XDeselect()
 * ====================================================================
 * Deselect an object and redraw - use the flag to redraw in case
 * the area to redraw needs to be bigger.( ie: SHADOWED )
 */
VOID XDeselect(P(OBJECT *) tree, P(int16_t) obj)
PP(OBJECT *tree;)
PP(int16_t obj;)
{
	GRECT trect;

	if (tree[obj].ob_state & SELECTED)
	{
		tree[obj].ob_state &= ~SELECTED;
		trect = *(GRECT *) & tree[(obj)].ob_x;
		objc_offset(tree, obj, &trect.g_x, &trect.g_y);
		trect.g_x -= 2;					/* cjg 08/06/92 */
		trect.g_y -= 2;
		trect.g_w += 4;
		trect.g_h += 4;
		objc_draw(tree, obj, MAX_DEPTH, trect.g_x, trect.g_y, trect.g_w, trect.g_h);
	}
}



/* DoPopup()
 * ====================================================================
 * Popup Routine to the dialog boxes.
 * This routine will select the title and dispay the PopUp Menu
 * at the button location.
 */
VOID DoPopup(P(OBJECT *)tree, P(int16_t) button, P(int16_t) title, P(OBJECT *)Mtree, P(int16_t) Mmenu, P(int16_t) Mfirst, P(int16_t *) Mstart, P(int16_t) Mscroll, P(int16_t) FirstMenu, P(int16_t) FirstText, P(int16_t) Skip)
PP(OBJECT *tree;)
PP(int16_t button;)
PP(int16_t title;)
PP(OBJECT *Mtree;)
PP(int16_t Mmenu;)
PP(int16_t Mfirst;)
PP(int16_t *Mstart;)
PP(int16_t Mscroll;)
PP(int16_t FirstMenu;)
PP(int16_t FirstText;)
PP(int16_t Skip;)
{
	GRECT brect;
	int16_t flag;

	if (!ChkTouchButton(tree, button))
		return;

	/* Select the Title */
	tree[title].ob_state |= SELECTED;
	drawfld(tree, title);

	objc_offset(tree, button, &brect.g_x, &brect.g_y);

	Menu.mn_tree = Mtree;
	Menu.mn_menu = Mmenu;
	Menu.mn_item = Mfirst;
	Menu.mn_scroll = Mscroll;
	flag = menu_popup(&Menu, brect.g_x, brect.g_y, &MData);
	if (flag)
	{
		if (Mtree == MData.mn_tree && MData.mn_menu == Mmenu)
		{
			menu_icheck(Mtree, *Mstart, 0);
			menu_icheck(Mtree, MData.mn_item, 1);
			*Mstart = MData.mn_item;

			if (!Skip)
				menu_istart(1, Mtree, Mmenu, *Mstart);

			tree[button].ob_spec = (intptr_t)get_fstring(*Mstart - FirstMenu + FirstText);
		}
	}
	/* Deselect the title */
	XDeselect(tree, title);
	XDeselect(tree, button);
}



/* init_vtree()
 * ====================================================================
 * Initialize the Video Dialog Box ( ST and TT Modes )
 */
VOID init_vtree(NOTHING)
{
	register OBJECT *vtree;
	register int16_t cyes, i, flag;
	int16_t monitor_type;

	UNUSED(flag);
	UNUSED(cyes);
	
	vtree = get_tree(MNVIDEO);

	/* Clear the Compatability buttons */
	for (i = SVSTLOW; i <= SVSTHIGH; i++)
		vtree[i].ob_state = NORMAL;

	/* If we're on a monochrome monitor, disable all of the buttons */
	monitor_type = VgetMonitor();
	if (!monitor_type)
	{
		for (i = SVSTLOW; i <= SVSTHIGH; i++)
			vtree[i].ob_state = DISABLED;
	}
#if 0
	vtree[SVTTLOW].ob_flags &= ~HIDETREE;
	vtree[SVTTMED].ob_flags &= ~HIDETREE;
	/* If we're on a TV or Color monitor, hide the TTLOW and TTMED buttons */
	if ((d_rezword & VIDEL_VGA) == 0)
	{
		vtree[SVTTLOW].ob_flags |= HIDETREE;
		vtree[SVTTMED].ob_flags |= HIDETREE;
	}
#endif
}





/* DoMKItems()
 * ====================================================================
 * Handle the Menu Item Popup for the configuration dialog box
 */
int16_t DoMKItems(P(OBJECT *)dtree, P(int16_t) button, P(int16_t *)item)
PP(OBJECT *dtree;)
PP(int16_t button;)
PP(int16_t *item;)
{
	OBJECT *obj1;
	int16_t index;
	OBJECT *tree;
	int16_t flag;
	GRECT brect;
	int16_t start;
	int16_t old_item;
	int16_t length;
	int16_t temp;

	obj1 = menu_addr;
	start = *item + 1;

	/* cjg 09/21/92 */
	length = 0;
	temp = 0;
	/* Get the length of the largest string */
	for (index = 1; index <= MAXMENU; index++)
	{
		temp = strlen((char *)obj1[tb3[index - 1]].ob_spec);
		if (temp > length)
			length = temp;
	}

	/* Ensure a minimum width */
	if (length < 30)
		length = 30;

	/* Initialize the ROOT */
	mtree[ROOT].ob_next = -1;
	mtree[ROOT].ob_head = mtree[ROOT].ob_tail = -1;
	mtree[ROOT].ob_type = G_BOX;
	mtree[ROOT].ob_flags = LASTOB;
	mtree[ROOT].ob_state = SHADOWED;
	mtree[ROOT].ob_spec = 0xFF1100L;
	mtree[ROOT].ob_x = mtree[ROOT].ob_y = 0;
	mtree[ROOT].ob_width = length;		/* cjg 09/21/92  - was 30 */
	mtree[ROOT].ob_height = MAXMENU;

	for (index = 1; index <= MAXMENU; index++)
	{
		mtree[index].ob_next = -1;
		mtree[index].ob_head = mtree[index].ob_tail = -1;
		mtree[index].ob_type = G_STRING;
		mtree[index].ob_flags = NONE;
		mtree[index].ob_state = NORMAL;
		mtree[index].ob_spec = obj1[tb3[index - 1]].ob_spec;
		mtree[index].ob_x = 0;
		mtree[index].ob_y = (index - 1);
		mtree[index].ob_width = length;	/* cjg 09/21/92 30; */
		mtree[index].ob_height = 1;
		objc_add(mtree, ROOT, index);
	}

	for (index = ROOT; index <= MAXMENU; index++)
		rsrc_obfix((LPTREE)mtree, index);

	mtree[MAXMENU].ob_flags |= LASTOB;
	tree = &mtree[ROOT];
	menu_icheck(tree, start, TRUE);



	if (!ChkTouchButton(dtree, button))
		return (FALSE);

	old_item = DoKeyCheck(*item);
	if (old_item != -1)
		mtree[old_item + 1].ob_spec = obj1[tb3[old_item]].ob_spec;
	mtree[start].ob_spec = obj1[tb3[*item]].ob_spec;

	objc_offset(dtree, button, &brect.g_x, &brect.g_y);

	Menu.mn_tree = tree;
	Menu.mn_menu = ROOT;
	Menu.mn_item = start;
	Menu.mn_scroll = TRUE;
	flag = menu_popup(&Menu, brect.g_x, brect.g_y, &MData);
	if (flag)
	{
		if (tree == MData.mn_tree)
		{
			menu_icheck(tree, start, 0);
			menu_icheck(tree, MData.mn_item, 1);
			*item = MData.mn_item - 1;
			dtree[button].ob_state &= ~SELECTED;
			return (TRUE);
		}
	}
	XDeselect(dtree, button);
	return (FALSE);
}


/* DoKeyCheck()
 * ====================================================================
 * Check if the keystroke is already used by another menu item
 * in the configuration dialog box.
 */
int16_t DoKeyCheck(P(int16_t) item)
PP(int16_t item;)
{
	OBJECT *obj;
	int16_t i;
	char buf[2];
	int16_t flag;

	flag = -1;

	obj = get_tree(MNSYSTEM);			/* Get config tree */

	fs_sget((LPTREE)obj, MKKEYS, buf);
	buf[0] = toupper(buf[0]);
	if (buf[0])
	{
		for (i = 0; i < MAXMENU; i++)
		{
			if (i == item)				/* Skip checking our own */
				continue;

			if (mentable[i] == buf[0])	/* if found elsewhere? */
			{
				if (do1_alert(KEYUSED) == 1)	/* Overwrite */
				{
					mentable[i] = 0;
					ch_key(i);
					flag = i;
					continue;
				}
				buf[1] = 0;
				buf[0] = mentable[item];
			}
		}
	}
	mentable[item] = buf[0];
	inf_sset(obj, MKKEYS, buf);
	drawfld(obj, MKKEYS);
	ch_key(item);
	return (flag);
}




BOOLEAN get_that_size(P(int16_t) newmode)
PP(int16_t newmode;)
{
	int32_t avail, size, needed;
	int32_t char_bytes;
	int16_t IsVGA;
	int16_t IsVertFlag;
	int16_t Result;
	int16_t Bits;

	UNUSED(Bits);
	avail = Srealloc(-1L);
	size = VgetSize(newmode);
	IsVGA = (newmode & VIDEL_VGA) == VIDEL_VGA;
	IsVertFlag = (newmode & VIDEL_VERTICAL) == VIDEL_VERTICAL;
	Result = 2 - !(IsVGA ^ IsVertFlag);

	char_bytes = 8 * (1 << newmode & VIDEL_BPPMASK) * Result;
	needed = 25L * 40L * char_bytes;

	if ((avail - size) < needed)
	{
		do1_alert(FCNOMEM);
		return FALSE;
	}
	return TRUE;
}



VOID wait_up(NOTHING)
{
	MRETS mk;

	do
	{
		graf_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
	} while (mk.buttons);
}

#endif /* POPUP_SUPPORT */
