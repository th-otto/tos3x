/* MN_SUBMN.C
 * ================================================================
 * DESCRIPTION: These are the submenu specific routines
 *
 * 11/30/91  cjg   - created
 * 12/04/91  cjg   - Submenus work just fine
 * 01/13/92  cjg   - Removed User-Defined MenuIDs
 * 01/15/92  cjg   - Installed Index nodes
 * 01/17/92  cjg   - Added several comments
 * 01/23/92  cjg   - moved index routines to its own file
 * 01/29/92  cjg   - Started conversion to Alcyon ( AES Version )
 * 02/19/92  cjg   - merged into AES
 * 02/25/92  cjg   - made sure that we deal only with G_STRING
 * 03/24/92  cjg   - modified SetDisplayDelay and SetDragDelay
 *		     to new names and to make them accessible
 *		     thru the AES.
 *		   - Added the arrow scroll delay and the scroll delay
 * 04/01/92  cjg   - added menu_attach() to cut down on the number of
 *		     AES calls used.
 *		   - modified menu_setmn() and menu_getmn()
 *		   - added menu_istart()
 *		   - modified menu_iset() and menu_iget()
 * 		   - added menu_settings()
 * 05/06/92  cjg   - Modified SetDragDelay() and SetDisplayDelay() to use
 *		     the actual ms passed in.
 * 05/11/92  cjg   - Limited the number of submenus that can be 
 *		     recursive to MAX_LEVEL.
 * 05/14/92  cjg   - Pass in process id's to appropriate functions
 * 05/15/92  cjg   - use internal AES function calls
 * 07/01/92  cjg   - Make sure starting scroll object is within limits
 * 07/30/92  cjg   - removed auto-initialization of global variables
 * 09/22/92  cjg   - Clip Menu draw to intersection of menu and full
 * 01/13/93  cjg   - Set ObFlag() to 'SUBMENU' if a submenu is attached.
 *		   - The Menu ID is still at the Extended Object Type byte.
 *		   - Add check for the ObFlag & SUBMENU
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


/* PROTOTYPES
 * ================================================================
 */



/* GLOBALS
 * ================================================================
 */
int32_t SUBMENU_DELAY;						/* Delay time for submenus to appear. ( ms ) */

int32_t SUBDRAG_DELAY;						/* Delay time for submenus to go active( ms )
										 * as the user drags the mouse to the menu.
										 */
int32_t SCROLL_DELAY;						/* Delay time to scroll menu items ( ms ) */

int32_t ARROW_DELAY;						/* Delay time to start scroll ( ms )      */


/* PROTOTYPES
 * ================================================================
 */
int16_t mn_iset PROTO((int16_t id, OBJECT *itree, int16_t imenu, int16_t start_obj));
int16_t mn_iget PROTO((int16_t id, OBJECT *tree, int16_t imenu));
BOOLEAN mn_setmn PROTO((int16_t id, OBJECT *tree, int16_t item, MENU *Menu));
BOOLEAN mn_getmn PROTO((int16_t id, OBJECT *tree, int16_t item, MENU *Menu));
VOID DetachSubMenu PROTO((int16_t id, OBJECT *tree, int16_t item));
MENU_PTR ShowSubMenu PROTO((int16_t id, int16_t MenuIndex, int16_t xpos, int16_t ypos, GRECT *rect));
VOID SetDisplayDelay PROTO((int32_t ms, int32_t *oldvalue));
VOID SetDragDelay PROTO((int32_t ms, int32_t *oldvalue));
VOID SetScrollDelay PROTO((int32_t ms, int32_t *oldvalue));
VOID SetArrowDelay PROTO((int32_t ms, int32_t *oldvalue));
BOOLEAN CountLevel PROTO((NOTHING));





/* FUNCTIONS
 * ================================================================
 */



