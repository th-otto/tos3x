/* MN_MENU.C
 * ================================================================
 * DESCRIPTION: Menu allocation manager
 * Modified from the POPMENU.C code
 *
 * 11/22/91   cjg	created
 * 11/27/91   cjg	Pop-Up Menus and Scrolling Menus work just A-OK!
 * 11/28/91   cjg       Happy Thanksgiving ( yum! yum! )
 *			Changed code so that a menu doesn't have
 *			to start at the root of a tree.
 * 11/30/91   cjg	Broke the file menu.c up into separate files.
 *			menu.c	   master - memory management
 *			menubar.c  menubar routines
 *			popmenu.c  popup routines
 *			submenu.c  submenu routines
 *			evntmenu.c events, blits and scrolling
 * 12/04/91   cjg	-Added MenuScrollAdjust() to speed up scrolling.
 *			-RestoreMenu() adjusted to take advantage
 *			 of those menus <= MAX_MENU_HEIGHT and stays
 *			 the course for those > MAX_MENU_HEIGHT.
 *			-Fixed nasty bug with menus that scroll that
 *			 have roots != 0. See SetMenuHeight().
 * 01/13/92   cjg	-Only those menus with the Last Child as LASTOB
 *			 will be updated with a LASTOB type.
 * 01/15/92   cjg	-MenuIDs are dynamically allocated into a
 *			 linked list at Menu_Insert() and removed at
 *			 Menu_Delete().
 * 01/27/92   cjg       - added comments before the storm.
 * 01/29/92   cjg	- Started conversion to Alcyon ( AES Version )
 * 02/19/92   cjg	- merged into AES
 * 03/24/92   cjg       - max height variable
 * 04/01/92   cjg	- Added Scroll Field to InitMenuNode()
 * 05/15/92   cjg       - Use internal AES calls.
 * 07/01/92   cjg       - The Scroll Flag now designates the starting
 *			  object that will scroll. Its no longer
 *			  the whole menu that must scroll.
 *			- CountMenuItems()s changed to take into accnt
 *			  the Scroll Flag
 * 07/30/92   cjg       - Removed auto-initialization of global variables
 * 8/3/92     cjg	- mn_init init MenuBar_Mode 	
 * 01/13/93   cjg	- Changed ObFlag() calls for LASTOB to
 *			  MakeLast() and NoLast()
 *			- Added MTOP_FLAG() and MB_FLAG().
 *			- SUBMENU ( BIT 11 ) SET in ObFlag() signifies
 *			  that a submenu is attached. The menu id is
 *			  in the extend object type.
 */


/* INCLUDE FILES
 * ================================================================
 */
#include <portab.h>
#include <machine.h>
#include <struct88.h>
#include <baspag88.h>
#include <obdefs.h>
#include <taddr.h>
#include <gemlib.h>
#include <osbind.h>
#include <mn_tools.h>


/* GLOBALS
 * ================================================================
 */
MENU_PTR MenuList;						/* Pointer to the head of the linked list */

				  /* that contains the structures of the    */
				  /* menus that are to be displayed, such as */
				  /* the ObRect, blit buffer etc...         */
static int16_t MAX_MENU_HEIGHT;					/* The menu height we start scrolling     */


/* PROTOTYPES
 * ================================================================
 */


VOID InitMenuNode PROTO((MENU_PTR MenuPtr));
int16_t GetNewMenu PROTO((NOTHING));
int16_t FindNewMenuID PROTO((NOTHING));
VOID SetMenuHeight PROTO((MENU_PTR MenuPtr, int16_t start_obj, BOOLEAN adjust_flag));
VOID PushArrowText PROTO((MENU_PTR MenuPtr));
VOID PopArrowText PROTO((MENU_PTR MenuPtr));



/* FUNCTIONS
 * ================================================================
 */

/* Menu_Insert()
 * ================================================================
 * Gets a free menu list slot and inserts the OBJECT tree into it.
 * Parent - is the parent of the menu children. Therefore, the
 * Menu doesn't have to start at ROOT.
 * The structure is also initialized.
 *
 * The Menu_Delete() call should be made later at some point to
 * remove the malloc'ed memory that this call creates.
 *
 * IN: OBJECT *tree  - the tree in question
 *     int16_t   Parent - the menu object
 *
 * OUT: SUCCESS - returns a Menu ID ranging from 1 - 32767
 *      FAILURE - returns 0
 */
