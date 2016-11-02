/* MN_MBAR.C
 * ================================================================
 * DESCRIPTION: Handles the menubar routines
 *
 * 12/20/91  cjg   - created
 * 01/07/92  cjg   - works...
 * 01/08/92  cjg   - make it work better....
 *		     RULE #1 - The initial menubar menu will blit
 *			       like the existing menus to the 
 *			       alert buffer.
 * 01/10/92  cjg   - moved duplicate AES code into DEREK.C
 * 01/13/92  cjg   - Fixed the scrolling for 1st level menus.
 * 01/13/92  cjg   - Modified to remove user-called MenuIDs
 * 01/17/92  cjg   - Added/updated comments
 * 01/29/92  cjg   - Started conversion to Alcyon ( AES Version )
 * 01/30/92  cjg   - Removed scrolling for 1st level menus.
 *		     Too many problems if they do scroll and
 *		     in objc_add() and objc_del() of menu items
 *		     in menus that are not at the end. PLUS
 *		     custom menus would be messed up.
 * 02/19/92  cjg   - merged into AES
 * 03/23/92  cjg   - added WaitForUpButton() to mn_hdo()
 * 03/24/92  cjg   - removed WaitForUpButton()
 *		   - use ev_button instead
 * 04/01/92  cjg   - Redo mn_hdo() to use MENU structures.
 * 05/06/92  cjg   - Redo mn_hdo() to use evnt_multi() better.
 * 05/13/92  cjg   - use menu_down(), menu_set() and do_chg()
 *		   - rewrote part of mn_hdo() to use m2 event to
 *		     look for the active if we are the only menu.
 *		     If a submenu, use m2 to look in 1 pixel increments.
 * 05/14/92  cjg   - pass in Process ID to appropriate functions
 * 05/19/92  cjg   - added keystate
 * 06/08/92  cjg   - when clicking on the ACC menu, restore the
 *		     menubar tree and the original menu.
 * 06/12/92  cjg   - Fixed mn_hdo() from locking up under MultiTos
 *		     when moving the mouse really fast over the menubar
 *		     while intense processing is taking place.
 * 06/29/92  cjg   - Allow the DA menu to scroll...
 * 07/01/92  cjg   - The DA menu scrolls only the Applications
 *		   - Make sure the DA menu is a minimum size
 * 07/08/92  cjg   - If entering the menubar and the title is disabled,
 *		     exit immediately.
 * 07/08/92  D.Mui - Take out mn_addr stuff
 * 07/31/92  cjg   - Removed auto-initialized global variables
 * 8/3/92    D.Mui - Change at mn_hdo's mn_popup to use gl_mnpid
 * 09/22/92  cjg   - menu_down() - Clip to the menu for drawing...
 */


/* INCLUDE FILES
 * ================================================================
 */
#include "aes.h"

#if SUBMENUS /* whole file */

#include "gemlib.h"
#include "taddr.h"
#include "gsxdefs.h"
#include "mn_tools.h"


#if BINEXACT
extern int16_t gl_mnpid;
#endif


/* GLOBALS
 * ================================================================
 */

/* These are globals used by evnt_submenu() found in evntmenu.c
 * The globals are used to help evnt_submenu() work for both popup menus
 * and the menubar.
 *
 * These are set just by the 1st level dropdown just before entering
 * menu_popup() in the routine mn_hdo().
 */
GRECT ActiveRect;						/* GRECT for the Menubar ACTIVE area */

GRECT TitleRect;						/* GRECT for the active Title       */

BOOLEAN MenuBar_Mode;					/* TRUE - Use code for the menubar  */

OBJECT *gl_mtree;						/* The menubar object tree ( ROOT ) */

MENU_PTR gl_menuptr;					/* MENU_PTR for the drop-down menu  */

/* PROTOTYPES
 * ================================================================
 */

VOID mn_mouse PROTO((BOOLEAN save));
VOID mu_save PROTO((BOOLEAN saveit, OBJECT *tree, int16_t imenu));
int16_t GetObjMenu PROTO((OBJECT *tree, int16_t ititle, OBJECT **itree));
int16_t menu_down PROTO((LPTREE tree, int16_t ititle, OBJECT **itree));


