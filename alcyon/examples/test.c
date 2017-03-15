/* TEST.C
 * ================================================================
 * DESCRIPTION: Test program for heirarchical menus linked in
 *		with the menu code.
 */

/* INCLUDES
 * ================================================================
 */
#include <obdefs.h>
#include <gemdefs.h>
#include <vdibind.h>
#include <gembind.h>
#include <osbind.h>
#include <mn_tools.h>
#include "test.h"

/* DEFINES
 * ================================================================
 */
#define THEACTIVE       2
#define FALSE 0
#define TRUE  1


/* GLOBALS
 * ================================================================
 */

/* VDI Variables */

static short phys_handle;

/* AES variables */
short gl_hchar, gl_wchar, gl_hbox, gl_wbox;

/* OBJECT Tree pointers */
static OBJECT *ad_tree;
static OBJECT *ad_box;
static OBJECT *ad_country;
static OBJECT *ad_other;
static OBJECT *ad_menubar;
static OBJECT *ad_extra;

/* Current starting value for submenus */
static short Cur1 = 1;
static short Cur2 = 1;
static short Cur3 = 2;
static short Cur4 = QUOTE1;
static short Cur5 = MICKEY;

static short msg[8];
static MENU Menu;
static MENU MData;


/* Functions
 * ================================================================
 */

static VOID MenuCheck(P(OBJECT *) ptree, P(short) pmenu, P(short) pitem)
PP(OBJECT *ptree;)
PP(short pmenu;)
PP(short pitem;)
{
	if (ad_tree == ptree)
	{
		menu_icheck(ad_tree, Cur1, 0);
		menu_icheck(ad_tree, pitem, 1);
		Cur1 = pitem;
		menu_istart(1, ad_tree, ROOT, Cur1);
	}

	if (ad_country == ptree)
	{
		menu_icheck(ad_country, Cur2, 0);
		menu_icheck(ad_country, pitem, 1);
		Cur2 = pitem;
		menu_istart(1, ad_country, ROOT, Cur2);
	}

	if (ad_other == ptree)
	{
		if (pmenu == QUOTES)
		{
			menu_icheck(ad_other, Cur4, 0);	/* QUOTES */
			menu_icheck(ad_other, pitem, 1);
			Cur4 = pitem;
		}

		if (pmenu == MODEM)
		{
			menu_icheck(ad_other, Cur3, 0);
			menu_icheck(ad_other, pitem, 1);
			Cur3 = pitem;
		}

		if (pmenu == DISNEY)
		{
			menu_icheck(ad_other, Cur5, 0);
			menu_icheck(ad_other, pitem, 1);
			Cur5 = pitem;
		}
		menu_istart(1, ptree, pmenu, pitem);	/* MODEM, DISNEY, QUOTES */
	}
}


/* MAIN()
 * ================================================================
 */