int16_t Menu_Insert(P(OBJECT *) tree, P(int16_t) Parent)
PP(OBJECT *tree;)							/* tree in question      */
PP(int16_t Parent;)							/* The menu object       */
{
	int16_t MenuID;						/* the Menu ID #         */
	register MENU_PTR MenuPtr;				/* ptr to the Menu Node  */

	if ((MenuID = GetNewMenu()) > NULL)	/* Get a new Menu ID     */
	{									/* Yes!              */
		MenuPtr = GetMenuPtr(MenuID);	/* Get a ptr to this node */
		InitMenuNode(MenuPtr);			/* Init the node         */

		MMENU_ID(MenuPtr) = MenuID;		/* Set the Menu ID#      */
		MTREE(MenuPtr) = tree;			/* and the TREE!         */
		MPARENT(MenuPtr) = Parent;		/* and the Menu Object   */

		MFIRST_CHILD(MenuPtr) = ObHead(Parent);	/* Set the 1st  */
		MLAST_CHILD(MenuPtr) = ObTail(Parent);	/* last children */
		MLASTFLAG(MenuPtr) = ObFlags(ObTail(Parent));

		MOFFSET(MenuPtr) = MFIRST_CHILD(MenuPtr);	/* set the  */
		MSTART_OBJ(MenuPtr) = MFIRST_CHILD(MenuPtr);	/* start obj */
		return (MenuID);
	}
	return (NULL);
}



/* Menu_Delete()
 * ================================================================
 * Deletes the given MenuID from the linked list.
 *
 * IN: int16_t MenuID - The Menu ID of the node we want to delete
 *
 * OUT: VOID
 */
VOID Menu_Delete(P(int16_t) MenuID)
PP(int16_t MenuID;)							/* MenuID of node to delete */
{
	register MENU_PTR ptr;					/* Temp Menu Ptr    */
	register MENU_PTR MenuPtr;				/* Current Menu Ptr */

	if ((MenuPtr = GetMenuPtr(MenuID)) > NULL)	/* Get the Ptr to the node */
	{									/* YUP!                   */
		if (MBUFFER(MenuPtr))			/* If the menu buffer is  */
			dos_free(MBUFFER(MenuPtr));	/* still around, delete it */
		MBUFFER(MenuPtr) = NULL;		/* and clear the variable */

		ptr = MenuList;					/* Get the ptr to the head */
		if (ptr == MenuPtr)				/* Is the Node the 1st one */
		{								/* Yes! so update the ptr */
			MenuList = MNEXT(ptr);		/* and give us a new head */
			dos_free(ptr);				/* So Long to the Node    */
		} else							/* Its not the first */
		{
			while (ptr && (MNEXT(ptr) != MenuPtr))	/* So search through */
				ptr = MNEXT(ptr);		/* the linked list   */
			if (MNEXT(ptr) == MenuPtr)	/* Until we find it  */
			{							/* and delete it!    */
				MNEXT(ptr) = MNEXT(MenuPtr);
				dos_free(MenuPtr);
			}
		}
	}
}




/*
 * ================================================================
 * INTERNAL ROUTINES
 * ================================================================
 */

/* mn_init()
 * ================================================================
 * Initializes the use of the Heirarchical and Popup System.
 * The AES needs to call this at boot time.
 *
 * IN: VOID
 * OUT: VOID
 */
VOID mn_init(NOTHING)
{
	/* These are required to make the menu system work and strut its stuff */
	Init_Delays();
	SetMaxHeight(INIT_MAX_HEIGHT);
	MenuList = NULL;					/* Clear out the Master Menu Node Pointer */
	IndexList = NULL;				/* Clear the SubMenu Index Master Pointer */
	MenuBar_Mode = 0;					/* 8/3/92 */
}



/*
 * ================================================================
 * MENU ALLOCATION ROUTINES
 * ================================================================
 */

/* InitMenuNode()
 * ================================================================
 * Initializes a new Menu Node. For a detailed description of each
 * field, see TOOLS.H for a complete summary of the fields.
 *
 * IN: MENU_PTR MenuPtr - pointer to the menu node that we want to init.
 *
 * OUT: VOID
 */
VOID InitMenuNode(P(MENU_PTR) MenuPtr)
PP(register MENU_PTR MenuPtr;)					/* ptr to the Menu Node */
{
	MMENU_ID(MenuPtr) = NIL;
	MTREE(MenuPtr) = NULL;

	MSTART_OBJ(MenuPtr) = NIL;
	MNUM_ITEMS(MenuPtr) = 0;

	MXPOS(MenuPtr) = 0;
	MYPOS(MenuPtr) = 0;
	MWIDTH(MenuPtr) = 0;
	MHEIGHT(MenuPtr) = 0;

	MOFFSET(MenuPtr) = NIL;

	MPARENT(MenuPtr) = NIL;
	MFIRST_CHILD(MenuPtr) = NIL;
	MLAST_CHILD(MenuPtr) = NIL;
	MLASTFLAG(MenuPtr) = 0;

	MTOP_OBJ(MenuPtr) = NIL;
	MTOP_STATE(MenuPtr) = NORMAL;
	MTOP_FLAG(MenuPtr) = NONE;
	MTOP_TXT(MenuPtr)[0] = '\0';

	MB_OBJ(MenuPtr) = NIL;
	MB_STATE(MenuPtr) = NORMAL;
	MB_FLAG(MenuPtr) = NONE;
	MB_TXT(MenuPtr)[0] = '\0';

	MBUFFER(MenuPtr) = NULL;
	MPREV(MenuPtr) = NULL;
	MNEXT(MenuPtr) = NULL;

	MSCROLL(MenuPtr) = FALSE;
}