/* FUNCTIONS
 * ================================================================
 */

/*	Save and set mouse accordingly	*/

VOID mn_mouse(P(BOOLEAN) save)
PP(BOOLEAN save;)
{
	if (save)
	{
		gr_mouse(M_SAVE, NULL);		/* save mouse */
		gr_mouse(ARROW, NULL);
	} else								/* restore mouse */
	{
		gr_mouse(M_RESTORE, NULL);
	}
}


/*
 *	Change a mouse-wait rectangle based on an object's size.
 */

VOID rect_change(P(LPTREE) tree, P(MOBLK *) prmob, P(int16_t) iob, P(int16_t) x)
PP(LPTREE tree;)
PP(MOBLK *prmob;)
PP(int16_t iob;)
PP(int16_t x;)
{
	ob_offset(tree, iob, &prmob->m_x, &prmob->m_y);
	prmob->m_w = LWGET(OB_WIDTH(iob));
	prmob->m_h = LWGET(OB_HEIGHT(iob));
	prmob->m_out = x;
}


/*
*	Routine to save or restore the portion of the screen underneath
*	a menu tree.  This involves BLTing out and back
*	the data that was underneath the menu before it was pulled
*	down.
*/

VOID mu_save(P(BOOLEAN) saveit, P(OBJECT *) tree, P(int16_t) imenu)
PP(BOOLEAN saveit;)
PP(OBJECT *tree;)
PP(int16_t imenu;)
{
	GRECT t;

/*	gsx_sclip(&gl_rzero);	*/

	ob_offset((LPTREE)tree, imenu, &t.g_x, &t.g_y);
	t.g_w = LWGET(OB_WIDTH(imenu));
	t.g_h = LWGET(OB_HEIGHT(imenu));

	t.g_x -= MTH;
	t.g_w += 2 * MTH;
	t.g_h += 2 * MTH;

/*	waitforaes();	*/
	gsx_sclip(&gl_rzero);

	if (saveit)
		bb_save(&t);
	else
		bb_restore(&t);

/*	releaes();	*/
}


/*
*	Routine to pull a menu down.  This involves saving the data
*	underneath the menu and drawing in the proper menu sub-tree.
*/

int16_t menu_down(P(LPTREE) tree, P(int16_t) ititle, P(OBJECT **) itree)
PP(register LPTREE tree;)
PP(int16_t ititle;)
PP(OBJECT **itree;)
{
	register OBJECT *newtree;
	register int16_t newimenu, i;
	int16_t imenu;
	GRECT clip;

	UNUSED(clip);
	/* correlate title # to menu subtree #  */
	imenu = LWGET(OB_HEAD(THEMENUS));
	for (i = ititle - THEACTIVE; i > 1; i--)
		imenu = LWGET(OB_NEXT(imenu));

	newtree = (OBJECT *)tree;
	newimenu = imenu;
#if 0
	/* save area underneath the menu */
	if (imenu == gl_dabox)
	{
		newtree = mn_addr;
		newimenu = 0;
	}
#endif /* draw title selected  */
	if (xdo_chg((OBJECT *)tree, ititle, SELECTED, TRUE, TRUE, TRUE))
	{
		mu_save(TRUE, newtree, newimenu);	/* save the screen  */
		gsx_sclip(&gl_rzero);
		ob_draw((LPTREE)newtree, newimenu, MAX_DEPTH);
	}

	*itree = newtree;

	return (newimenu);					/* menu disabled    */
}



/* FUNCTIONS
 * ================================================================
 */


/* mn_hdo()
 * ================================================================
 * The main routine for handling the menubar. This is a modification
 * of the original mn_do() routine from the AES. The main difference
 * is the additional code to handle heirarchical menus.
 * 
 * This routine should be called only when the mouse enters the 
 * menubar area.
 *
 * This routine is missing several calls from the original mn_do()
 * that should be included.
 *
 * IN:	OBJECT *tree:	Pointer to the menubar object tree
 *
 * OUT: BOOLEAN FALSE - The user did not click on an active menu item.
 *
 *      BOOLEAN TRUE  - The user clicked on an active menu item.
 *
 *	int16_t    *ptitle: returns the menu title of the 1st level menu	 
 *	OBJECT **ptree:	returns the object tree of the menu selected
 *	int16_t    *pmenu:  returns the menu object number selected
 *	int16_t    *pitem:  returns the menu item object number selected
 */