int main(NOTHING)
{
	OBJECT *tree;
	short button;
	GRECT box;
	GRECT xrect;
	short tempx;
	GRECT rect;
	int flag;
	int done;
	short dummy;
	short out1, out2, out3, out4;
	short sel_box;
	OBJECT *sel_menu;
	
	appl_init();

	phys_handle = graf_handle(&gl_wchar, &gl_hchar, &gl_wbox, &gl_hbox);

	graf_mouse(ARROW, 0L);

	rsrc_load("test.rsc");

	rsrc_gaddr(0, MENU1, &ad_tree);
	rsrc_gaddr(0, BIGBOX, &ad_box);
	rsrc_gaddr(0, COUNTRY, &ad_country);
	rsrc_gaddr(0, OTHER, &ad_other);
	rsrc_gaddr(0, MENUBAR, &ad_menubar);
	rsrc_gaddr(0, TREE6, &ad_extra);

	ad_other[ROOT].ob_x = ad_other[ROOT].ob_y = 0;
#if 0
	/* test setting delay and height variables */
	{
		MN_SET TData;

		TData.Delay = 100L;
		TData.Drag = 10000L;
		TData.Delay = 250L;
		TData.Speed = 0L;
		TData.Height = 16;
		menu_settings(1, &TData);
	}
#endif

	if (!appl_xgetinfo(9, &out1, &out2, &out3, &out4) || !out1)
	{
		form_alert(1, "[1][Submenus not available][Cancel]");
	} else
	{
		/* test attaching submenus */
		Menu.mn_tree = ad_country;
		Menu.mn_menu = ROOT;
		Menu.mn_item = Cur2;
		Menu.mn_scroll = TRUE;
		menu_attach(1, ad_tree, DELETE, &Menu);
	
		Menu.mn_tree = ad_other;
		Menu.mn_menu = DISNEY;
		Menu.mn_item = MICKEY;
		Menu.mn_scroll = FALSE;
		menu_attach(1, ad_tree, FLOPPY, &Menu);
	
		Menu.mn_tree = ad_other;
		Menu.mn_menu = MODEM;
		Menu.mn_item = Cur3;
		Menu.mn_scroll = FALSE;
		menu_attach(1, ad_country, 5, &Menu);
	
		Menu.mn_tree = ad_other;
		Menu.mn_menu = DISNEY;
		Menu.mn_item = MICKEY;
		Menu.mn_scroll = FALSE;
		menu_attach(1, ad_country, 1, &Menu);
	
		Menu.mn_tree = ad_other;
		Menu.mn_menu = DISNEY;
		Menu.mn_item = MICKEY;
		Menu.mn_scroll = FALSE;
		menu_attach(1, ad_country, ARKANSAS, &Menu);
	
		Menu.mn_tree = ad_other;
		Menu.mn_menu = QUOTES;
		Menu.mn_item = Cur4;
		Menu.mn_scroll = FALSE;
		menu_attach(1, ad_other, 5, &Menu);
	
		Menu.mn_tree = ad_country;
		Menu.mn_menu = ROOT;
		Menu.mn_item = Cur2;
		Menu.mn_scroll = TRUE;
		menu_attach(1, ad_menubar, PASTE, &Menu);
	
		Menu.mn_tree = ad_country;
		Menu.mn_menu = ROOT;
		Menu.mn_item = Cur2;
		Menu.mn_scroll = TRUE;
		menu_attach(1, ad_menubar, FNEW, &Menu);
	
		menu_icheck(ad_tree, Cur1, 1);
		menu_icheck(ad_country, Cur2, 1);
		menu_icheck(ad_other, Cur3, 1);
		menu_icheck(ad_other, Cur4, 1);
		menu_icheck(ad_other, Cur5, 1);
	
		ActiveTree(ad_box);
	
		form_center(ad_box, &rect.g_x, &rect.g_y, &rect.g_w, &rect.g_h);
	
		ObX(ROOT) = rect.g_x;
		xrect = ObRect(TITLE1);
		objc_offset(ad_box, TITLE1, &xrect.g_x, &xrect.g_y);
		tempx = xrect.g_x;
		xrect.g_x = ((xrect.g_x + 7) / 8) * 8;
		rect.g_x = rect.g_x + (xrect.g_x - tempx);
		ObX(ROOT) = rect.g_x;
		rect.g_x -= 3;
	
		form_dial(FMD_START, rect.g_x, rect.g_y, rect.g_w, rect.g_h, rect.g_x, rect.g_y, rect.g_w, rect.g_h);
		objc_draw(ad_box, ROOT, MAX_DEPTH, rect.g_x, rect.g_y, rect.g_w, rect.g_h);
	
		do
		{
			ActiveTree(ad_box);
			button = form_do(ad_box, 0L);
	
			switch (button)
			{
			case TITLE1:
				box = ObRect(TITLE1);
				objc_offset(ad_box, TITLE1, &box.g_x, &box.g_y);
	
				Menu.mn_tree = ad_tree;
				Menu.mn_menu = ROOT;
				Menu.mn_item = Cur1;
				Menu.mn_scroll = FALSE;
				flag = menu_popup(&Menu, box.g_x, box.g_y, &MData);
				if (flag)
					MenuCheck(MData.mn_tree, MData.mn_menu, MData.mn_item);
				break;
	
			case TITLE2:
				box = ObRect(TITLE2);
				objc_offset(ad_box, TITLE2, &box.g_x, &box.g_y);
	
				Menu.mn_tree = ad_country;
				Menu.mn_menu = ROOT;
				Menu.mn_item = Cur2;
				Menu.mn_scroll = TRUE;
				flag = menu_popup(&Menu, box.g_x, box.g_y, &MData);
				if (flag)
					MenuCheck(MData.mn_tree, MData.mn_menu, MData.mn_item);
				break;
	
			case TITLE3:
				box = ObRect(TITLE3);
				objc_offset(ad_box, TITLE3, &box.g_x, &box.g_y);
	
				Menu.mn_tree = ad_other;
				Menu.mn_menu = MODEM;
				Menu.mn_item = Cur3;
				Menu.mn_scroll = FALSE;
				flag = menu_popup(&Menu, box.g_x, box.g_y, &MData);
				if (flag)
					MenuCheck(MData.mn_tree, MData.mn_menu, MData.mn_item);
				break;
	
			case TITLE4:
				box = ObRect(TITLE4);
				objc_offset(ad_box, TITLE4, &box.g_x, &box.g_y);
	
				Menu.mn_tree = ad_other;
				Menu.mn_menu = QUOTES;
				Menu.mn_item = Cur4;
				Menu.mn_scroll = FALSE;
				flag = menu_popup(&Menu, box.g_x, box.g_y, &MData);
				if (flag)
					MenuCheck(MData.mn_tree, MData.mn_menu, MData.mn_item);
				break;
	
			default:
				break;
			}
	
		} while (button != XEXIT);
	
		form_dial(FMD_FINISH, rect.g_x, rect.g_y, rect.g_w, rect.g_h, rect.g_x, rect.g_y, rect.g_w, rect.g_h);
	
		evnt_button(1, 1, 0, &dummy, &dummy, &dummy, &dummy);
	
	}
		

	/* display menubar stuff here */
	ActiveTree(ad_menubar);
	menu_bar(ad_menubar, TRUE);

	do
	{
		evnt_mesag(msg);
		if (msg[0] == MN_SELECTED)
		{
			/* msg[7] is the parent of FQUIT - which the user can't know */
			if (out4 == 0)
			{
				sel_menu = ad_menubar;
				sel_box = 0;
			} else
			{
				OBJECT **ptr = (OBJECT **)&msg[5];
				sel_menu = *ptr;
				sel_box = msg[7];
			}
			if (sel_menu == ad_menubar && msg[4] == FQUIT)
			{
				button = form_alert(1, "[1][ |  EXIT PROGRAM? ][OK|Cancel]");
				if (button == 1)
					done = TRUE;
			} else
			{
				MenuCheck(ad_menubar, sel_box, msg[4]);
			}
			menu_tnormal(ad_menubar, msg[3], TRUE);
		}
	} while (!done);
	menu_bar(ad_menubar, FALSE);

	rsrc_free();
	graf_mouse(ARROW, 0L);
	appl_exit();
	return 0;
}