/*
 * AES #38 - menu_istart - Align a submenu entry. 
 *
 * mn_istart()
 * ================================================================
 * Sets and Gets the start item of an attached submenu.
 *
 * flag == 0 - GET starting menu item
 *	       Requires - Menu->mn_tree
 *			- Menu->mn_menu
 *			- place-holder
 *
 *	       RETURNS in D0 - start item
 *
 *
 *      == 1 - SET starting menu item
 *	       Requires - Menu->mn_tree
 *			- Menu->mn_menu
 *			- Menu->mn_item
 *
 *	       RETURNS in D0 - start item
 * RETURN == FALSE == ERROR
 */
int16_t mn_istart(P(int16_t) id, P(int16_t) flag, P(OBJECT *) tree, P(int16_t) menu, P(int16_t) item)
PP(int16_t id;)								/* Process id */
PP(int16_t flag;)								/* Set or Get */
PP(OBJECT *tree;)							/* Tree of the submenu */
PP(int16_t menu;)								/* Menu object of the submenu   */
PP(int16_t item;)								/* Starting item of the submenu */
{
	int16_t result;

	switch (flag)
	{
	case 0:							/* get the starting menu item */
		result = mn_iget(id, tree, menu);
		break;

	case 1:							/* SET the starting menu item */
		result = mn_iset(id, tree, menu, item);
		break;

	default:
		result = 0;
		break;
	}
	return (result);
}



/* mn_iset()
 * ================================================================
 * Sets the start item of an already attached menu.
 *
 * IN: int16_t   id       Process id
 *     OBJECT *itree   The tree of the menu
 *     int16_t   imenu    The menu object ( parent ) of the menu
 *     int16_t   start    The start object to align the menu with.
 *
 * OUT: Returns the starting menu item
 *      0 - FAILURE 
 */
int16_t mn_iset(P(int16_t) id, P(OBJECT *) itree, P(int16_t) imenu, P(int16_t) start_obj)
PP(int16_t id;)								/* Process id              */
PP(OBJECT *itree;)							/* the tree of the menu        */
PP(int16_t imenu;)								/* the menu object of the menu */
PP(int16_t start_obj;)							/* the start obj of the menu   */
{
	int16_t Index;							/* The Index ID                */
	register INDEX_PTR IndexPtr;				/* Ptr to the Index Node       */
	PNODE_PTR ProcPtr;					/* ptr to the process node     */
	OBJECT *tree;

	/* Get the pointer to the process node */
	if ((ProcPtr = FindProcess(id)) > (PNODE_PTR)NULL)
	{
		/* Get the Index ID Number */
		if ((Index = FindIndex(id, itree, imenu)) > 0)
		{
			/* Get the pointer to the index node */
			if ((IndexPtr = GetIndexPtr(ProcPtr, Index)) > (INDEX_PTR)NULL)
			{
				ActiveTree(itree);
				start_obj = min(start_obj, ObTail(imenu));
				start_obj = max(start_obj, ObHead(imenu));
				INDEX_OBJ(IndexPtr) = start_obj;	/* Change the startobj */
				return (start_obj);		/* and exit!          */
			}
		}
	}
	return (FALSE);						/* FAILURE...         */
}



/* mn_iget()
 * ================================================================
 * Get the start menu item object number of an attached menu.
 *
 * IN: int16_t    id    - Process id 
 *     OBJECT *tree  - the tree of the attached menu
 *     int16_t    imenu - the menu object( parent ) of the attach menu
 *
 * OUT:SUCCESS - returns the start object of this menu.
 *     FAILURE - returns 0
 */
int16_t mn_iget(P(int16_t) id, P(OBJECT *) tree, P(int16_t) imenu)
PP(int16_t id;)								/* Process id               */
PP(OBJECT *tree;)							/* the tree of the attached menu    */
PP(int16_t imenu;)								/* the menu obj of the attached menu */
{
	int16_t Index;							/* Temp Index ID           */
	int16_t output;						/* return this startobj    */
	register INDEX_PTR IndexPtr;				/* ptr to the Index node   */
	PNODE_PTR ProcPtr;					/* ptr to the process node */

	output = 0;

	/* Find the process pointer to the node */
	if ((ProcPtr = FindProcess(id)) > (PNODE_PTR)NULL)
	{
		/* Find the Index ID */
		if ((Index = FindIndex(id, tree, imenu)) > 0)
		{
			/* Get the pointer to the Index Node */
			if ((IndexPtr = GetIndexPtr(ProcPtr, Index)) > (INDEX_PTR)NULL)
				output = INDEX_OBJ(IndexPtr);	/* return the value...  */
		}
	}
	return (output);
}