BOOLEAN mn_hdo(P(int16_t *) ptitle, P(LPTREE *) ptree, P(int16_t *) pmenu, P(int16_t *) pitem, P(int16_t *) keyret)
PP(int16_t *ptitle;)						/* returns the menu title       */
PP(LPTREE *ptree;)							/* returns the object tree...   */
PP(int16_t *pmenu;)							/* returns the menu object      */
PP(int16_t *pitem;)							/* returns the menu item        */
PP(int16_t *keyret;)						/* returns the keystate - shift,ctrl */
{
	MENU Menu;								/* Input  Menu Values    */
	MENU MData;								/* Output Menu Values    */
	OBJECT *objs;
	OBJECT *tree;							/* ptr to the tree       */
	int16_t menu_state, wall;
	int16_t cur_title, cur_state, cur_menu, cur_item;
	int16_t last_title, last_menu;
	int16_t dummy;
	BOOLEAN done;
	BOOLEAN flag;
	int16_t NewMenuID;						/* Menu ID of menu displayed.       */
	register MENU_PTR MenuPtr;				/* Pointer to menu node structures  */
	BOOLEAN output;							/* TRUE/FALSE for valid result      */
	OBJECT *newtree;						/* tree ptr of popup menu       */
	int16_t obj;
	int16_t title_object;
	/* evnt_multi() variables */
	int16_t bbutton;
	int16_t mflags;
	uint16_t ev_which;
	MOBLK m1, m2;
	int16_t keycode, nclicks;
	MRETS mk;
	int16_t i, tail;
	int16_t curobj, lrets[6];
	int32_t bflags;
	MN_SET MValueNew;						/* CUrrent Popup/SubMenu Parameters */
	MN_SET MValueOld;						/* Old Parameters           */

	/* Initialize several key variables */
	menu_state = OUTTITLE;
	NewMenuID = 0;
	MenuPtr = NULL;
	cur_title = cur_state = cur_menu = cur_item = NIL;
	title_object = last_title = last_menu = NIL;
	done = FALSE;
	bbutton = 1;

	tree = (OBJECT *)gl_mntree;				/* global menu tree */
	newtree = tree;
	buparm = 0x01;
	flag = FALSE;
	MenuBar_Mode = FALSE;					/* TRUE - Use code for the menubar  */

	MValueNew.Display = -1;
	MValueNew.Drag = -1;
	MValueNew.Delay = -1;
	MValueNew.Speed = -1;
	MValueNew.Height = -1;

	wm_update(BEG_MCTRL);

/*   ctlmouse( TRUE );	*/

	mn_mouse(1);						/* save mouse */

	/* Setup  globals for evnt_submenu routine */

	ob_actxywh((LPTREE)tree, THEACTIVE, &ActiveRect);

	/* Get the title that we are over. - we MUST be in the menu title area
	 * in order to even get here.
	 */
	gr_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);

	/* Find the object that we are over */
	title_object = cur_title = ob_find((LPTREE)tree, THEACTIVE, 1, mk.x, mk.y);

	if (cur_title == NIL)				/* 06/11/92 D.Mui */
		goto m_hexit;

/*   cur_state = (( cur_title != NIL ) ? ( ObState( cur_title )):(0) ); */

