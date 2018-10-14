/*	aesalt.h - alternate AES bindings
 * Copyright 1990, Atari Corporation
 * ================================================================
 * 900129 kbad	Incorporated cgee updates, added enums for type
 *				checking, created WARGS union, added event macros
 * 890716 kbad	Modified object structure macros so as not to
 *				conflict with AES <taddr.h>
 * 890716 kbad	finito...
 */

#ifndef __AESALT__
#define __AESALT__
#ifndef __AES__

/*
 * Object bitfield structures
 */
typedef struct sColorword
{
#if 0
	unsigned cfill	 : 4;
	unsigned pattern : 3;
	unsigned replace : 1;
	unsigned ctext	 : 4;
	unsigned cborder : 4;
#else
	unsigned cborder : 4;
	unsigned ctext	 : 4;
	unsigned replace : 1;
	unsigned pattern : 3;
	unsigned cfill	 : 4;
#endif
} Colorword;

#ifdef LATTICE
typedef struct sObInfo
{
	unsigned char letter;
	signed char border;
	Colorword c;
} ObInfo;
#endif

#include <aes.h>
#endif

#ifndef PORTAB_H
#include <stddef.h>
#include "aportab.h"
#endif


#ifdef LATTICE_56
typedef struct moblk
{
	short m_out;
	short m_x;
	short m_y;
	short m_w;
	short m_h;
} MOBLK;
#endif

/* Definitions
 * ================================================================
 */

#define A_OK	TRUE
#define A_ERROR FALSE
#define A_CANCEL FALSE

/* nil object */
#undef NIL
#define NIL		(-1)

/* Window info structure, demonstrated in textwind.c
 */
#ifdef WINFO
#error WINFO defined somewhere other than aesalt.h
#endif
typedef struct _winfo{
		struct _winfo *next;
		struct _winfo *prev;
		WORD	id;				/* window handle */
		GRECT	r;				/* window position */
		GRECT	work;			/* working rectangle */
		GRECT	save;			/* saved window position */
		BOOLEAN fulled;			/* window is fulled */
		WORD	kind;			/* wind_create flags */
		char	name[80];		/* window name */
		char	info[80];		/* window info */
		GRECT	phy;			/* physical coordinates */
		WORD 	wchar, hchar;	/* dimensions of a "character" */
		GRECT	vir;			/* virtual coordinates */
		void *x;				/* extended info (app. specific) */
} WINFO;


/* MRETS struct for mouse parameters returned by
 * Evnt_button(), Evnt_mouse(), Evnt_multi(), Graf_mkstate()
 */
#ifdef MRETS
#error MRETS defined somewhere other than aesalt.h
#endif
#if defined(__TURBOC__) && !defined(__PUREC__)
typedef struct {
	int	x;
	int	y;
	int	buttons;
	int	kstate;
} MRETS;
#else
typedef struct {
	WORD	x;
	WORD	y;
	WORD	buttons;
	WORD	kstate;
} MRETS;
#endif

/* flags for objc_edit(), as enum for type checking */
#undef ED_START
#undef ED_INIT
#undef ED_CHAR
#undef ED_END
typedef enum {
	ED_START=0,
	ED_INIT,
	ED_CHAR,
	ED_END
} ED_FLAG;

/* flags for form_dial(), as enum for type checking */
#undef FMD_START
#undef FMD_GROW
#undef FMD_SHRINK
#undef FMD_FINISH
typedef enum {
	FMD_START=0,
	FMD_GROW,
	FMD_SHRINK,
	FMD_FINISH
} FMD_FLAG;

/* flags for wind_set/get, as enum for type checking */
#undef WF_KIND
#undef WF_NAME
#undef WF_INFO
#undef WF_WORKXYWH
#undef WF_CURRXYWH
#undef WF_PREVXYWH
#undef WF_FULLXYWH
#undef WF_HSLIDE
#undef WF_VSLIDE
#undef WF_TOP
#undef WF_FIRSTXYWH
#undef WF_NEXTXYWH
#undef WF_RESVD
#undef WF_NEWDESK
#undef WF_HSLSIZE
#undef WF_VSLSIZE
#undef WF_SCREEN
typedef enum {
	WF_KIND=1,
	WF_NAME,
	WF_INFO,
	WF_WORKXYWH,
	WF_CURRXYWH,
	WF_PREVXYWH,
	WF_FULLXYWH,
	WF_HSLIDE,
	WF_VSLIDE,
	WF_TOP,
	WF_FIRSTXYWH,
	WF_NEXTXYWH,
	WF_RESVD,
	WF_NEWDESK,
	WF_HSLSIZE,
	WF_VSLSIZE,
	WF_SCREEN
} WFLAG;