/*
 * AES #37 - menu_attach - Add, alter, delete or obtain information about a submenu.
 *
 * mn_attach()
 * ================================================================
 * Attach, remove, change a submenu that is attached to a menu item.
 * Also, Inquire if a menu has a submenu attached.
 *
 * IN: int16_t id  - Process id  
 *     int16_t flag - 0 - INQUIRE
 *                 1 - ATTACH, REMOVE, CHANGE
 *		   2 - REMOVE
 *     OBJECT *tree  - the object tree of the menu item
 *		       that we are inquiring about.
 *     int16_t   item   - the menu item that we are inquiring about
 *
 *     MENU   *Menu  - (flag==1) - The submenu that we are going to attach.
 *			         - If ( *Menu == NULL ), the submenu attached
 *				   to the above menu item will be removed.
 *		       (flag==0) - If( flag == 0) AND ( OUT == TRUE )
 *				   'Menu' contains the Menu data attached
 *				   to the above menu item upon return.
 *		       (flag==2) - remove the submenu attached to the menu
 *				   item above.
 * OUT: BOOLEAN - TRUE - success
 *		  FALSE - failure
 *	
 */
BOOLEAN mn_attach(P(int16_t) id, P(int16_t) flag, P(OBJECT *) tree, P(int16_t) item, P(MENU *) Menu)
PP(int16_t id;)
PP(int16_t flag;)
PP(OBJECT *tree;)
PP(int16_t item;)
PP(MENU *Menu;)
{
	int16_t result;

	result = FALSE;
	switch (flag)
	{
	case 0:							/* INQUIRE */
		result = mn_getmn(id, tree, item, Menu);
		break;

	case 1:							/* ATTACH, CHANGE, REMOVE */
		result = mn_setmn(id, tree, item, Menu);
		break;

	case 2:							/* REMOVE */
		result = mn_setmn(id, tree, item, NULL);
		break;

	default:
		break;
	}
	return (result);
}



/* mn_setmn()
 * ================================================================
 * Attach a submenu to a menu item.
 *
 * 1) if a submenu is already attached, detach the submenu.
 * 2) if the tree to attach is NULL, then don't attach a new menu.
 *    NOTE: This is one way of clearing out an attached submenu.
 * 3) if the tree is valid, then:
 *    1) the character '0x03'(Right Arrow) is placed ( ARROW_OFFSET )
 *       characters in from the end of the text string.
 *    2) a non-zero index node is placed in the extended object type field.
 *    3) a index node is created, if one doesn't already exist.
 * 4) Scroll-Flag ->  TRUE - if num items > 18, will scroll
 *		      FALSE - don't scroll ever
 *
 * IN:  int16_t   id     -    Process id
 *      OBJECT *tree  -	   Tree that will have a submenu attached.
 *	int16_t   item   -	   the menu item that will have a submenu attached.
 *      OBJECT *itree -    the tree that will be attached.
 *      int16_t   imenu  -	   the menu object that will be attached.
 *
 * OUT: TRUE  - SUCCESS
 *      FALSE - FAILURE
 */
