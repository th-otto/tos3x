/* MN_TOOLS.H
 * ================================================================
 * DESCRIPTION: structures and defines for menu.c
 * 
 * 04/01/92 cjg - added new structures ( MENU and MN_SET )
 *		- added scroll_flag field to the MENU_NODE structure
 */


/* STRUCTURES
 * ================================================================
 */


/* Structure for passing menu data */
typedef struct _menu
{
   OBJECT *mn_tree;		/* Object tree of the menu */
   int16_t   mn_menu;		/* Parent of the menu items*/
   int16_t   mn_item;		/* Starting menu item      */
   int16_t   mn_scroll;		/* scroll flag for the menu*/
   int16_t   mn_keystate;		/* Key State 		   */
} MENU;


/* Structure for the Menu Settings */
typedef struct _mn_set
{
   int32_t   Display;		/* The display delay      */
   int32_t   Drag;			/* The drag delay         */
   int32_t   Delay;		/* The Arrow Delay        */
   int32_t   Speed;		/* The scroll speed delay */
   int16_t   Height;		/* The menu scroll height */
} MN_SET;



typedef struct _mrets
{
  int16_t x;
  int16_t y;
  int16_t buttons;
  int16_t kstate;
} MRETS;

/*
 * Object bitfield structures
 */
typedef struct sColorword
{
/*
#if HIBIT_1ST
	unsigned cborder : 4;
	unsigned ctext	 : 4;
	unsigned replace : 1;
	unsigned pattern : 3;
	unsigned cfill	 : 4;
#else
*/
	unsigned cfill	 : 4;
	unsigned pattern : 3;
	unsigned replace : 1;
	unsigned ctext	 : 4;
	unsigned cborder : 4;
/*#endif*/
} Colorword;


typedef struct sObInfo
{
	char		letter;
	char border;	/* signed */
	Colorword	c;
} ObInfo;




/* SubMenu Index Structure -
 * This is a structure used for attaching submenus to menu items.
 * The index number is stored in the extended object type of the object.
 * There can be a maximum of 255 ( 0 is invalid ) submenus attached to
 * a menu item for each process.
 */
typedef struct _index_info
{
    BOOLEAN  status;	      /* 0 - InActive, 1 - Active   */
    int16_t     index;	      /* Index ID for this node     */
    OBJECT  *subtree;	      /* The OBJECT tree            */
    int16_t     menu;	      /* The menu object ( parent ) */
    int16_t     start_obj;	      /* The starting menu item     */
    BOOLEAN  scroll_flag;     /* TRUE - scroll if >18 items */
    int16_t     count;	      /* The # of times this menu is attached. */
} INDEX_NODE, *INDEX_PTR;

#define INDEX_STATUS( ptr )    ptr->status
#define INDEX_ID( ptr )        ptr->index
#define INDEX_TREE( ptr )      ptr->subtree
#define INDEX_MENU( ptr )      ptr->menu
#define INDEX_OBJ( ptr )       ptr->start_obj
#define INDEX_FLAGSCROLL(ptr)  ptr->scroll_flag
#define INDEX_COUNT( ptr )     ptr->count


#define CMAX 		8
#define MIN_INDEX	128
#define MAX_INDEX	192


/* INDEX cluster structure
 * Index nodes are grouped into clusters of 8 nodes.
 */
typedef struct _cnode
{
    INDEX_NODE    ctable[ CMAX ];  /* First Cluster       */
    struct _cnode *cnext;          /* ptr to next cluster */
} CNODE, *CNODE_PTR;

#define CTABLE( ptr )   ptr->ctable
#define CNEXT( ptr )    ptr->cnext


/* Index Process Structure */
typedef struct _proc_node
{
    int16_t	       pid;
    int16_t	       num;		/* Number of nodes in use       */
    CNODE              cluster;		/* First CLUSTER_MAX group	*/
    struct _proc_node *pnext;		/* pointer to next process node */
} PNODE, *PNODE_PTR;

#define PID( ptr )	  ptr->pid
#define PCOUNT( ptr )	  ptr->num
#define PCLUSTER( ptr )   ptr->cluster
#define PNEXT( ptr )	  ptr->pnext



/* Structure for popup and submenu nodes.
 * These are allocated when the menu is prepared to be displayed and
 * free'd when they are removed.
 * There can be a maximum of 32768 submenus displayed at any one time.
 */