/* GetNewMenu()
 * ================================================================
 * This routine mallocs the node that the menu structure
 * will reside upon and must be deleted with the MenuDelete call.
 * The new node is attached to the linked list of Menu IDs.
 * 
 * IN: VOID
 * OUT: SUCCESS - A Menu ID ranging from 1 to 32767
 *      FAILURE - 0 error
 */
int16_t GetNewMenu(NOTHING)
{
	register MENU_PTR ptr;					/* Ptr to a temp node           */
	register MENU_PTR newptr;				/* Ptr to the New malloced node */
	int16_t MenuID;						/* The New Menu ID for the node */

	/* Malloc a new node!           */
	if ((newptr = (MENU_PTR) dos_alloc((int32_t) sizeof(MENU_NODE))) == NULL)
		return (NULL);					/* error - no memory for menuid! */

	if ((MenuID = FindNewMenuID()) == NULL)	/* Get us a new and unused ID!   */
	{
		dos_free(newptr);				/* free the node         */
		return (NULL);					/* a zero to the powers above.   */
	}

	ptr = MenuList;						/* Set the temp to the head.     */

	if (ptr == NULL)					/* Its NULL! so make our new node */
		MenuList = newptr;				/* the king of the hill!...      */
	else								/* Oops-we have to go look for   */
	{									/* the last node and attach the  */
		while (MNEXT(ptr))				/* new node to the end of the    */
			ptr = MNEXT(ptr);			/* linked list.          */
		MNEXT(ptr) = newptr;
	}
	MMENU_ID(newptr) = MenuID;			/* Set the Menu ID into the node */
	return (MenuID);					/* and return!                   */
}


/* FindNewMenuID()
 * ================================================================
 * Finds a new available menuid.  It looks down the linked list and
 * finds the last menuID and returns the last known menuid + 1.
 * If no menuIDs exist, it returns 1 as the first MenuID.
 * If the MenuID returns is greater than MENU_MAX, then return 0
 *
 * The MenuIDs should be in ascending order since they are deleted
 * once the menu is removed.
 *
 * IN:  VOID
 * OUT: Returns the Menu ID - returns 1 if this is the first menu.
 *      Returns 0 if there is an error. ie - > 32767 menu ids
 */
int16_t FindNewMenuID(NOTHING)
{
	register MENU_PTR ptr;					/* Temp Menu Node Pointer        */
	int16_t MenuID;						/* A variable menu ID            */
	ptr = MenuList;						/* Set the temp to the head      */

	if (ptr == NULL)					/* This is the first Menu Node!  */
		return (1);
	else
	{									/* Find the last MenuID and add 1 */
		while (MNEXT(ptr))				/* Get it by going down the      */
			ptr = MNEXT(ptr);			/* linked list and seeing the    */
		if (ptr)						/* MenuIDs of existing nodes     */
			MenuID = MMENU_ID(ptr);

		if (MenuID < MENU_MAX)			/* return the id if its < 32767 */
			return (MenuID + 1);
		else
			return (NULL);				/* otherwise, return 0!         */
	}
}



/* GetMenuPtr()
 * ================================================================
 * Looks for a MenuID in the MenuList and returns a MenuPtr to it.
 * returns NULL if the MenuID can't be found.
 *
 * IN:  int16_t MenuID - The menu id that we are looking for.
 *
 * OUT: MENU_PTR - SUCCESS - returns a pointer to the node.
 *                 FAILURE - returns NULLPTR.
 */
MENU_PTR GetMenuPtr(P(int16_t) MenuID)
PP(int16_t MenuID;)							/* the menu id we want...    */
{
	register MENU_PTR ptr;					/* Temp Pointer              */

	ptr = MenuList;						/* Set us to the head.       */

	if (!MenuID || !MenuList)			/* Ensure a valid list and id */
		return ((MENU_PTR) NULL);

	while (ptr)							/* Go through the linked list */
	{									/* checking menu ids. If fnd */
		if (MMENU_ID(ptr) == MenuID)	/* return it! otherwise we'll */
			break;						/* evntually return NULL  */
		ptr = MNEXT(ptr);
	}
	return (ptr);
}





