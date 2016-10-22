/*
*************************************************************************
*			Revision Control System
* =======================================================================
*  $Revision: 2.2 $	$Source: /u2/MRS/osrevisions/aes/obdefs.h,v $
* =======================================================================
*  $Author: mui $	$Date: 89/04/26 18:29:21 $	$Locker: kbad $
* =======================================================================
*  $Log:	obdefs.h,v $
* Revision 2.2  89/04/26  18:29:21  mui
* TT
* 
* Revision 2.1  89/02/22  05:31:23  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  88/06/02  12:35:57  lozben
* Initial revision
* 
*************************************************************************
*/
/*	OBDEFS.H	03/15/84 - 02/08/85	Gregg Morris		*/
/*	Add 3D definitons		7/7/92	D.Mui			*/
/*	Add another set of 3D definitons	8/1/92	D.Mui		*/
/*	LK3DIND	etc							*/	

#define ROOT 0
#define NIL -1

/* keybd states		*/
#define K_RSHIFT 0x0001
#define K_LSHIFT 0x0002
#define K_CTRL 0x0004
#define K_ALT 0x00008
/* max string length	*/
#define MAX_LEN 81
						/* max depth of search	*/
						/*   or draw for objects*/
#define MAX_DEPTH 8
						/* inside patterns	*/
#define IP_HOLLOW 0
#define IP_1PATT 1
#define IP_2PATT 2
#define IP_3PATT 3
#define IP_4PATT 4
#define IP_5PATT 5
#define IP_6PATT 6
#define IP_SOLID 7
						/* system foreground and*/
						/*   background rules	*/
						/*   but transparent	*/
#define SYS_FG 0x1100

#define WTS_FG 0x11a1				/* window title selected*/
						/*   using pattern 2 &	*/
						/*   replace mode text 	*/
#define WTN_FG 0x1100				/* window title normal	*/
						/* gsx modes		*/
#define MD_REPLACE 1
#define MD_TRANS 2
#define MD_XOR 3
#define MD_ERASE 4
						/* gsx styles		*/
#define FIS_HOLLOW 0
#define FIS_SOLID 1
#define FIS_PATTERN 2
#define FIS_HATCH 3
#define FIS_USER 4
						/* bit blt rules	*/
#define ALL_WHITE 0
#define S_AND_D 1
#define S_ONLY 3
#define NOTS_AND_D 4
#define S_XOR_D 6
#define S_OR_D 7
#define D_INVERT 10
#define NOTS_OR_D 13
#define ALL_BLACK 15
						/* font types		*/
#define IBM 3
#define SMALL 5

/* Object Drawing Types */
						/* Graphic types of obs	*/
#define G_BOX 20
#define G_TEXT 21
#define G_BOXTEXT 22
#define G_IMAGE 23
#define G_USERDEF 24
#define G_IBOX 25
#define G_BUTTON 26
#define G_BOXCHAR 27
#define G_STRING 28
#define G_FTEXT 29
#define G_FBOXTEXT 30
#define G_ICON 31
#define G_TITLE 32
#define G_CICON	33

#if 0
/* no longer in effect; we use DRAW3D and ACT3D now: ++ERS 1/12/93 */
/* New Object Drawing Types for the 3D-look (May 22 1992 - ml) */

#define	X_3DIND		1	/* extended type for 3D indicator objects */
#define	X_3DACT		2	/* extended type for 3D action objects */

/* Object types of 3D indicators */
#define I3DBOX		((X_3DIND << 8) | G_BOX)
#define I3DBOXTEXT	((X_3DIND << 8) | G_BOXTEXT)
#define I3DBUTTON	((X_3DIND << 8) | G_BUTTON)
#define I3DBOXCHAR	((X_3DIND << 8) | G_BOXCHAR)
#define I3DFBOXTEXT	((X_3DIND << 8) | G_FBOXTEXT)

/* Object types with 3D actions */
#define A3DBOX		((X_3DACT << 8) | G_BOX)
#define A3DBOXTEXT	((X_3DACT << 8) | G_BOXTEXT)
#define A3DBUTTON	((X_3DACT << 8) | G_BUTTON)
#define A3DBOXCHAR	((X_3DACT << 8) | G_BOXCHAR)
#define A3DFBOXTEXT	((X_3DACT << 8) | G_FBOXTEXT)
#endif

/* 3D-object options for look-and-feel */
#define	OPT1	1
#define	OPT2	2
#define	OPT3	3
#define	OPT4	4
#define	OPT5	5

/* 6-18-92 - ml.    Object mode for ob_xtend */
#define	LK3DIND	    1	/* look of 3D indicators */
#define	LK3DACT	    2	/* look of 3D action objects */
#define	BUTCOL	    3	/* color of buttons */

/* #define	ALRTCOL	    4	*/ /* color of alert background */

						/* Object flags		 */
#define NONE 0x0
#define SELECTABLE 0x1
#define DEFAULT 0x2
#define EXIT 0x4
#define EDITABLE 0x8
#define RBUTTON 0x10
#define LASTOB 0x20
#define TOUCHEXIT 0x40
#define HIDETREE 0x80
#define INDIRECT 0x100
#define	IS3DOBJ	 0x200		/* set for any 3D object */
#define	IS3DACT	 0x400		/* 0 for indicator, 1 for activator */
#define SUBMENU	0x800
						/* Object states	*/