typedef struct _menu_node
{
	uint16_t   MenuID;	 	 /* Menu ID # 			  */
	OBJECT  *tree_ptr;	 /* Pointer to object tree data   */

        int16_t	StartItem;	 /* Start Menu Item = Default == 1*/
	int16_t    NumItems;	 /* Num Items in menu 		  */

	GRECT   rect;		 /* GRECT of Menu in pixels...    */
	int16_t    offset;		 /* Offset into the menu ( SCROLL)*/

	int16_t	Parent;		 /* Parent object in tree...	  */
	int16_t	FirstChild;	 /* First object in tree...       */
	int16_t    LastChild;	 /* Last object in tree...        */
	uint16_t   LastFlag;   	 /* ObFlag of Last Child	  */

	int16_t	TopObject;	 /* Top object in Scroll Menu...  */
	uint16_t   TopState;   	 /* Top item's object state.      */
	uint16_t	TopFlag;	 /* Top items object flag	  */
	char    TopText[ 128 ];	 /* Top item's ObString()	  */

	int16_t    BObject;    	 /* Bottom object in Scroll Menu  */
	uint16_t   BState;	 	 /* Bottom item's object state    */
	uint16_t   BFlag;		 /* Bottom item's object flag     */
	char    BText[128]; 	 /* Bottom item's ObString()      */

	int32_t    *buffer;	 /* Pointer to redraw buffer...   */
	struct _menu_node *mprev; /* Ptr to Previous SubMenu       */

	struct _menu_node *mnext; /* Ptr to Next Linked Menu_Node  */

	int16_t    mscroll;	  /* scroll flag */
} MENU_NODE, *MENU_PTR;



/* DEFINES
 * ================================================================
 */
#define MMENU_ID( ptr )      ptr->MenuID
#define MTREE( ptr )         ptr->tree_ptr

#define MSTART_OBJ( ptr )    ptr->StartItem
#define MNUM_ITEMS( ptr )    ptr->NumItems

#define MXPOS( ptr )	     ptr->rect.g_x
#define MYPOS( ptr )	     ptr->rect.g_y
#define MWIDTH( ptr )        ptr->rect.g_w
#define MHEIGHT( ptr )       ptr->rect.g_h
#define MOBRECT( ptr )       ( *( GRECT *)&ptr->rect.g_x )

#define MOFFSET( ptr )	     ptr->offset

#define MPARENT( ptr )	     ptr->Parent
#define MFIRST_CHILD( ptr )  ptr->FirstChild
#define MLAST_CHILD( ptr )   ptr->LastChild
#define MLASTFLAG( ptr )     ptr->LastFlag

#define MTOP_OBJ( ptr )	     ptr->TopObject
#define MTOP_STATE( ptr )    ptr->TopState
#define MTOP_FLAG( ptr )     ptr->TopFlag
#define MTOP_TXT( ptr )	     ptr->TopText

#define MB_OBJ( ptr )        ptr->BObject
#define MB_STATE( ptr )      ptr->BState
#define MB_FLAG( ptr )	     ptr->BFlag
#define MB_TXT( ptr )        ptr->BText

#define MBUFFER( ptr )	     ptr->buffer
#define MPREV( ptr )	     ptr->mprev
#define MNEXT( ptr )	     ptr->mnext


#define MSCROLL( ptr )	     ptr->mscroll


/*
 * Object structure access macros ( `tree' must be declared OBJECT * )
 * ----------------------------------------------------------------------
 */
#define ObNext(obj) 	( tree[(obj)].ob_next )
#define ObHead(obj) 	( tree[(obj)].ob_head )
#define ObTail(obj) 	( tree[(obj)].ob_tail )
#define ObType(obj) 	( tree[(obj)].ob_type )
#define ObFlags(obj)	( tree[(obj)].ob_flags )
#define ObState(obj)	( tree[(obj)].ob_state )
#define ObSpec(obj) 	( tree[(obj)].ob_spec )
#define ObIndex(obj)    ( tree[(obj)].ob_spec )

/* ObSpecs for ObBOX, ObIBOX, ObBOXCHAR */
#define ObChar(obj) 	( (*(ObInfo *)&ObSpec(obj)).letter )
#define ObBorder(obj)	( (*(ObInfo *)&ObSpec(obj)).border )
#define ObCBorder(obj)	( (*(ObInfo *)&ObSpec(obj)).c.cborder )
#define ObCText(obj)	( (*(ObInfo *)&ObSpec(obj)).c.ctext )
#define ObReplace(obj)	( (*(ObInfo *)&ObSpec(obj)).c.replace )
#define ObPattern(obj)	( (*(ObInfo *)&ObSpec(obj)).c.pattern )
#define ObCFill(obj)	( (*(ObInfo *)&ObSpec(obj)).c.cfill )