/*
 * ================================================================
 * MENU OBJECT STRUCTURE HANDLING
 * ================================================================
 */

/* CheckMenuHeight()
 * ================================================================
 * The menus will be constructed with arrows to scroll them if they
 * exceed the MAX_MENU_HEIGHT. The menus will also be cut down to
 * the MAX_MENU_HEIGHT.
 *
 * IN: MENU_PTR MenuPtr - The ptr to the Menu Node that we need to check.
 * OUT: VOID
 */
VOID CheckMenuHeight(P(MENU_PTR) MenuPtr)
PP(register MENU_PTR MenuPtr;)					/* ptr to the Menu Node   */
{
	register OBJECT *tree;

	ActiveTree(MTREE(MenuPtr));

	MNUM_ITEMS(MenuPtr) = CountMenuItems(MenuPtr);	/* Check # of items */

	MWIDTH(MenuPtr) = ObW(MPARENT(MenuPtr));	/* Set width and the */
	MHEIGHT(MenuPtr) = ObH(MPARENT(MenuPtr));	/* height ( init )  */
	if (MNUM_ITEMS(MenuPtr) > MAX_MENU_HEIGHT)	/* if > MAX_MENU HT */
		SetMenuHeight(MenuPtr, MSTART_OBJ(MenuPtr), TRUE);	/* Set it to it */
}


/* CountMenuItems()
 * ================================================================
 * Get the Number of Menu Items in this menu based off of the parent.
 *         Last Child - First Child + 1 == Num of Menu Items.
 *
 * IN:  MENU_PTR MenuPtr - Ptr to the menu node in question
 * OUT: returns the number of menu items in this menu.
 */
int16_t CountMenuItems(P(MENU_PTR) MenuPtr)
PP(register MENU_PTR MenuPtr;)					/* ptr to the Menu Node */
{
	register OBJECT *tree;					/* tree to count on     */
	register int16_t parent;					/* menu object      */
	int16_t NumItems;						/* Num Items in picture. */
	int16_t obj;

	ActiveTree(MTREE(MenuPtr));
	parent = MPARENT(MenuPtr);

	NumItems = 0;
	obj = ObHead(parent);
	while (obj != parent)
	{
		NumItems++;
		obj = ObNext(obj);
	}
	return (NumItems);
}



/* SetMenuHeight()
 * ================================================================
 * Sets the menu height to MAX_MENU_HEIGHT and adjust it accordingly.
 * Especially, since we have to, or might, start with both
 * scroll arrows showing.
 *
 * IN: MENU_PTR MenuPtr   - the menu ptr in question
 *     int16_t     start_obj - the menu item that we should have at the top.
 *     BOOLEAN  adjust_flag -  TRUE - take into account the start obj.
 *
 * OUT: VOID
 */