/* wind_get/set() args */
typedef union {
	char 	*name;		/* NAME */
	char 	*info;		/* INFO */
	GRECT	r;			/* WORK, CURR, PREV, FULL, FIRST, NEXT */
	WORD	position;	/* HSLIDE, VSLIDE */
	WORD	id;			/* TOP */
	struct {
		OBJECT	*tree;
		WORD	startob;
	} newdesk;			/* NEWDESK */
	WORD	size;		/* HSLSIZE, VSLSIZE */
	VOIDP	buf;		/* SCREEN */
} WARGS;

#define NOBOX 0, 0, 0, 0	/* some GEM routines need coordinates */
#define MAX_WINDOWS 7		/* DO NOT RELY ON THIS NUMBER!!!
							 * it will go away someday.
							 */

/* Event_multi() returns, require `WORD event' declaration
 * ================================================================
 */
#define EvKey()		(event & MU_KEYBD)
#define EvButton()	(event & MU_BUTTON)
#define EvM1()		(event & MU_M1)
#define EvM2()		(event & MU_M2)
#define EvMessage()	(event & MU_MESAG)
#define EvTimer()	(event & MU_TIMER)

/* Message buffer defines, require `WORD (msg)[8]' declaration
 * ================================================================
 */
#define MsgType(msg)	((msg)[0])
#define MsgSender(msg)	((msg)[1])
#define MsgExtra(msg)	((msg)[2])
#define MnTitle(msg)	((msg)[3])
#define MnItem(msg)		((msg)[4])
#define WmId(msg)		((msg)[3])
#define WmRect(msg)		(*(GRECT *)&((msg)[4]))
#define WmAction(msg)	((msg)[4])
#define WmSlider(msg)	((msg)[4])
#define AcPosition(msg)	((msg)[3])
#define AcOpenId(msg)	((msg)[4])
#define AcCloseId(msg)	((msg)[3])




/* Object structure macros, useful in dealing with forms
 * ================================================================
 * `tree' must be an OBJECT *
 */
#undef SPECIAL
#define SPECIAL		0x40 /* user defined object state */

#ifdef LATTICE_560
/*
 * this is ugly, but without it, Lattice 5.6
 * generates calls to _ulmul for every access
 */
#define _O(t, o) (*((OBJECT *)((char *)(t) + (o) * (short)sizeof(OBJECT))))
#else
#define _O(t, o) t[o]
#endif
#define _OT(o) _O(tree, o)

#define ObNext(obj)		( _OT(obj).ob_next )
#define ObHead(obj)		( _OT(obj).ob_head )
#define ObTail(obj)		( _OT(obj).ob_tail )
#define ObFlags(obj)	( _OT(obj).ob_flags )
#define ObState(obj)	( _OT(obj).ob_state )
#define ObSpec(obj)		( _OT(obj).ob_spec )
#define ObType(obj)		( _OT(obj).ob_type )

/* ObSpecs for ObBOX, ObIBOX, ObBOXCHAR */
#ifdef LATTICE
#define ObIndex(obj)    ( (*(long *)&ObSpec(obj)))
#define ObColor(obj)    ( (*(ObInfo *)&ObSpec(obj)).c )
#define ObChar(obj) 	( (*(ObInfo *)&ObSpec(obj)).letter )
#define ObBorder(obj)	( (*(ObInfo *)&ObSpec(obj)).border )
#define ObCBorder(obj)	( ObColor(obj).cborder )
#define ObCText(obj)	( ObColor(obj).ctext )
#define ObReplace(obj)	( ObColor(obj).replace )
#define ObPattern(obj)	( ObColor(obj).pattern )
#define ObCFill(obj)	( ObColor(obj).cfill )

#define TedThick(obj)   ( tree[(obj)].ob_spec.tedinfo->te_thickness )

#define _TE(obj)	( (TEDINFO *)ObSpec(obj) )
#define TedText(obj)	( _TE(obj)->te_ptext )
#define TedTemplate(obj)( _TE(obj)->te_ptmplt )
#define TedValid(obj)	( _TE(obj)->te_pvalid )
#define TedLen(obj) 	( _TE(obj)->te_txtlen )
#define TedTempLen(obj) ( _TE(obj)->te_tmplen )
#define TedBorder(obj)	( _TE(obj)->te_thickness )
#define TedFont(obj)	( _TE(obj)->te_font )
#define TedJust(obj)	( _TE(obj)->te_just )