/* TEDINFO access macros, for ObTEXT, ObBOXTEXT, ObFTEXT, ObFBOXTEXT */
#define _TE(obj)	( (TEDINFO *)ObSpec(obj) )
#define TedText(obj)	( _TE(obj)->te_ptext )
#define TedTemplate(obj)( _TE(obj)->te_ptmplt )
#define TedValid(obj)	( _TE(obj)->te_pvalid )
#define TedFont(obj)	( _TE(obj)->te_font )
#define TedJust(obj)	( _TE(obj)->te_just )

#define TedCBorder(obj) ( (*(Colorword *)&_TE(obj)->te_color).cborder )
#define TedCText(obj)	( (*(Colorword *)&_TE(obj)->te_color).ctext )
#define TedReplace(obj) ( (*(Colorword *)&_TE(obj)->te_color).replace )
#define TedPattern(obj) ( (*(Colorword *)&_TE(obj)->te_color).pattern )
#define TedCFill(obj)	( (*(Colorword *)&_TE(obj)->te_color).cfill )

#define TedBorder(obj)	( _TE(obj)->te_thickness )
#define TedLen(obj) 	( _TE(obj)->te_txtlen )
#define TedTempLen(obj) ( _TE(obj)->te_tmplen )

/* ICONBLK access for ObICON */
#define _IB(obj)	( (ICONBLK *)ObSpec(obj) )
#define IconMask(obj)	( _IB(obj)->ib_pmask )
#define IconData(obj)	( _IB(obj)->ib_pdata )
#define IconText(obj)	( _IB(obj)->ib_ptext )
#define IconFColor(obj) ( (*(IconInfo *)&_IB(obj)->ib_char).fcolor )
#define IconBColor(obj) ( (*(IconInfo *)&_IB(obj)->ib_char).bcolor )
#define IconChar(obj)	( (*(IconInfo *)&_IB(obj)->ib_char).letter )
#define IconRect(obj)	( *(GRECT *)&_IB(obj)->ib_xicon )
#define IconTRect(obj)	( *(GRECT *)&_IB(obj)->ib_xtext )

/* BITBLK access for ObIMAGE */
#define _BB(obj)	( (BITBLK *)ObSpec(obj) )
#define ImageData(obj)	( _BB(obj)->bi_pdata )
#define ImageWb(obj)	( _BB(obj)->bi_wb )
#define ImageH(obj) 	( _BB(obj)->bi_hl )
#define ImageColor(obj) ( _BB(obj)->bi_color )

/* String access for ObBUTTON, ObSTRING, ObTITLE */
#define ObString(obj)	( (char *)ObSpec(obj) )

/* Object extents */
#define ObX(obj)	( tree[(obj)].ob_x )
#define ObY(obj)	( tree[(obj)].ob_y )
#define ObW(obj)	( tree[(obj)].ob_width )
#define ObH(obj)	( tree[(obj)].ob_height )
#define ObRect(obj) 	( *(GRECT *)&tree[(obj)].ob_x )


/*
 * Object flags & states macros
 */
#define IsSelectable(obj)	( ObFlags(obj) & SELECTABLE )
#define IsDefault(obj)		( ObFlags(obj) & DEFAULT )
#define IsExit(obj) 		( ObFlags(obj) & EXIT )
#define IsEditable(obj) 	( ObFlags(obj) & EDITABLE )
#define IsRadio(obj)		( ObFlags(obj) & RBUTTON )
#define IsRButton(obj)		IsRadio(obj)
#define IsLast(obj) 		( ObFlags(obj) & LASTOB )
#define IsLastob(obj)		IsLast(obj)
#define IsTouchexit(obj)	( ObFlags(obj) & TOUCHEXIT )
#define IsHidden(obj)		( ObFlags(obj) & HIDETREE )
#define IsVisible(obj)		( !IsHidden(obj) )
#define IsIndirect(obj) 	( ObFlags(obj) & INDIRECT )
#define IsSubMenu(obj)		( ObFlags(obj) & SUBMENU )

#define IsSelected(obj) 	( ObState(obj) & SELECTED )
#define IsCrossed(obj)		( ObState(obj) & CROSSED )
#define IsChecked(obj)		( ObState(obj) & CHECKED )
#define IsDisabled(obj) 	( ObState(obj) & DISABLED )
#define IsEnabled(obj)		( !IsDisabled(obj) )
#define IsOutlined(obj) 	( ObState(obj) & OUTLINED )
#define IsShadowed(obj) 	( ObState(obj) & SHADOWED )
#define IsTed(obj)		( (ObType(obj) == G_TEXT)	|| \
				  (ObType(obj) == G_BOXTEXT)	|| \
				  (ObType(obj) == G_FTEXT)	|| \
				  (ObType(obj) == G_FBOXTEXT) )