VOID SetMenuHeight(P(MENU_PTR) MenuPtr, P(int16_t) start_obj, P(BOOLEAN) adjust_flag)
PP(register MENU_PTR MenuPtr;)				/* ptr to the menu node    */
PP(int16_t start_obj;)						/* menu item at the top    */
PP(BOOLEAN adjust_flag;)					/* see above...            */
{
	register OBJECT *tree;				/* object tree in question */
	register int16_t obj;				/* temp object             */
	int16_t parent;						/* menu object             */
	int16_t offset_obj, temp;			/* Offset menu item        */

	ActiveTree(MTREE(MenuPtr));
	parent = MPARENT(MenuPtr);

	/* Find the offset object necessary */
	offset_obj = start_obj;
	if (adjust_flag)
	{
		/* We've added startobj - MPARENT so that we can adjust
		 * for those menus who don't start with root == 0.
		 */

		/* If the start_obj is less than the scrolling starting object,
		 * just position the offset to the first child.
		 */
		if (start_obj <= MSCROLL(MenuPtr))
		{
			offset_obj = MFIRST_CHILD(MenuPtr);
		} else
		{

			/* Now, take into account the height of the new menu...
			 * and adjust the offset if necessary.
			 */
			if (MSCROLL(MenuPtr) == MFIRST_CHILD(MenuPtr))
			{
				/* The starting object is within the scroll area */
				offset_obj = ((start_obj - parent +
							   (MNUM_ITEMS(MenuPtr) > MAX_MENU_HEIGHT)) / MAX_MENU_HEIGHT) + MFIRST_CHILD(MenuPtr);

				if (offset_obj != MFIRST_CHILD(MenuPtr))
				{
					offset_obj = start_obj - 1;
					temp = MNUM_ITEMS(MenuPtr) - MAX_MENU_HEIGHT + MFIRST_CHILD(MenuPtr);
					if (offset_obj >= temp)
					{
						offset_obj = temp;
					}
				}
			} else
			{
				/* The starting object is within the scroll area */
				offset_obj = ((start_obj - parent +
							   (MNUM_ITEMS(MenuPtr) > MAX_MENU_HEIGHT)) / MAX_MENU_HEIGHT) + MSCROLL(MenuPtr);
				if (offset_obj != MSCROLL(MenuPtr))
				{
					offset_obj = start_obj - 1;
					temp = MNUM_ITEMS(MenuPtr) - MAX_MENU_HEIGHT + MSCROLL(MenuPtr);
					if (offset_obj >= temp)
					{
						offset_obj = temp;
					}
				}
			}
		}
	}

	/* Delete the menu items from MSCROLL to offset object.
	 * Naturally, if the offset is NOT within the scroll area, 
	 * we fall thru....
	 */
	for (obj = MSCROLL(MenuPtr); obj < offset_obj; obj++)
	{
		ObH(parent) -= ObH(obj);		/* fixup height of root menu */
		ob_delete(tree, obj);			/* delete object in question */
	}

	/* update the yoffset starting from the offset object + MSCROLL()
	 * by fixing up based upon the height based on height of text
	 * Do this ONLY if the offset is > than the scroll...
	 */
	if (offset_obj > MSCROLL(MenuPtr))
	{
		for (obj = offset_obj; obj <= MLAST_CHILD(MenuPtr); obj++)
			ObY(obj) = (obj - offset_obj + (MSCROLL(MenuPtr) - MFIRST_CHILD(MenuPtr))) * ObH(obj);
	}


	MTOP_OBJ(MenuPtr) = max(MSCROLL(MenuPtr), offset_obj);
/*
    if(( offset_obj > MSCROLL( MenuPtr ) ) && (MSCROLL( MenuPtr ) != MFIRST_CHILD( MenuPtr )))
 */
	if ((start_obj > MSCROLL(MenuPtr)) && (MSCROLL(MenuPtr) != MFIRST_CHILD(MenuPtr)))
	{
		MB_OBJ(MenuPtr) = min((((MAX_MENU_HEIGHT - (MSCROLL(MenuPtr) - MFIRST_CHILD(MenuPtr)))
								+ offset_obj) - 1), MLAST_CHILD(MenuPtr));
	} else
	{
		MB_OBJ(MenuPtr) = min(offset_obj + MAX_MENU_HEIGHT - 1, MLAST_CHILD(MenuPtr));
	}

	/* Now, shorten the menu to the new height reducing the size of the
	 * root menu and deleting the excess menu items.
	 */
	for (obj = MLAST_CHILD(MenuPtr); obj > MB_OBJ(MenuPtr); obj--)
	{
		ObH(parent) -= ObH(obj);
		ob_delete(tree, obj);
	}

	if (MLASTFLAG(MenuPtr))				/* CJG 01/13/92 */
		MTREE(MenuPtr)[MB_OBJ(MenuPtr)].ob_flags |= LASTOB;
	PushArrowText(MenuPtr);

	MOFFSET(MenuPtr) = max(offset_obj, MSCROLL(MenuPtr));
	/* Adjust the height of the GRECT to be set to new adjusted height */
	MHEIGHT(MenuPtr) = ObH(parent);
}


/* RestoreMenu()
 * ================================================================
 * Restores the Menu...to its original form.
 * Should only be called if the menu was modified to fit
 * into the MAX_MENU_HEIGHT ( Scrollable )
 *
 * IN:  MENU_PTR MenuPtr - Pointer to the menu node in question.
 * OUT: VOID
 */