BOOLEAN mn_setmn(P(int16_t) id, P(OBJECT *) tree, P(int16_t) item, P(MENU *) Menu)
PP(int16_t id;)								/* Process id                 */
PP(OBJECT *tree;)							/* tree that will have a submenu attached */
PP(int16_t item;)								/* the menu item that will have a menu    */
PP(MENU *Menu;)
{
	register uint16_t Index;					/* the Extend Object Type         */
	register INDEX_PTR IndexPtr;				/* temp ptr to the index node     */
	MENU MData;
	BOOLEAN flag;						/* test flag for reuse of menu    */
	PNODE_PTR ProcPtr;					/* ptr to the process node        */

	/* Check that the menu item is a G_STRING */
	if (!IsG_String(item))
		return (FALSE);

	/*Check to see if the menu item has something attached to it already */
	if ((flag = mn_getmn(id, tree, item, &MData)) > 0)
	{
		/* This menu item already has something attached, so delete it */
		DetachSubMenu(id, tree, item);

		/* If the Menu == NULL, we really wanted to just remove
		 * the submenu, so return TRUE! SUCCESS
		 */
		if (Menu == NULL)
			return (TRUE);
	}

	/* Make sure the menu structure is not NULL */
	if (Menu == NULL)
		return (FALSE);


	/* Check if the tree and menu have been used before for another
	 * menu item. Try to get the id for that one.
	 */
	Index = FindIndex(id, Menu->mn_tree, Menu->mn_menu);

	/* Now, attach the new submenu to the menu item in question */
	if (!Index)							/* Need to get a new menu ID! */
		Index = Get_New_Index(id, Menu->mn_tree, Menu->mn_menu);

	if (Index)
	{
		ObString(item)[(int)strlen(ObString(item)) - ARROW_OFFSET] = RIGHT_ARROW;
		ObType(item) |= (Index << 8);
		MakeSubMenu(item);

		if ((ProcPtr = FindProcess(id)) > (PNODE_PTR)NULL)
		{
			if ((IndexPtr = GetIndexPtr(ProcPtr, (int16_t) Index)) > (INDEX_PTR)NULL)
			{
				INDEX_COUNT(IndexPtr) += 1;	/* Up the count */
				INDEX_FLAGSCROLL(IndexPtr) = Menu->mn_scroll;

				ActiveTree(Menu->mn_tree);
				Menu->mn_item = min(Menu->mn_item, ObTail(Menu->mn_menu));
				Menu->mn_item = max(Menu->mn_item, ObHead(Menu->mn_menu));
				INDEX_OBJ(IndexPtr) = Menu->mn_item;
				return (TRUE);			/* in the node.. */
			}
		}
	}
	return (FALSE);
}



/* mn_getmn()
 * ================================================================
 * Checks to see if a submenu is attached to a specific menu item.
 *
 * IN: int16_t   id    - process id
 *     OBJECT *tree - the tree to see if something is attached to it.
 *     int16_t   item - the menu item that might have something attached
 *                    to it...
 *
 * OUT:    FALSE if there is an error
 *         TRUE - returns the tree, parent, item and scroll status in Menu.
 */
BOOLEAN mn_getmn(P(int16_t) id, P(OBJECT *) tree, P(int16_t) item, P(MENU *) Menu)
PP(int16_t id;)								/* Process id             */
PP(OBJECT *tree;)							/* the tree we want to check  */
PP(int16_t item;)								/* the menu item  for above   */
PP(MENU *Menu;)
{
	PNODE_PTR ProcPtr;					/* ptr to the process node  */
	uint16_t type;							/* the extended object type */
	int16_t Index;							/* the Index ID ( temp )    */
	register INDEX_PTR IndexPtr;				/* ptr to the index node... */
	BOOLEAN flag;						/* SUCCESS or FAILUE        */

	UNUSED(type);
	
	flag = FALSE;
	if (IsG_String(item) && IsSubMenu(item))
	{
		if (ObString(item)[(int)strlen(ObString(item)) - ARROW_OFFSET] == RIGHT_ARROW)
		{
			Index = (int16_t) ((ObType(item) & 0xFF00) >> 8);
			if ((ProcPtr = FindProcess(id)) > (PNODE_PTR)NULL)
			{
				if ((IndexPtr = GetIndexPtr(ProcPtr, Index)) > (INDEX_PTR)NULL)
				{
					Menu->mn_tree = INDEX_TREE(IndexPtr);
					Menu->mn_menu = INDEX_MENU(IndexPtr);
					Menu->mn_item = INDEX_OBJ(IndexPtr);
					Menu->mn_scroll = INDEX_FLAGSCROLL(IndexPtr);
					flag = TRUE;
				}
			}
		}
	}
	return (flag);
}