#define IsG_String(obj)		( ( ObType(obj) & 0x00FF ) == G_STRING )

#define ActiveTree( newtree )	( tree = newtree )
#define IsActiveTree( newtree ) ( tree == newtree )

/*
 * Object flags & states manipulation macros.
 * NOTE: These do not affect the visual appearance of the object.
 */
#define MakeSelectable(obj) 	( ObFlags(obj) |= SELECTABLE )
#define MakeDefault(obj)	( ObFlags(obj) |= DEFAULT )
#define MakeExit(obj)		( ObFlags(obj) |= EXIT )
#define MakeEditable(obj)	( ObFlags(obj) |= EDITABLE )
#define MakeRadio(obj)		( ObFlags(obj) |= RBUTTON )
#define MakeRButton(obj)	MakeRadio(obj)
#define MakeLast(obj)		( ObFlags(obj) |= LASTOB )
#define MakeLastob(obj) 	MakeLast(obj)
#define MakeTouchexit(obj)	( ObFlags(obj) |= TOUCHEXIT )
#define HideObj(obj)		( ObFlags(obj) |= HIDETREE )
#define MakeHidden(obj) 	HideObj(obj)
#define MakeHidetree(obj)	HideObj(obj)
#define MakeIndirect(obj)	( ObFlags(obj) |= INDIRECT )
#define MakeSubMenu(obj)	( ObFlags(obj) |= SUBMENU )

#define NoSelect(obj)		( ObFlags(obj) &= ~SELECTABLE )
#define NoDefault(obj)		( ObFlags(obj) &= ~DEFAULT )
#define NoExit(obj) 		( ObFlags(obj) &= ~EXIT )
#define NoEdit(obj) 		( ObFlags(obj) &= ~EDITABLE )
#define NoRadio(obj)		( ObFlags(obj) &= ~RBUTTON )
#define NoRButton(obj)		NoRadio(obj)
#define NoLast(obj) 		( ObFlags(obj) &= ~LASTOB )
#define NoLastob(obj)		NoLast(obj)
#define NoTouchexit(obj)	( ObFlags(obj) &= ~TOUCHEXIT )
#define ShowObj(obj)		( ObFlags(obj) &= ~HIDETREE )
#define NoHidetree(obj) 	ShowObj(obj)
#define NoIndirect(obj) 	( ObFlags(obj) &= ~INDIRECT )
#define NoSubMenu(obj)		( ObFlags(obj) &= ~SUBMENU )

#define SelectObj(obj)		( ObState(obj) |= SELECTED )
#define CrossObj(obj)		( ObState(obj) |= CROSSED )
#define CheckObj(obj)		( ObState(obj) |= CHECKED )
#define DisableObj(obj) 	( ObState(obj) |= DISABLED )
#define OutlineObj(obj) 	( ObState(obj) |= OUTLINED )
#define ShadowObj(obj)		( ObState(obj) |= SHADOWED )
#define Select(obj)		( ObState(obj) |= SELECTED )
#define Disable(obj)		( ObState(obj) |= DISABLED )

#define DeselectObj(obj)	( ObState(obj) &= ~SELECTED )
#define UnCrossObj(obj) 	( ObState(obj) &= ~CROSSED )
#define UnCheckObj(obj) 	( ObState(obj) &= ~CHECKED )
#define EnableObj(obj)		( ObState(obj) &= ~DISABLED )
#define NoOutlineObj(obj)	( ObState(obj) &= ~OUTLINED )
#define NoShadowObj(obj)	( ObState(obj) &= ~SHADOWED )
#define Deselect(obj)		( ObState(obj) &= ~SELECTED )
#define Enable(obj)		( ObState(obj) &= ~DISABLED )
#define SetNormal(obj)		( ObState(obj) = NORMAL	   )

#define NO_ARROW	(-1)
#define UP_ARROW	0
#define DOWN_ARROW	1
#define TIME_SAMPLE		150L

#define INIT_MAX_HEIGHT 16
#define MIN_HEIGHT	5

#define INIT_ARROW_DELAY	250L
#define INIT_SCROLL_DELAY	0L

#define MENU_MAX	32767
#define MAX_LEVEL	4