VOID RestoreMenu(P(MENU_PTR) MenuPtr)
PP(register MENU_PTR MenuPtr;)					/* ptr to the menu node... */
{
	register OBJECT *tree;					/* the tree to set this to */
	register int16_t obj;						/* temp object             */
	int16_t parent;						/* The menu object         */
	int16_t count;							/* The # of items          */
	int16_t temp;

	ActiveTree(MTREE(MenuPtr));
	parent = MPARENT(MenuPtr);

	/* Return immediately if the menu is not modified. The only ones
	 * That need this are menus with num items <= MAX_MENU_HEIGHT.
	 * Just make sure that they are deselected.
	 */
	count = CountMenuItems(MenuPtr);

	/* Means that it was not modified for MAX_MENU_HEIGHT, so do just this */
	if (MNUM_ITEMS(MenuPtr) == count)
	{
		for (obj = MFIRST_CHILD(MenuPtr); obj <= MLAST_CHILD(MenuPtr); obj++)
		{
			if (IsSelected(obj))
				Deselect(obj);
		}
		if (IsSelected(parent))
			Deselect(parent);
		return;
	}

	/* Fix up menus that have a height > MAX_MENU_HEIGHT 
	 * Restore the text that occupied the original menu items, if required
	 * If we got this far, it is required....
	 */
	PopArrowText(MenuPtr);

	obj = ObHead(parent);
	while (obj != parent)
	{
		temp = ObNext(obj);
		ob_delete(tree, obj);
		ObH(parent) -= ObH(obj);
		obj = temp;
	}

	/* Add in all the menu items again... */
	for (obj = MFIRST_CHILD(MenuPtr); obj <= MLAST_CHILD(MenuPtr); obj++)
	{
		ob_add(tree, parent, obj);
		if (IsSelected(obj))
			Deselect(obj);
		ObH(parent) += gl_hchar;
		NoLast(obj);
		ObY(obj) = (obj - MFIRST_CHILD(MenuPtr)) * ObH(MFIRST_CHILD(MenuPtr));
	}

	/* Refix the LASTOB pointer in the REAL tree format */
	if (MLASTFLAG(MenuPtr))				/* CJG 01/13/92 */
		MTREE(MenuPtr)[MLAST_CHILD(MenuPtr)].ob_flags |= LASTOB;

	/* Adjust the height of the GRECT to be set to new adjusted height */
	MHEIGHT(MenuPtr) = ObH(parent);
}



/* AdjustMenuPosition()
 * ================================================================
 * Positions the popup/submenu based on the xpos, ypos handed in.
 *
 * 1) If there are more than 18 menu items, scroll arrows will
 *    automatically be supplied. ( MAX_MENU_HEIGHT )
 * 2) The menu will be adjusted so that the start menu item
 *    is positioned at xpos, ypos. ( Upper Left corner ).
 * 3) GRECT rect - rect of the box. Good only for HORIZONTAL FLAG == TRUE.
 *    The menus, if TRUE, will pop left instead of right.
 * 4) Horizontal_Flag
 *    FALSE - The menu is positioned according to xpos.
 *    TRUE  - Position the menu onto a byte-aligned position.
 *	      If the menu is off the screen, reposition back-on.
 * 5) Vertical_Flag
 *    FALSE - The menu is positioned according to rule #2.
 *    TRUE  - The menu is adjusted upwards if it exceeds the height
 *	      of the screen.
 *	      The menu is adjusted downwards if it exceeds the top 
 * 	      of the screen.
 */
VOID AdjustMenuPositio(P(MENU_PTR) MenuPtr, P(int16_t) xpos, P(int16_t) ypos, P(GRECT *) rect, P(BOOLEAN) Horizontal_Flag, P(BOOLEAN) Vertical_Flag)
PP(register MENU_PTR MenuPtr;)					/* ptr to the Menu Node         */
PP(int16_t xpos;)								/* xpos that we originally want */
PP(int16_t ypos;)								/* ypos that we originally want */
PP(GRECT *rect;)							/* GRECT of the button          */
PP(BOOLEAN Horizontal_Flag;)				/* See above...         */
PP(BOOLEAN Vertical_Flag;)					/* See above...         */
{
	register OBJECT *tree;					/* Local tree variable   */
	register int16_t parent;					/* menu object           */
	int16_t tempx;							/* temp x and y positions */
	int16_t tempy;

	ActiveTree(MTREE(MenuPtr));			/* Set the tree...     */
	parent = MPARENT(MenuPtr);			/* get the menu object */

	/*
	 *----------------------------------------------------------------
	 * ADJUST IN THE HORIZONTAL POSITION
	 *----------------------------------------------------------------
	 */
	ObX(parent) = MXPOS(MenuPtr) = xpos;
	if (Horizontal_Flag)
	{
		/* Check the menu to see if it exceeds the right of the screen.
		 * if it does, we flop it left, instead of right.
		 */
		tempy = gl_rfull.g_x + gl_rfull.g_w - (gl_wchar / 2) - 1;
		tempx = ObX(parent) + ObW(parent) - 1;
		if (tempx > tempy)
			MXPOS(MenuPtr) = ObX(parent) = rect->g_x - ObW(parent);

		/* Check and adjust if the menu exceeds the left of the screen.  */
		while (ObX(parent) < gl_wchar)
		{
			ObX(parent) = MXPOS(MenuPtr) = (ObX(parent) + gl_wchar);
		}

		/* Adjust onto a byte-aligned xpos */
		tempx = ((ObX(parent) + 7) / 8) * 8;
		ObX(parent) = MXPOS(MenuPtr) = tempx;

	}

  /*------------------------------------------------------------------
   * ADJUST IN THE VERTICAL POSITION
   *------------------------------------------------------------------
   */
	ObY(parent) = MYPOS(MenuPtr) = ypos - ObY(MSTART_OBJ(MenuPtr));
	if (Vertical_Flag)					/* Check and adjust if the menu exceeds the */
	{									/* Screen dimensions.               */
		/* Check and adjust if the menu exceeds the top of the menu */
		tempy = gl_rfull.g_y;
		while (ObY(parent) < tempy)
			ObY(parent) = MYPOS(MenuPtr) = (ObY(parent) + gl_hchar);

		/* Check and adjust if menu exceeds the bottome of the menu */
		tempx = gl_rfull.g_y + gl_rfull.g_h - (gl_hchar / 2) - 1;
		tempy = ObY(parent) + ObH(parent) - 1;
		while (tempy > tempx)
		{
			MYPOS(MenuPtr) = ObY(parent) = (ObY(parent) - gl_hchar);
			tempy = ObY(parent) + ObH(parent) - 1;
		}
	}
}