/*   Deselect any the existing menus 	*/

	objs = tree;

	i = objs[THEACTIVE].ob_head;

	do
	{
		if (objs[i].ob_state & SELECTED)
			xdo_chg(objs, i, SELECTED, FALSE, TRUE, TRUE);

		i = objs[i].ob_next;

		if (i == THEACTIVE)
			break;

	} while (1);

	cur_state = ObState(cur_title);		/* 06/11/92 D.Mui */

	if (cur_state & DISABLED)			/* 07/08/92 D.Mui */
		goto m_hexit;

	/* Set up the new menu state */
	if (mu_set(tree, cur_title, last_title, TRUE, TRUE))
	{
		cur_menu = GetObjMenu(tree, cur_title, &newtree);
		NewMenuID = Menu_Insert(newtree, cur_menu);
		if (NewMenuID)
		{
			MenuPtr = GetMenuPtr(NewMenuID);

			MWIDTH(MenuPtr) = ObW(MPARENT(MenuPtr));
			MHEIGHT(MenuPtr) = ObH(MPARENT(MenuPtr));

			if (newtree == tree)
			{
				AssignMenuData(MenuPtr, 0);
				MSCROLL(MenuPtr) = FALSE;	/* cjg 06/29/92 */
				ob_offset((LPTREE)MTREE(MenuPtr), MPARENT(MenuPtr), &MXPOS(MenuPtr), &MYPOS(MenuPtr));
			} else
			{
				/* Find the active process */
				curobj = newtree[ROOT].ob_head;
				while (newtree[curobj].ob_state != CHECKED)
					curobj = newtree[curobj].ob_next;

				AssignMenuData(MenuPtr, curobj);
				/* This is the DA Menu - See if we need to scroll it! */
				MXPOS(MenuPtr) = newtree[ROOT].ob_x;
				MYPOS(MenuPtr) = newtree[ROOT].ob_y;
				MSCROLL(MenuPtr) = ((gl_dacnt) ? (gl_dacnt + 4) : (3));

				/* Save the original values - and restore them after.
				 * Ensure that we have a minimum height for the menu.
				 */
				MValueNew.Height = -1;
				mn_settings(0, &MValueOld);
				if (MValueOld.Height < (MSCROLL(MenuPtr) + 2))
				{
					MValueNew.Height = (MSCROLL(MenuPtr) + 2);
					mn_settings(1, &MValueNew);
					MValueNew.Height = MValueOld.Height;
				}

				CheckMenuHeight(MenuPtr);	/* allow DA menu to scroll */
				mn_settings(1, &MValueNew);
			}

			menu_down((LPTREE)tree, cur_title, &newtree);
		}
	}

#if !BINEXACT
	wall = 0; /* quiet compiler */