/* DetachSubMenu()
 * ================================================================
 * Detach the submenu attached to this menu item and free up the
 * memory used for the node.
 *
 * IN: int16_t   id    - Process id
 *     OBJECT *tree - the tree of the menu
 *     int16_t   item  - the menu item that has the subtree attached.
 *
 * OUT: void
 */
VOID DetachSubMenu(P(int16_t) id, P(OBJECT *) tree, P(int16_t) item)
PP(int16_t id;)								/* Process id            */
PP(OBJECT *tree;)							/* the tree fo the menu...   */
PP(int16_t item;)								/* see above...          */
{
	uint16_t type;							/* the Extended Object Types */
	int16_t Index;							/* Index ID!             */
	PNODE_PTR ProcPtr;					/* ptr to the process node   */

	UNUSED(type);
	if (IsG_String(item) && IsSubMenu(item))
	{
		/* Clear the character where the arrow is located. */
		ObString(item)[(int)strlen(ObString(item)) - ARROW_OFFSET] = ' ';

		/* Get the index id node */
		Index = (int16_t) ((ObType(item) & 0xFF00) >> 8);

		ObType(item) &= 0x00FF;			/* Clear out the extended object type */

		NoSubMenu(item);				/* Clear out SUBMENU FLAG */

		if ((ProcPtr = FindProcess(id)) > (PNODE_PTR)NULL)
			DeleteIndex(ProcPtr, Index, FALSE);	/* Clear out the index node. */
	}
}




/* CheckForSubMenu()
 * ================================================================
 * Checks the Menu item for a RIGHT_ARROW ( ARROW OFFSET )
 * characters in from the end and that the menu_id returned is valid.
 *
 * IN: int16_t     id      - Process id
 *     MENU_PTR MenuPtr - ptr to the Menu Node structure
 *     int16_t     obj     - the menu item
 *     MENU_PTR SubMenuPtr -
 *
 * OUT:     NULL - for not a submenu or invalid submenu
 *          MENU_PTR - A valid MenuPtr.
 */
BOOLEAN CheckForSubMenu(P(int16_t) id, P(OBJECT *) tree, P(int16_t) obj, P(MENU_PTR) SubMenuPtr)
PP(int16_t id;)								/* Process id           */
PP(OBJECT *tree;)
PP(int16_t obj;)								/* the object to check for      */
PP(MENU_PTR SubMenuPtr;)					/* ptr to the submenu - if valid */
{
	register uint16_t MenuIndex;				/* extended object type */
	register INDEX_PTR NewMenuPtr;			/* ptr to Index Node   */
	PNODE_PTR ProcPtr;					/* ptr to process node */


	if (IsG_String(obj) && IsEnabled(obj) && IsSubMenu(obj))
	{
		if (ObString(obj)[(int)strlen(ObString(obj)) - ARROW_OFFSET] == RIGHT_ARROW)
		{
			if ((MenuIndex = (ObType(obj) >> 8)) > 0)
			{
				/* if SubmenuPtr is non-zero, then the menu is already displayed */
				if (!SubMenuPtr)		/* cjg 01/13/92 */
				{
					/* Check for a valid ptr to an Index Structure. */
					if ((ProcPtr = FindProcess(id)) > (PNODE_PTR)NULL)
					{
						if ((NewMenuPtr = GetIndexPtr(ProcPtr, (int16_t) MenuIndex)) > (INDEX_PTR)NULL)
						{
							/* Make sure the number of displayed submenus does not
							 * exceed the MAX_LEVEL Limit
							 */
							if (CountLevel())
								return (TRUE);
						}
					}
				}
			}
		}
	}

	return (FALSE);
}