#define NORMAL 0x0
#define SELECTED 0x1
#define CROSSED 0x2
#define CHECKED 0x4
#define DISABLED 0x8
#define OUTLINED 0x10
#define SHADOWED 0x20
#if 0
#define DRAW3D	0x40
#endif
#define WHITEBAK 0x80

						/* Object colors	*/
#define WHITE 0
#define BLACK 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define CYAN 5
#define YELLOW 6
#define MAGENTA 7
#define LWHITE 8
#define LBLACK 9
#define LRED 10
#define LGREEN 11
#define LBLUE 12
#define LCYAN 13
#define LYELLOW 14
#define LMAGENTA 15

/* 7-30-92 - ml.    3D objects */

#define	ADJ3DPIX    2	/* pixel adjustment for 3D objects */
#define	LK3DIND	    1	/* change look of 3D indicators */
#define	LK3DACT	    2	/* change look of 3D action objects */
#define	INDBUTCOL   3	/* change color of buttons */
#define	ACTBUTCOL   4	/* change color of buttons */
#define	ALRTCOL	    5	/* change color of alert background */
#define AD3DVALUE   6


#define OBJECT struct object

OBJECT
{
	int16_t		ob_next;	/* -> object's next sibling	*/
	int16_t		ob_head;	/* -> head of object's children */
	int16_t		ob_tail;	/* -> tail of object's children */
	uint16_t		ob_type;	/* type of object- BOX, CHAR,...*/
	uint16_t		ob_flags;	/* flags			*/
	uint16_t		ob_state;	/* state- SELECTED, OPEN, ...	*/
	int32_t		ob_spec;	/* "out"- -> anything else	*/
	int16_t		ob_x;		/* upper left corner of object	*/
	int16_t		ob_y;		/* upper left corner of object	*/
	int16_t		ob_width;	/* width of obj			*/
	int16_t		ob_height;	/* height of obj		*/
};

#define ORECT	struct orect

ORECT
{
	ORECT	*o_link;
	int16_t	o_x;
	int16_t	o_y;
	int16_t	o_w;
	int16_t	o_h;
} ;


#define GRECT	struct grect

GRECT
{
	int16_t	g_x;
	int16_t	g_y;
	int16_t	g_w;
	int16_t	g_h;
} ;


#define TEDINFO struct text_edinfo

TEDINFO
{
	int32_t		te_ptext;	/* ptr to text (must be 1st)	*/
	int32_t		te_ptmplt;	/* ptr to template		*/
	int32_t		te_pvalid;	/* ptr to validation chrs.	*/
	int16_t		te_font;	/* font				*/
	int16_t		te_junk1;	/* junk word			*/
	int16_t		te_just;	/* justification- left, right...*/
	int16_t		te_color;	/* color information word	*/
	int16_t		te_junk2;	/* junk word			*/
	int16_t		te_thickness;	/* border thickness		*/
	int16_t		te_txtlen;	/* length of text string	*/
	int16_t		te_tmplen;	/* length of template string	*/
};


#define ICONBLK struct icon_block

ICONBLK
{
	int32_t	ib_pmask;
	int32_t	ib_pdata;
	int32_t	ib_ptext;
	int16_t	ib_char;
	int16_t	ib_xchar;
	int16_t	ib_ychar;
	int16_t	ib_xicon;
	int16_t	ib_yicon;
	int16_t	ib_wicon;
	int16_t	ib_hicon;
	int16_t	ib_xtext;
	int16_t	ib_ytext;
	int16_t	ib_wtext;
	int16_t	ib_htext;
};

#define BITBLK struct bit_block

BITBLK
{
	int32_t	bi_pdata;		/* ptr to bit forms data	*/
	int16_t	bi_wb;			/* width of form in bytes	*/
	int16_t	bi_hl;			/* height in lines		*/
	int16_t	bi_x;			/* source x in bit form		*/
	int16_t	bi_y;			/* source y in bit form		*/
	int16_t	bi_color;		/* fg color of blt 		*/
};


#define USERBLK struct user_blk
USERBLK
{
	int32_t	ub_code;
	int32_t	ub_parm;
};

#define PARMBLK struct parm_blk
PARMBLK
{
	int32_t	pb_tree;
	int16_t	pb_obj;
	int16_t	pb_prevstate;
	int16_t	pb_currstate;
	int16_t	pb_x, pb_y, pb_w, pb_h;
	int16_t	pb_xc, pb_yc, pb_wc, pb_hc;
	int32_t	pb_parm;
};


typedef struct cicon_data {
    int num_planes;
    int *col_data;
    int *mask;
    int *sel_data;
    int *sel_mask;
    struct cicon_data *next_res;
} CICON;

typedef struct cicon_blk {
    ICONBLK monoblk;
    CICON *mainlist;	/* list of color icons for different res */
} CICONBLK;

#define EDSTART 0
#define EDINIT 1
#define EDCHAR 2
#define EDEND 3

#define TE_LEFT 0
#define TE_RIGHT 1
#define TE_CNTR 2