#endif
	while (!done)
	{
		mflags = MU_BUTTON | MU_M1;
		flag = TRUE;
		output = FALSE;

		switch (menu_state)
		{
		case INBAR:
			wall = THEACTIVE;			/* Look to enter THEACTIVE */
			flag = FALSE;

			/* Look to ENTER this MENU */
			if ((cur_title != NIL) && !IsDisabled(cur_title))
			{
				mflags = MU_BUTTON | MU_M1 | MU_M2;
				/* Look for enter this rectangle */
				rect_change((LPTREE)newtree, &m2, cur_menu, FALSE);
			}
			break;

		case OUTTITLE:
			wall = title_object;		/* Look to LEAVE this title */
			/* Look to ENTER this menu */
			if ((cur_title != NIL) && !IsDisabled(cur_title))
			{
				mflags = MU_BUTTON | MU_M1 | MU_M2;
				rect_change((LPTREE)newtree, &m2, cur_menu, FALSE);
			}
			break;
		}

		rect_change((LPTREE)tree, &m1, wall, flag);
		bflags = ((int32_t) (1) << 16);
		bflags += (1 << 8) + buparm;
		ev_which = ev_multi(mflags, &m1, &m2, 0x0L,	/* timer */
							bflags, 0x0L, &lrets[0]);

		mk.x = lrets[0];
		mk.y = lrets[1];
		mk.buttons = lrets[2];
		mk.kstate = lrets[3];
		keycode = lrets[4];
		nclicks = lrets[5];

		if (ev_which & MU_BUTTON)
		{
			obj = ob_find((LPTREE)tree, THEACTIVE, 1, mk.x, mk.y);
			if ((obj == NIL) || ((obj != NIL) && IsDisabled(obj)))
			{
				*keyret = mk.kstate;
				done = TRUE;
			} else
			{
				buparm ^= 0x01;
				continue;
			}
		}


		/* MU_M2 EVENT - ENTERED MENU! */
		if (!done)
		{
			if (ev_which & MU_M2)
			{
				if (NewMenuID)
				{
					ob_actxywh((LPTREE)tree, cur_title, &TitleRect);

					MenuBar_Mode = TRUE;
					gl_mtree = tree;
					gl_menuptr = MenuPtr;

					Menu.mn_tree = newtree;
					Menu.mn_menu = cur_menu;
					Menu.mn_item = 0;


					/* If its the DA Menu, we want scrolling...cjg 06/29/92 */
					Menu.mn_scroll = MSCROLL(MenuPtr);

					/* If its the DA Menu - Set minimum height if necessary */
					MValueNew.Height = -1;
					if (tree != newtree)
					{
						Menu.mn_item = curobj;

						/* Save the original values - and restore them after.
						 * Ensure that we have a minimum height for the menu.
						 */
						mn_settings(0, &MValueOld);
						if (MValueOld.Height < (MSCROLL(MenuPtr) + 2))
						{
							MValueNew.Height = (MSCROLL(MenuPtr) + 2);
							mn_settings(1, &MValueNew);
							MValueNew.Height = MValueOld.Height;
						}
					}
					/* 7/8/92 */
					output = mn_popup(gl_mnpid, &Menu, newtree[cur_menu].ob_x, newtree[cur_menu].ob_y, &MData);

					/* Restore Original Height, if at all */
					mn_settings(1, &MValueNew);

					gl_mtree = NULL;
					gl_menuptr = NULL;
					MenuBar_Mode = FALSE;
					if (output)
					{
						/* If the mouse is over the menu titles, don't exit
						 * Unless its a disabled menu item
						 */
						gr_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
						obj = ob_find((LPTREE)tree, THEACTIVE, 1, mk.x, mk.y);
						if ((obj == NIL) || ((obj != NIL) && IsDisabled(obj)))
							done = TRUE;
					} else
					{
						output = FALSE;
						ev_which = MU_M1;
						gr_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
					}
				}
			}							/* ev_which & MU_M2 */
		}


		if (!done)
		{
			if (ev_which & MU_M1)
			{
				last_title = cur_title;
				last_menu = cur_menu;

				/* Find the object that we are over */
				title_object = cur_title = ob_find((LPTREE)tree, THEACTIVE, 1, mk.x, mk.y);
				cur_state = ((cur_title != NIL) ? (ObState(cur_title)) : (0));

				if ((cur_title != NIL) && (cur_state != DISABLED))
				{
					menu_state = OUTTITLE;
				} else
				{
					/* We're not in the menu bar at all OR
					 * we're on a menu title that is DISABLED
					 */
					if (cur_title != NIL)
						menu_state = OUTTITLE;
					else
					{
						/* OUTSIDE menu bar */
						menu_state = INBAR;
					}
					cur_title = last_title;	/* Preserve cur_title */
				}


				/* Clean up the old menu state */
				if (mu_set(tree, last_title, cur_title, FALSE, TRUE))
				{
					if (NewMenuID)
					{
						mu_save(FALSE, newtree, last_menu);
						RestoreMenu(MenuPtr);
						Menu_Delete(NewMenuID);
						NewMenuID = 0;
						MenuPtr = NULL;
					}
				}


				/* Set up the new menu state */
				if (mu_set(tree, cur_title, last_title, TRUE, TRUE))
				{
					cur_menu = GetObjMenu(tree, cur_title, &newtree);
					NewMenuID = Menu_Insert(newtree, cur_menu);
					if (NewMenuID)
					{
						MenuPtr = GetMenuPtr(NewMenuID);

						MWIDTH(MenuPtr) = ObW(MPARENT(MenuPtr));
						MHEIGHT(MenuPtr) = ObH(MPARENT(MenuPtr));

						if (newtree == tree)
						{
							AssignMenuData(MenuPtr, 0);
							MSCROLL(MenuPtr) = FALSE;	/* cjg - 06/29/92 */
							ob_offset((LPTREE)MTREE(MenuPtr), MPARENT(MenuPtr), &MXPOS(MenuPtr), &MYPOS(MenuPtr));
						} else
						{
							/* Find the active process */
							curobj = newtree[ROOT].ob_head;
							while (newtree[curobj].ob_state != CHECKED)
								curobj = newtree[curobj].ob_next;

							AssignMenuData(MenuPtr, curobj);

							/* The tree has changed, - it is the DA menu */
							MXPOS(MenuPtr) = newtree[ROOT].ob_x;
							MYPOS(MenuPtr) = newtree[ROOT].ob_y;
							MSCROLL(MenuPtr) = ((gl_dacnt) ? (gl_dacnt + 4) : (3));

							/* Save the original values - and restore them after.
							 * Ensure that we have a minimum height for the menu.
							 */
							MValueNew.Height = -1;
							mn_settings(0, &MValueOld);
							if (MValueOld.Height < (MSCROLL(MenuPtr) + 2))
							{
								MValueNew.Height = (MSCROLL(MenuPtr) + 2);
								mn_settings(1, &MValueNew);
								MValueNew.Height = MValueOld.Height;
							}

							CheckMenuHeight(MenuPtr);	/* allow DA menu to scroll */
							mn_settings(1, &MValueNew);
						}
						menu_down((LPTREE)tree, cur_title, &newtree);
					}
				}
			}
		}
	}

	/*Decide what should be cleaned up and returned. */
	flag = FALSE;
	if (cur_title != NIL)
	{
		if (NewMenuID)
		{								/* 5/15/92  */
			mu_save(FALSE, newtree, cur_menu);
			RestoreMenu(MenuPtr);
			Menu_Delete(NewMenuID);
			NewMenuID = 0;
			MenuPtr = NULL;
		}

		/* 'output' is TRUE or FALSE from menu_popup.
		 * 'out_item' contains the menu item and
		 * 'out_tree' contains the tree for that item.
		 * returns true if AOK, FALSE otherwise.
		 */
		if (output && (MData.mn_item != NIL))
		{
			flag = TRUE;
			*ptitle = cur_title;
			*ptree = (LPTREE)MData.mn_tree;
			*pmenu = MData.mn_menu;
			*pitem = MData.mn_item;

			/* If clicked on the ACC Menu, put the tree and menu back in */
#if 0
			if (*ptree == mn_addr)
			{
				*ptree = gl_mntree;
				*pmenu = ObHead(ObTail(ROOT));
				for (i = *ptitle - THEACTIVE; i > 1; i--)
					*pmenu = ObNext(*pmenu);
			}
#endif
		} else
			xdo_chg(tree, cur_title, SELECTED, FALSE, TRUE, TRUE);
		*keyret = MData.mn_keystate;
	}

  m_hexit:								/* 06/11/92 D.Mui */

	mn_mouse(0);