#define TedColor(obj)   ((*(WORD *)&_TE(obj)->te_color))
#define TedCBorder(obj) ((*(Colorword *)&_TE(obj)->te_color).cborder )
#define TedCText(obj)	((*(Colorword *)&_TE(obj)->te_color).ctext )
#define TedReplace(obj) ((*(Colorword *)&_TE(obj)->te_color).replace )
#define TedPattern(obj) ((*(Colorword *)&_TE(obj)->te_color).pattern )
#define TedCFill(obj)	((*(Colorword *)&_TE(obj)->te_color).cfill )

#define ObString(obj)	( (char *)ObSpec(obj) )

#else

#define ObIndex(obj)	( tree[(obj)].ob_spec.index )
#define ObColor(obj)	( tree[(obj)].ob_spec.index )

#define TedText(obj)	( tree[(obj)].ob_spec.tedinfo->te_ptext )
#define TedTemplate(obj)	( tree[(obj)].ob_spec.tedinfo->te_ptmplt )
#define TedValid(obj)	( tree[(obj)].ob_spec.tedinfo->te_pvalid )
#define TedLen(obj)	( tree[(obj)].ob_spec.tedinfo->te_txtlen )
#define TedTempLen(obj)	( tree[(obj)].ob_spec.tedinfo->te_tmplen )
#define TedBorder(obj)	( tree[(obj)].ob_spec.tedinfo->te_thickness )
#define TedJust(obj)	( tree[(obj)].ob_spec.tedinfo->te_just )
#define TedFont(obj)	( tree[(obj)].ob_spec.tedinfo->te_font )
#define TedColor(obj)	( tree[(obj)].ob_spec.tedinfo->te_color )

#define ObString(obj)	( tree[(obj)].ob_spec.free_string )

#endif

#define ObX(obj)	( _OT(obj).ob_x )
#define ObY(obj)	( _OT(obj).ob_y )
#define ObW(obj)	( _OT(obj).ob_width )
#define ObH(obj)	( _OT(obj).ob_height )
#define ObRect(obj) 	( *(GRECT *)(&ObX(obj)) )

#define IconData(obj)	( tree[(obj)].ob_spec.iconblk->ib_pdata )
#define IconText(obj)	( tree[(obj)].ob_spec.iconblk->ib_ptext )
#define IconWidth(obj)	( tree[(obj)].ob_spec.iconblk->ib_wicon )
#define IconHeight(obj)	( tree[(obj)].ob_spec.iconblk->ib_hicon )
#define IconColor(obj)	( tree[(obj)].ob_spec.iconblk->ib_char )
#define IconTextH(obj)	( tree[(obj)].ob_spec.iconblk->ib_htext )
#define IconY(obj)		( tree[(obj)].ob_spec.iconblk->ib_yicon )

#define BitData(obj)	( tree[(obj)].ob_spec.bitblk )

#define Set_tree(obj)		( rsrc_gaddr(R_TREE,(obj),&tree) )
#define Set_alert(num,s)	( rsrc_gaddr(R_STRING,(num),&((OBJECT *)(s)) )
#define Set_button(num,s)	( rsrc_gaddr(R_STRING,(num),&((OBJECT *)(s)) )

#define IsSelectable(obj)	( ObFlags(obj) & SELECTABLE )
#define IsDefault(obj)		( ObFlags(obj) & DEFAULT )
#define IsExit(obj) 		( ObFlags(obj) & EXIT )
#define IsEditable(obj)		( ObFlags(obj) & EDITABLE )
#define IsRadio(obj)		( ObFlags(obj) & RBUTTON )
#define IsLastob(obj) 		( ObFlags(obj) & LASTOB )
#define IsTouchexit(obj)	( ObFlags(obj) & TOUCHEXIT )
#define IsHidden(obj)		( ObFlags(obj) & HIDETREE )
#define IsIndirect(obj) 	( ObFlags(obj) & INDIRECT )
#define IsSubMenu(obj)		( ObFlags(obj) & SUBMENU )

#define IsSelected(obj)		( ObState(obj) & SELECTED )
#define IsCrossed(obj)		( ObState(obj) & CROSSED )
#define IsChecked(obj)		( ObState(obj) & CHECKED )
#define IsDisabled(obj)		( ObState(obj) & DISABLED )
#define IsOutlined(obj) 	( ObState(obj) & OUTLINED )
#define IsShadowed(obj) 	( ObState(obj) & SHADOWED )
#define IsSpecial(obj)		( ObState(obj) & SPECIAL )