/* DoSubMenu()
 * ================================================================
 * Displays a submenu if valid ( Called by EvntSubMenu )
 * RETURNS: NULL if not valid - or if the blit fails.
 *          MenuPTR if valid or SUCCESS
 *
 * IN: int16_t     id      - Process id
 *     MENU_PTR MenuPtr - ptr to the Menu Node
 *     int16_t  obj         - menu item
 *
 * OUT: return the ptr to the submenu displayed.
 *      NULL if a FAILURE.
 */
MENU_PTR DoSubMenu(P(int16_t) id, P(OBJECT *) tree, P(int16_t) obj)
PP(int16_t id;)								/* Process id             */
PP(OBJECT *tree;)
PP(int16_t obj;)								/* menu item we pop with      */
{
	MENU_PTR SubPopPtr;					/* The MenuPtr of the Submenu */
	uint16_t type;							/* The type of the menu item  */
	int16_t MenuIndex;						/* The Submenu Menu ID        */
	GRECT rect;							/* Rect of the Menu item      */
	register int16_t xpos;						/* xpos, ypos of new submenu  */
	int16_t ypos;

	UNUSED(type);
	
	SubPopPtr = (MENU_PTR) NULL;

	if ((obj != NIL) && IsG_String(obj) && IsSubMenu(obj))
	{
		ob_actxywh((LPTREE)tree, obj, &rect);

		/* We won't check for a valid menuid here, since the
		 * CheckForSubMenu() routines already did that.
		 */
		MenuIndex = (int16_t) (ObType(obj) >> 8);

		/* Calculate the position of where to display the submenu. */
		xpos = rect.g_x + rect.g_w - 1 - gl_wchar;
		xpos = ((xpos + 7) / 8) * 8;
		ypos = rect.g_y;

		SubPopPtr = ShowSubMenu(id, MenuIndex, xpos, ypos, &rect);
	}
	return (SubPopPtr);
}



/* ShowSubMenu()
 * ================================================================
 * Shows the Actual Submenu, called by DoSubMenu()
 * Returns a valid MENU_PTR if SUCCESS, NULL if FAILURE.
 * RECT is supplied so that if we can't pop to the right,
 * we can pop it to the left.
 * ADJUSTPOSITION will have to be modified to accomodate that.
 *
 * IN:   int16_t  id;	    - Process id
 *       int16_t  MenuIndex    - The Index ID!
 *       int16_t  xpos,ypos    - the xpos and ypos
 *       GRECT *rect      - rect of the button used to display the menu.
 *
 * OUT:  MENU_PTR - returns a pointer for SUCCESS
 *                - NULL for FAILURE
 */