/* MenuScrollAdjust()
 * ================================================================
 * Adjust the resource menu tree while scrolling up and down.
 * This is MUCH quicker than just doing a RestoreMenu().
 * This routine scrolls by adding and deleting objects on the fly.
 *
 * IN: MENU_PTR MenuPtr - Ptr to the menu node in question.
 *     int16_t     start_obj - the menu item that we want on top
 *
 * OUT: VOID
 */
VOID MenuScrollAdjust(P(MENU_PTR) MenuPtr, P(int16_t) start_obj)
PP(register MENU_PTR MenuPtr;)					/* ptr to the menu node in question */
PP(int16_t start_obj;)							/* the menu item that we want on top */
{
	register int16_t obj;						/* temp obj variable       */
	int16_t offset_obj;					/* the offset/start object */
	int16_t new_bottom;					/* the new bottom object   */

	/* Restore the text that occupied the original menu items, if required */
	PopArrowText(MenuPtr);

	/* Find the Direction of the Scroll */
	offset_obj = start_obj;

	if (offset_obj < MTOP_OBJ(MenuPtr))
	{
		/*  UP ARROW SCROLL */

		/* Add in the objects preceding up to offset_obj */
		for (obj = offset_obj; obj < MTOP_OBJ(MenuPtr); obj++)
			ob_add(MTREE(MenuPtr), MPARENT(MenuPtr), obj);
		MTOP_OBJ(MenuPtr) = offset_obj;

		/* Delete the bottom object or objects. */
		new_bottom = min((MAX_MENU_HEIGHT - MSCROLL(MenuPtr)) + start_obj, MLAST_CHILD(MenuPtr));

		for (obj = (new_bottom + 1); obj <= MB_OBJ(MenuPtr); obj++)
			ob_delete(MTREE(MenuPtr), obj);
		MB_OBJ(MenuPtr) = new_bottom;
	} else
	{
		/* DOWN ARROW SCROLL */

		/* Delete the starting objects. */
		for (obj = MTOP_OBJ(MenuPtr); obj < offset_obj; obj++)
			ob_delete(MTREE(MenuPtr), obj);
		MTOP_OBJ(MenuPtr) = offset_obj;

		/* Add in the bottom objects */
		new_bottom = min((MAX_MENU_HEIGHT - MSCROLL(MenuPtr)) + start_obj, MLAST_CHILD(MenuPtr));
		for (obj = MB_OBJ(MenuPtr) + 1; obj <= new_bottom; obj++)
			ob_add(MTREE(MenuPtr), MPARENT(MenuPtr), obj);
		MB_OBJ(MenuPtr) = new_bottom;
	}

	/* Adjust the ypos offsets */
	for (obj = MTOP_OBJ(MenuPtr); obj <= MB_OBJ(MenuPtr); obj++)
	{
		MTREE(MenuPtr)[obj].ob_state &= ~SELECTED;

		MTREE(MenuPtr)[obj].ob_y = ((obj - MTOP_OBJ(MenuPtr)) +
									(MSCROLL(MenuPtr) - MFIRST_CHILD(MenuPtr)))
			* MTREE(MenuPtr)[MTOP_OBJ(MenuPtr)].ob_height;

		MTREE(MenuPtr)[obj].ob_flags &= ~LASTOB;
	}

	/* Set the LASTOB only if the menu started out that way. */
	if (MLASTFLAG(MenuPtr))				/* CJG 01/13/92 */
		MTREE(MenuPtr)[MB_OBJ(MenuPtr)].ob_flags |= LASTOB;

	PushArrowText(MenuPtr);				/* Save the text under */
	/* the arrows...       */
	MOFFSET(MenuPtr) = offset_obj;		/* set the new offset  */
}



/* PushArrowText()
 * ================================================================
 * Saves the text that is under the proposed up and down arrows.
 * 
 * IN: MENU_PTR MenuPtr - the menu node in question
 *
 * OUT: VOID
 */