#define RIGHT_ARROW		0x03
#define ARROW_OFFSET		2
#define INIT_DRAG_DELAY		10000L
#define INIT_DISPLAY_DELAY	200L


/* these are defined or should have been defined in gemlib etc */
#define BEG_MCTRL	3
#define END_MCTRL	2



/*
 * mn_event.c
 */
extern char const UpText[];
extern char const DownText[];

OBJECT *CurTree;
int16_t CurMenu;
int16_t CurObject;
int16_t CurScroll;
int16_t CurKeyState;

uint16_t xdo_chg PROTO((OBJECT *tree, int16_t iitem, uint16_t chgvalue, int16_t dochg, int16_t dodraw, int16_t usetrap));
int16_t mu_set PROTO((OBJECT *tree, int16_t last_item, int16_t cur_item, int16_t setit, int16_t usetrap));
int32_t EvntSubMenu PROTO((int16_t id, MENU_PTR MenuPtr));
BOOLEAN Pop_Blit PROTO((MENU_PTR MenuPtr, int16_t flag));


/*
 * mn_index.c
 */
extern PNODE_PTR IndexList;

int16_t FindIndex PROTO((int16_t id, OBJECT *itree, int16_t imenu));
int16_t Get_New_Index PROTO((int16_t id, OBJECT *itree, int16_t imenu));
PNODE_PTR FindProcess PROTO((int16_t id));
INDEX_PTR GetIndexPtr PROTO((PNODE_PTR ProcPtr, int16_t index));
VOID DeleteIndex PROTO((PNODE_PTR ProcPtr, int16_t index, BOOLEAN flag));

/*
 * mn_mbar.c
 */
extern GRECT ActiveRect;
extern GRECT TitleRect;
extern BOOLEAN MenuBar_Mode;
extern OBJECT *gl_mtree;
extern MENU_PTR gl_menuptr;					/* MENU_PTR for the drop-down menu  */
extern int16_t buparm; /* WTF */



/*
 * mn_menu.c
 */
extern MENU_PTR MenuList;
extern int16_t MAX_MENU_HEIGHT;

int16_t Menu_Insert PROTO((OBJECT *tree, int16_t Parent));
VOID Menu_Delete PROTO((int16_t MenuID));
MENU_PTR GetMenuPtr PROTO((int16_t MenuID));
VOID CheckMenuHeight PROTO((MENU_PTR MenuPtr));
int16_t CountMenuItems PROTO((MENU_PTR MenuPtr));
VOID RestoreMenu PROTO((MENU_PTR MenuPtr));
VOID AdjustMenuPosition PROTO((MENU_PTR MenuPtr, int16_t xpos, int16_t ypos, GRECT *rect, BOOLEAN Horizontal_Flag, BOOLEAN Vertical_Flag));
VOID MenuScrollAdjust PROTO((MENU_PTR MenuPtr, int16_t start_obj));
int16_t SetMaxHeight PROTO((int16_t height));


/*
 * mn_popup.c
 */
BOOLEAN mn_popup PROTO((int16_t id, MENU *Menu, int16_t xpos, int16_t ypos, MENU *MData));
VOID AssignMenuData PROTO((MENU_PTR MenuPtr, int16_t start_obj));


/*
 * mn_submenu.c
 */
extern int32_t SUBMENU_DELAY;			/* Delay time for submenus to appear. ( ms ) */
extern int32_t SUBDRAG_DELAY;			/* Delay time for submenus to go active( ms )
										 * as the user drags the mouse to the menu.
										 */
extern int32_t SCROLL_DELAY;			/* Delay time to scroll menu items ( ms ) */
extern int32_t ARROW_DELAY;				/* Delay time to start scroll ( ms )      */

int16_t mn_istart PROTO((int16_t id, int16_t flag, OBJECT *tree, int16_t menu, int16_t item));
BOOLEAN mn_attach PROTO((int16_t id, int16_t flag, OBJECT *tree, int16_t item, MENU *Menu));
BOOLEAN CheckForSubMenu PROTO((int16_t id, OBJECT *tree, int16_t obj, MENU_PTR SubMenuPtr));
MENU_PTR DoSubMenu PROTO((int16_t id, OBJECT *tree, int16_t obj));
VOID HideSubMenu PROTO((MENU_PTR MenuPtr));
VOID Init_Delays PROTO((NOTHING));
VOID mn_settings PROTO((int16_t flag, MN_SET *Values));


/*
 * mn_tools.c
 */
VOID ObjcDraw PROTO((OBJECT *tree, int obj, GRECT *rect));
VOID rc_2xy PROTO((GRECT *r, int16_t *pxy));