MENU_PTR ShowSubMenu(P(int16_t) id, P(int16_t) MenuIndex, P(int16_t) xpos, P(int16_t) ypos, P(GRECT *) rect)
PP(int16_t id;)								/* Process id            */
PP(int16_t MenuIndex;)							/* the Index # we look for   */
PP(int16_t xpos;)								/* xpos to pop the menu up to */
PP(int16_t ypos;)								/* ypos to pop the menu up to */
PP(GRECT *rect;)							/* rect of the button        */
{
	register OBJECT *tree;					/* temp tree          */
	register INDEX_PTR IndexPtr;				/* ptr to the Index Node  */
	register MENU_PTR MenuPtr;				/* ptr to the menu Node   */
	OBJECT *newtree;					/* temp tree          */
	int16_t NewMenuID;						/* the new menu id    */
	PNODE_PTR ProcPtr;					/* ptr to the process node */

	if ((ProcPtr = FindProcess(id)) == NULL)	/* Find the process */
		return ((MENU_PTR) NULL);

	if ((IndexPtr = GetIndexPtr(ProcPtr, MenuIndex)) == NULL)	/* Get the index ptr */
		return ((MENU_PTR) NULL);

	newtree = INDEX_TREE(IndexPtr);		/* get the new tree */
	/* and insert it!   */
	if ((NewMenuID = Menu_Insert(newtree, INDEX_MENU(IndexPtr))) == 0)
		return ((MENU_PTR) NULL);

	if ((MenuPtr = GetMenuPtr(NewMenuID)) > (MENU_PTR)NULL)	/* get Menu Ptr     */
	{									/* set start obj    */

		MSTART_OBJ(MenuPtr) = INDEX_OBJ(IndexPtr);
		ActiveTree(MTREE(MenuPtr));
		ObX(MPARENT(MenuPtr)) = xpos;	/* and xpos and */
		ObY(MPARENT(MenuPtr)) = ypos;	/* the ypos.   */
		AssignMenuData(MenuPtr, MSTART_OBJ(MenuPtr));	/* assign data */

		/* Make sure we stay within the limits */
		if (INDEX_FLAGSCROLL(IndexPtr))
		{
			INDEX_FLAGSCROLL(IndexPtr) = max(INDEX_FLAGSCROLL(IndexPtr), MFIRST_CHILD(MenuPtr));

			if (INDEX_FLAGSCROLL(IndexPtr) >= (MLAST_CHILD(MenuPtr) - 1))
				INDEX_FLAGSCROLL(IndexPtr) = MFIRST_CHILD(MenuPtr);
		}

		MSCROLL(MenuPtr) = INDEX_FLAGSCROLL(IndexPtr);
		if (INDEX_FLAGSCROLL(IndexPtr))
			CheckMenuHeight(MenuPtr);	/* check height */

		AdjustMenuPosition(MenuPtr, xpos, ypos, rect, TRUE, TRUE);

		/* Pop_Blit relies that the ROOT of the tree is already fixed up
		 * and in its proper position on the screen.
		 */
		if (!Pop_Blit(MenuPtr, FALSE))	/* if the blit fails, just */
		{								/* return and call it a day */
			RestoreMenu(MenuPtr);
			Menu_Delete(NewMenuID);
			return ((MENU_PTR) NULL);
		}
		ObjcDraw(tree, MPARENT(MenuPtr), &gl_rfull);
	}
	return (MenuPtr);
}



/* HideSubMenu()
 * ================================================================
 * Hides the submenu - The buffer is cleared by Pop_Blit.
 *
 * IN:  MENU_PTR MenuPtr - the ptr to the menu node in question.
 *
 * OUT: VOID
 */
VOID HideSubMenu(P(MENU_PTR) MenuPtr)
PP(register MENU_PTR MenuPtr;)					/* ptr to the menu node */
{
	if (MenuPtr)
	{
		Pop_Blit(MenuPtr, TRUE);		/* restore background */
		RestoreMenu(MenuPtr);			/* fixup menu        */
		Menu_Delete(MMENU_ID(MenuPtr));	/* delete node       */
	}
}



/* ================================================================
 * MENU DISPLAY AND DRAG ROUTINES
 * ================================================================
 */

/* Init_Delays()
 * ================================================================
 * Initializes the SubMenu Display Delay and the SubMenu Drag Delay
 * This routine is called in the beginning as an init routine.
 * It should be inside an INIT_SYSTEM call...
 */
VOID Init_Delays(NOTHING)
{
	int32_t dummy;

	SetDragDelay(INIT_DRAG_DELAY, &dummy);
	SetDisplayDelay(INIT_DISPLAY_DELAY, &dummy);
	SetScrollDelay(INIT_SCROLL_DELAY, &dummy);
	SetArrowDelay(INIT_ARROW_DELAY, &dummy);
}


/* SetDisplayDelay()
 * ================================================================
 * Sets the length of time before a submenu appears as the user
 * drags the mouse thru a menu and on top of a menu item with a
 * submenu attached.
 *        1000ms   = 1 sec
 *	  5ms      = 1 tick
 *        200ticks = 1 sec
 *
 * IN: Milliseconds ( int32_t )     1000ms == 1 sec.
 *     < 0L - will return the current value.
 * OUT: returns the current value set to.
 */