#define IsTed(obj) ((ObType(obj) & 0x00ff) == G_TEXT || (ObType(obj) & 0x00ff) == G_BOXTEXT || (ObType(obj) & 0x00ff) == G_FTEXT || (ObType(obj) & 0x00ff) == G_FBOXTEXT)

#define ActiveTree( newtree )	( tree = newtree )
#define IsActiveTree( newtree ) ( tree == newtree )

/* macros ok when object is not on screen
 */
#define HideObj(obj)		( ObFlags(obj) |= HIDETREE )
#define ShowObj(obj)		( ObFlags(obj) &= ~HIDETREE )
#define MakeEditable(obj)	( ObFlags(obj) |= EDITABLE )
#define NoEdit(obj)			( ObFlags(obj) &= ~EDITABLE )
#define MakeDefault(obj)	( ObFlags(obj) |= DEFAULT )
#define NoDefault(obj)		( ObFlags(obj) &= ~DEFAULT )
#define MakeExit(obj)		( ObFlags(obj) |= EXIT )
#define NoExit(obj)			( ObFlags(obj) &= ~EXIT )
#define MakeTouchExit(obj)	( ObFlags(obj) |= TOUCHEXIT )
#define NoTouchExit(obj)	( ObFlags(obj) &= ~TOUCHEXIT )

#define SetNormal(obj)		( ObState(obj) = NORMAL )
#define Select(obj)			( ObState(obj) |= SELECTED )
#define Deselect(obj)		( ObState(obj) &= ~SELECTED )
#define Disable(obj)		( ObState(obj) |= DISABLED )
#define Enable(obj)			( ObState(obj) &= ~DISABLED )
#define CheckObj(obj)		( ObState(obj) |= CHECKED )
#define UnCheckObj(obj)		( ObState(obj) &= ~CHECKED )
#define MakeShadow(obj)		( ObState(obj) |= SHADOWED )
#define NoShadow(obj)		( ObState(obj) &= ~SHADOWED )
#define MarkObj(obj)		( ObState(obj) |= SPECIAL )
#define UnmarkObj(obj)		( ObState(obj) &= ~SPECIAL )


/* use these when object is on screen - see Objc_change below
 */
#define deselect( tree, obj ) \
	Objc_change( (tree), (obj), NULL, (ObState(obj) & ~SELECTED), 1 )
#define selectobj( tree, obj ) \
	Objc_change( (tree), (obj), NULL, (ObState(obj) | SELECTED), 1 )
#define enable( tree, obj ) \
	Objc_change( (tree), (obj), NULL, (ObState(obj) & ~DISABLED), 1 )
#define disable( tree, obj ) \
	Objc_change( (tree), (obj), NULL, (ObState(obj) | DISABLED), 1 )


/* Functions not in TC bindings
 * ================================================================
 */

/* min & max - used by rc_functions
 * self explanatory (I hope!)
 */
WORD mymin( WORD a, WORD b );
WORD mymax( WORD a, WORD b );

/* rc (rect) functions
 */

/* Convert GRECT to VDI pxy array.
*/
void	rc_2xy( const GRECT *r, WORD *pxy );

/* Center `rd' within `rs', offset by `xoffset' and `yoffset'
 */
#ifdef LATTICE
void Rc_center(const GRECT *rs, WORD xoffset, WORD yoffset, GRECT *rd);
#else
void Rc_center(const GRECT *rs, GRECT *rd, WORD xoffset, WORD yoffset);
#endif

/* -WARNING- Psychotic typecasts! */
#undef rc_equal
#define rc_equal( r1, r2 ) \
	( *((long *)(r1)) == *((long *)(r2)) && \
	  *((long *)(&((r1)->g_w))) == *((long *)(&((r2)->g_w))) )
#define rc_empty( r ) ( !*(long *)(&((r)->g_w)) )

/* Inset `r' within itself by `xoffset' and `yoffset'
 */
void rc_inset(GRECT *r, WORD xoffset, WORD yoffset);
void rc_shrink(GRECT *r, WORD xoffset, WORD yoffset);
/* If `rs' and `rd' intersect, change `rd' to the intersection rect,
 * and return TRUE.  Otherwise, leave `rd' alone and return FALSE.
 */
BOOLEAN	rc_intersect(const GRECT *rs, GRECT *rd);
/* Change `rd' to the union of `rs' and `rd'.
 */
void rc_union( const GRECT *rs, GRECT *rd );

/* xy functions
 */
/* Convert VDI pxy array to rect
*/
void	xy_2rect( const WORD *pxy, GRECT *r );
/* Return TRUE if coordinates are within r
*/
BOOLEAN	xy_inrect( WORD x, WORD y, GRECT *r );