/*   ctlmouse( FALSE ); */
	wm_update(END_MCTRL);

	UNUSED(tail);
	UNUSED(nclicks);
	UNUSED(keycode);
	UNUSED(bbutton);
	UNUSED(dummy);
	
	return flag;
}




/* GetObjMenu()
 * ================================================================
 * Gets the object number of the menu being displayed based upon
 * which menu title is highlighted.
 *
 * IN: OBJECT *tree
 *     int    ititle
 *
 * OUT: int - returns the object number of the menu ( -1 ) if none
 *      OBJECT **itree - returns the tree of the menu also.
 *		         If the menu is the Desk Accessory menu, the
 *			 tree can be switched.
 * 
 */
int16_t GetObjMenu(P(OBJECT *) tree, P(int16_t) ititle, P(OBJECT **) itree)
PP(OBJECT *tree;)							/* ptr to the tree we want      */
PP(int16_t ititle;)							/* the title of the menu we want */
PP(OBJECT **itree;)							/* return the menu item...      */
{
	int16_t imenu;
	int16_t i;
	OBJECT *newtree;
	int16_t newimenu;

	imenu = ObHead(ObTail(ROOT));
	for (i = ititle - THEACTIVE; i > 1; i--)
		imenu = ObNext(imenu);

	newtree = tree;
	newimenu = imenu;
#if 0
	if (imenu == gl_dabox)
	{
		newtree = mn_addr;
		newimenu = 0;
	}
#endif
	*itree = newtree;
	return (newimenu);
}

#endif /* SUBMENUS */