VOID SetDisplayDelay(P(int32_t) ms, P(int32_t *) oldvalue)
PP(int32_t ms;)
PP(int32_t *oldvalue;)
{
	if (ms >= 0L)
		SUBMENU_DELAY = ms;
	*oldvalue = SUBMENU_DELAY;
}


/* SetDragDelay()
 * ================================================================
 * Sets the length of time the user has to drag diagonally, the mouse
 * to an active submenu before the submenu closes.
 *
 *        1000ms   = 1 sec
 *	  5ms      = 1 tick
 *        200ticks = 1 sec
 *
 * IN: Milliseconds ( int32_t )     1000ms == 1 sec.
 *     < 0L - will return the current value.
 * OUT: returns the value that we set it to.
 */
VOID SetDragDelay(P(int32_t) ms, P(int32_t *) oldvalue)
PP(int32_t ms;)
PP(int32_t *oldvalue;)
{
	if (ms >= 0L)
		SUBDRAG_DELAY = ms;
	*oldvalue = SUBDRAG_DELAY;
}



/* SetScrollDelay()
 * ================================================================
 * Set the time delay the menu scrolls, per menu item,
 * once scrolling occurs.
 * IN: Milliseconds ( int32_t )     1000ms == 1 sec.
 *     < 0L - will return the current value.
 * OUT: returns the value that we set it to.
 */
VOID SetScrollDelay(P(int32_t) ms, P(int32_t *) oldvalue)
PP(int32_t ms;)
PP(int32_t *oldvalue;)
{
	if (ms >= 0L)
		SCROLL_DELAY = ms;
	*oldvalue = SCROLL_DELAY;
}


/* SetArrowDelay()
 * ================================================================
 * Set the time delay when the menu starts to scroll
 * IN: Milliseconds ( int32_t )     1000ms == 1 sec.
 *     < 0L - will return the current value.
 * OUT: returns the value that we set it to.
 */
VOID SetArrowDelay(P(int32_t) ms, P(int32_t *) oldvalue)
PP(int32_t ms;)
PP(int32_t *oldvalue;)
{
	if (ms >= 0L)
		ARROW_DELAY = ms;
	*oldvalue = ARROW_DELAY;
}


/*
 * AES #39 - menu_settings - Set or inquire popup/submenu parameters. 
 *
 * menu_settings()
 * ================================================================
 * flag == 0    GET SETTINGS
 * flag == 1    SET SETTINGS
 *		if any setting is < 0, ignore setting
 */
VOID mn_settings(P(int16_t) flag, P(MN_SET *) Values)
PP(int16_t flag;)
PP(MN_SET *Values;)
{
	int32_t dummy;

	switch (flag)
	{
	case 0:							/* GET SETTINGS */
		SetDisplayDelay(-1L, &Values->Display);
		SetDragDelay(-1L, &Values->Drag);
		SetArrowDelay(-1L, &Values->Delay);
		SetScrollDelay(-1L, &Values->Speed);
		Values->Height = SetMaxHeight(-1);
		break;

	case 1:							/* SET SETTINGS */
		if (Values->Display >= 0L)
			SetDisplayDelay(Values->Display, &dummy);

		if (Values->Drag >= 0L)
			SetDragDelay(Values->Drag, &dummy);

		if (Values->Delay >= 0L)
			SetArrowDelay(Values->Delay, &dummy);

		if (Values->Speed >= 0L)
			SetScrollDelay(Values->Speed, &dummy);

		if (Values->Height >= 0)
			SetMaxHeight(Values->Height);
		break;

	default:
		break;
	}
}



/* CountLevel()
 * ================================================================
 * Count the Number of submenus displayed, including this one.
 * Return TRUE if the count is less than or equal to the limit. MAX_LEVEL
 */
BOOLEAN CountLevel(NOTHING)
{
	int count;
	MENU_PTR curptr;

	count = 0;
	curptr = MenuList;

	while (curptr)
	{
		count++;
		curptr = MNEXT(curptr);
	}
	return (count < MAX_LEVEL);
}

#endif /* SUBMENUS */