/* File selector utility functions
 * ================================================================
 */

void	fsel_init( void );
/* initialize file selector utility module
 */ 

WORD	fsel_name( BOOLEAN remember, char *name, const char *searchmask,
					const char *label );
/* Handle file and/or path selection.
 *
 * If `remember' is TRUE, the file selector module remembers
 * the path chosen by the user.
 * `name' must point to a string at least 128 characters long.
 *
 *	If path\filename supplied in `name', use it as default,
 *	  (NOTE: if caller supplies path\filename, there MUST be a filename,
 *		or the path must end in a backslash, otherwise fsel_name will
 *		think that the end of the path is the filename)
 * 	else use remembered default path.
 *  `searchmask' is default file mask for the file selector top line.
 *	If `searchmask' is NULL and a default path\filename is supplied,
 *	  use extender of filename as searchmask (*.ext), else use ``*.*''.
 *
 *	If `label' is non-NULL, use it as a prompt string for the file selector
 *	(using fsel_exinput if available, or a special dialog for older TOSes).
 *
 *	Return conditions:
 *	1)	If fsel_input returns an error, null the string in `name',
 *		and return the error code.
 *	1)	If user selects OK, copy full path\filename to `name',
 *		and return A_OK (TRUE).
 *	2)	If user selects OK with no file, copy backslash terminated path to
 *		`name' and return A_CANCEL (FALSE).
 *	3)	If user selects Cancel, null the string in `name'
 *		and return A_CANCEL.
 */

WORD	fsel_load( BOOLEAN remember, char *name, const char *searchmask,
					const char *label, VOIDP *buffer );
/* Select a file, Malloc a buffer for it, and read the file into the buffer.
 * Parameters as for fsel_name(); the address of the Malloc'ed
 * buffer is passed back in `buffer'.  Return any errors in buffer Malloc,
 * or Fopen/Fread.
 */



/* Warped AES bindings
 * ================================================================
 * Unless otherwise noted, these behave the same way as the
 * usual AES bindings for these functions.
 */


/* Event manager functions
 */
WORD	Evnt_button( WORD clicks, WORD bmask, WORD bstate, MRETS *m );
void	Evnt_mouse( const MOBLK *mblock, MRETS *m );
WORD	Evnt_multi( WORD flags, WORD clicks, WORD bmask, WORD bstate,
					const MOBLK *m1, const MOBLK *m2, WORD *msgbuf, long time,
					MRETS *m, WORD *key, WORD *nclicks );
/* Evnt_multi input struct pointers (m1, m2) may be NULL, but
 * output pointers (msgbuf, m, key, nclicks) must point to the
 * appropriate things
 */
void	Evnt_timer( long time );


/* Form manager functions
 */
void	Form_center( const OBJECT *tree, GRECT *r );
BOOLEAN Form_dial( FMD_FLAG flag, const GRECT *small, const GRECT *large );


/* Graphics manager functions
 */
BOOLEAN Graf_dragbox( GRECT *box, const GRECT *bound );
/* Pass start rect of box in `box', returns end rect of box in `box'.
 */
BOOLEAN	Graf_growbox( const GRECT *small, const GRECT *large );
#define Graf_movebox Graf_mbox
BOOLEAN	Graf_mbox( const GRECT *box, WORD endx, WORD endy );
void	Graf_mkstate( MRETS *m );
BOOLEAN	Graf_rubberbox( GRECT *r );
/* Pass min rect of box in `r', returns end rect of box in `r'.
 */
BOOLEAN	Graf_shrinkbox( const GRECT *small, const GRECT *large );


/* Object manager functions
 * If `clip' is NULL in Objc_draw or _change, the binding will calculate
 * a clip rectangle for you, based on the extent of the object.
 */
BOOLEAN Objc_change( OBJECT *tree, WORD object, const GRECT *clip,
						WORD state, BOOLEAN redraw );
BOOLEAN Objc_draw(OBJECT *tree, WORD startob, WORD depth, const GRECT *clip);
BOOLEAN ObjcDraw(OBJECT *tree, WORD startob, WORD depth, const GRECT *clip);

/* Window manager functions
 */
WORD	Wind_create( WORD kind, const GRECT *fullsize );
BOOLEAN	Wind_get( WORD id, WFLAG which, WARGS *args );
BOOLEAN	Wind_open( WORD id, const GRECT *size );
BOOLEAN	Wind_set( WORD id, WFLAG which, const WARGS *args );

#endif /* __AESALT__ */