VOID PushArrowText(P(MENU_PTR) MenuPtr)
PP(register MENU_PTR MenuPtr;)					/* ptr to the menu node in question */
{
	register int16_t obj;						/* temp obj variable  */

	/* Set the Up ARROW if the first object is not the FIRST_CHILD */
	obj = MTOP_OBJ(MenuPtr);
	MTOP_STATE(MenuPtr) = MTREE(MenuPtr)[obj].ob_state;
	MTOP_FLAG(MenuPtr) = MTREE(MenuPtr)[obj].ob_flags;

	if (obj != MSCROLL(MenuPtr))
	{
		MTREE(MenuPtr)[obj].ob_state = NORMAL;
		MTREE(MenuPtr)[obj].ob_flags &= ~SUBMENU;

		/* AES version - src,dst */
		xstrpcpy((char *) MTREE(MenuPtr)[obj].ob_spec, &MTOP_TXT(MenuPtr)[0]);
		xstrpcpy(&UpText[0], (char *) MTREE(MenuPtr)[obj].ob_spec);
	}

	/* Set the down ARROW if the last object is not MLAST_CHILD */
	obj = MB_OBJ(MenuPtr);
	MB_STATE(MenuPtr) = MTREE(MenuPtr)[obj].ob_state;
	MB_FLAG(MenuPtr) = MTREE(MenuPtr)[obj].ob_flags;

	if (obj != MLAST_CHILD(MenuPtr))
	{
		MTREE(MenuPtr)[MB_OBJ(MenuPtr)].ob_state = NORMAL;
		MTREE(MenuPtr)[MB_OBJ(MenuPtr)].ob_flags &= ~SUBMENU;

		if (MLASTFLAG(MenuPtr))			/* CJG 01/13/92 */
			MTREE(MenuPtr)[obj].ob_flags |= LASTOB;
		/* AES version - src,dst */
		xstrpcpy((char *) MTREE(MenuPtr)[obj].ob_spec, &MB_TXT(MenuPtr)[0]);
		xstrpcpy(&DownText[0], (char *) MTREE(MenuPtr)[obj].ob_spec);
	}
}


/* PopArrowText()
 * ================================================================
 * Restores the text that the up and down arrows replaced.
 * Called by: RestoreMenu() and MenuScrollAdjust().
 *
 * IN: MENU_PTR MenuPtr - Ptr to the menu node in question
 *
 * OUT: VOID
 */
VOID PopArrowText(P(MENU_PTR) MenuPtr)
PP(register MENU_PTR MenuPtr;)					/* ptr to the menu node */
{
	register int16_t obj;						/* temp object variable */

	/* Restore the text underneath the UP ARROW */
	if ((obj = MTOP_OBJ(MenuPtr)) != MSCROLL(MenuPtr))
	{
		xstrpcpy(&MTOP_TXT(MenuPtr)[0], (char *) MTREE(MenuPtr)[obj].ob_spec);
		MTREE(MenuPtr)[obj].ob_state = MTOP_STATE(MenuPtr);
		MTREE(MenuPtr)[obj].ob_flags = MTOP_FLAG(MenuPtr);
	}

	/* Restore the text underneath the BOTTOM ARROW */
	if ((obj = MB_OBJ(MenuPtr)) != MLAST_CHILD(MenuPtr))
	{
		MTREE(MenuPtr)[obj].ob_flags = NONE;
		xstrpcpy(&MB_TXT(MenuPtr)[0], (char *) MTREE(MenuPtr)[obj].ob_spec);
		MTREE(MenuPtr)[obj].ob_state = MB_STATE(MenuPtr);
		MTREE(MenuPtr)[obj].ob_flags = MB_FLAG(MenuPtr);
	}
}


/* SetMaxHeight()
 * ================================================================
 * Set the Maximum Height of the submenus when they are to start
 * scrolling. Minimum height is MIN_HEIGHT and maximum height
 * is the maximum height of the screen.
 * IN:    < 0 - will return the current value.
 * OUT: returns the value that we set it to.
 */
int16_t SetMaxHeight(P(int16_t) height)
PP(int16_t height;)
{
	int16_t max_height;

	if (height < 0)
		return (MAX_MENU_HEIGHT);

	MAX_MENU_HEIGHT = height;

	/* Must have a certain minimum */
	if (height <= MIN_HEIGHT)
		MAX_MENU_HEIGHT = MIN_HEIGHT;

	/* See if we exceed the height of the screen */
	max_height = ((gl_rfull.g_h - (gl_hchar / 2) - 1) / gl_hchar);
	if (height >= max_height)
		MAX_MENU_HEIGHT = max_height;

	return (MAX_MENU_HEIGHT);
}
