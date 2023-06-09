/*
 *************************************************************************
 *			Revision Control System
 * =======================================================================
 *  $Author: mui $	$Date: 89/04/26 18:25:02 $
 * =======================================================================
 *
 * Revision 2.2  89/04/26  18:25:02  mui
 * TT
 * 
 * Revision 2.1  89/02/22  05:28:06  kbad
 * *** TOS 1.4  FINAL RELEASE VERSION ***
 * 
 * Revision 1.1  88/06/02  12:34:26  lozben
 * Initial revision
 * 
 *************************************************************************
 */
/*	GEMLIB.H	03/15/84 - 01/07/85	Lee Lorenzen		*/
/*	EVNTLIB.H	03/15/84 - 05/16/84	Lee Lorenzen		*/
/*	to 68k		03/08/85 - 05/11/85	Lowell Webster		*/
/*	Add defines	11/25/87		D.Mui			*/
/*	Update		01/06/88		D.Mui			*/
/*	Add cmdlen	1/13/88			D.Mui			*/
/*	Add g_path	1/28/88			D.Mui			*/
/*	Add SAVE_AFILE	2/16/88			D.Mui			*/
/*	Change SIZE_AFILE	04/22/88	D.Mui			*/

/*	Be careful when changing the THEGLO				*/
/*	Gembind.s Gemoblib.c may be affected 				*/

/*	Change these defines and you have to change those in deskapp.h */

#ifndef GEMLIB_H
#define GEMLIB_H 1

#define NUM_ANODES 32				/* # of appl. nodes	*/
#define PATH_LEN 121				/* define maximum path length */
#define SAVE_ATARI 128				/* atari specific bytes in DESKTOP.INF file size of DESKTOP.INF	file */

/* max string length	*/
#define MAX_LEN 81

#define EXTENSION  6

/* size of DESKTOP.INF file */
#define SIZE_AFILE ((NUM_ANODES*PATH_LEN)+(NUM_ANODES*EXTENSION)+SAVE_ATARI)
#ifdef SHBUFSIZE /* TP_47 */
# if SHBUFSIZE < SIZE_AFILE
#   undef SIZE_AFILE /* to generate compile error */
# else
#   undef SIZE_AFILE
#   define SIZE_AFILE SHBUFSIZE
# endif
#endif

/***********************************************************************/

/*	MENULIB.H	05/04/84 - 11/01/84	Lowell Webster	*/

#define MTH 1					/* menu thickness	*/

#define THESCREEN 0
#define THEBAR 1
#define THEACTIVE 2
#define THEMENUS ( LWGET(OB_TAIL(THESCREEN)) )

/*** STATE DEFINITIONS FOR menu_state ***********************************/

#define INBAR	1	/* mouse position	outside menu bar & mo dn */
			/* multi wait		mo up | in menu bar	*/
			/* moves		-> 5  ,  ->2		*/

#define OUTTITLE 2  	/* mouse position	over title && mo dn	*/
			/* multiwait		mo up | out title rect	*/
			/* moves		-> 5  ,	->1 ->2  ->3	*/

#define OUTITEM 3 	/* mouse position	over item && mo dn	*/
			/* multi wait		mo up | out item rect	*/
			/* moves		-> 5  , ->1 ->2 ->3 ->4	*/

#define INBARECT 4	/* mouse position	out menu rect && bar && mo dn*/			/* multi wait	mo up | in menu rect | in menu bar */
			/* moves	-> 5  ,	-> 3	     , -> 2	*/



/*	APPLLIB.H	05/05/84 - 10/16/84	Lee Lorenzen		*/

#define SCR_MGR 0x0001			/* pid of the screen manager*/

/*	FSELLIB.H	05/05/84 - 01/07/85	Lee Lorenzen		*/

#define RMODE_RD 0
#define RMODE_WR 1
#define RMODE_RW 2

#define SMODE 0				/* seek mode is absolute offset	*/
#define F_ATTR 0			/* file attribute for creating	*/

/* these must coincide w/ rshdr */
#define RT_VRSN 0
#define RT_OB 1
#define RT_TEDINFO 2
#define RT_ICONBLK 3
#define RT_BITBLK 4
#define RT_FREESTR 5
#define RT_STRING 6
#ifndef __ALCYON__ /*  clashes with RT_IMAGES */
#define RT_IMAGEDATA 7
#endif
#define	RT_FREEIMG 8
#define RT_TRINDEX 9

#define NUM_RTYPES 10

#define RT_NOBS 10
#define RT_NTREE 11
#define RT_NTED 12
#define RT_NICON 13
#define RT_NBITBLK 14
#define RT_NSTRING 15
#define RT_IMAGES 16

#define NUM_RN 7

#define RS_SIZE (NUM_RTYPES + NUM_RN)

#define HDR_LENGTH ((RS_SIZE + 1) * 2)		/* in bytes	*/


/*	WINDLIB.H	05/05/84 - 01/26/85	Lee Lorenzen		*/

#define WS_FULL 0
#define WS_CURR 1
#define WS_PREV 2
#define WS_WORK 3
#define WS_TRUE 4



#define	TPARTS	(NAME|CLOSER|FULLER|MOVER)
#define	VPARTS	(UPARROW|DNARROW|VSLIDE)
#define	HPARTS	(LFARROW|RTARROW|HSLIDE)


#define	HASHSIZ	8		/* size of hash table */
#define	NUMRECT	80		/* # RLISTs per block of memory allocated */
#define	SHADOW	2		/* thickness of drop shadow in pixels */


/* Bit masks of pieces of a window */
#define TOP		0x0001
#define LEFT	0x0002
#define RIGHT	0x0004
#define BOTTOM	0x0008


/* Indices into object tree of window */
#define	W_BOX	    0		/* background of window */
#define	W_TITLE	    1		/* parent of closer, name and fuller */
#define	W_CLOSER    2		/* closer */
#define	W_NAME	    3		/* name and mover bar */
#define	W_FULLER    4		/* fuller */
#define	W_INFO	    5		/* info line */
#define	W_DATA	    6		/* holds remaining window elements */
#define	W_WORK	    7		/* application work area */
#define	W_SIZER	    8		/* sizer */
#define	W_VBAR	    9		/* holds vertical slider elements */
#define	W_UPARROW   10		/* vertical slider up arrow */
#define	W_DNARROW   11		/* vertical slider down arrow */
#define	W_VSLIDE    12		/* vertical slider background */
#define	W_VELEV	    13		/* vertical slider thumb/elevator */
#define	W_HBAR	    14		/* holds horizontal slider elements */
#define	W_LFARROW   15		/* horizontal slider left arrow */
#define	W_RTARROW   16		/* horizontal slider right arrow */
#define	W_HSLIDE    17		/* horizontal slider background */
#define	W_HELEV	    18		/* horizontal slider thumb/elevator */
#if AESVERSION >= 0x330
#define	W_MENUBAR	19		/* menu bar (added Jul 23 91 - ml.) */
#endif


/* Callers of totop() */
#define	WMOPEN	0		/* called from wm_open() */
#define	WMCLOSE	1		/* called from wm_close() */
#define	WMSET	2		/* called from wm_set() */


/* Ob_spec values */
#define	GBOX_OBS    0x00010000L	/* ob_spec value for GBOX objects */


/* Memory descriptor structure */
typedef	struct	memhdr {
    int	    numused;		/* number of window structures used */
    struct  memhdr *mnext;	/* pointer to next memory descriptor */
} MEMHDR;


/* Rectangle list structure */
typedef	struct	rlist {
    struct {
	unsigned rused : 1;	/* bit 0 ->1: slot is currently used */
    } rstatus;
    GRECT   rect;		/* the rectangle dimensions */
    MEMHDR  *rwhere;		/* ptr to memory this RLIST resides in */
    struct  rlist *rnext;	/* ptr to next RLIST */
} RLIST;


#if NEWWIN

/* Window structure */
typedef	struct	window {
    struct {
	unsigned used : 1;	/* bit 0 -> 1: slot is currently used */
	unsigned opened : 1;	/* bit 1 -> 1: window is currently opened */
    } status;			/* window status */
    PD	    *w_owner;		/* owner of this window */
    uint16_t   w_mowner;	/* mouse owner of this window */
    uint16_t   w_handle;	/* window handle */
    uint16_t   w_kind;		/* flag for components of window */
    OBJECT  *aesobj;		/* for use in AES */
    OBJECT  obj[MAXOBJ];	/* definition of each object */
#if AES3D
    int16_t    w_tcolor[MAXOBJ];	/* object colors if window is topped */
    int16_t    w_bcolor[MAXOBJ];	/* object colors if window is in background */
#endif
    TEDINFO ttxt;			/* title bar text */
    TEDINFO itxt;			/* info line text */
    TEDINFO mtxt;			/* menu bar text */
    GRECT   w_full;			/* coords and size when full */
    GRECT   w_prev;			/* previous coords and size */
    GRECT   w_curr;			/* current coords and size */
    RLIST   *fxywh;			/* first rect in rectangle list */
    RLIST   *nxywh;			/* next rect in rectangle list */
							/* slider positions and sizes are in 1-1000
							   range and relative to the scroll bar */
    uint16_t   w_hslide;	/* horizontal slider position */
    uint16_t   w_vslide;	/* vertical slider position */
    uint16_t   w_hslsiz;	/* horizontal slider size */
    uint16_t   w_vslsiz;	/* vertical slider size */
    uint16_t   ontop;		/* handle # of window on top */
    uint16_t   under;		/* handle # of window under */
    uint16_t   nxthndl;		/* next handle # in used */
    uint16_t   parent;		/* handle # of parent window */
    uint16_t   child;		/* handle # of 1st child window */
    uint16_t   type;		/* window's characteristics	*/
    uint16_t   sibling;		/* handle # of next sibling window */
    MEMHDR  *wwhere;		/* ptr to memory this WINDOW resides in */
    struct  window *wnext;	/* ptr to next WINDOW in database */
} WINDOW;

#else

#define VF_INUSE 0x0001
#define VF_BROKEN 0x0002
#define VF_INTREE 0x0004
#define VF_SUBWIN 0x0008
#define VF_KEEPWIN 0x0010

typedef struct window
{
	/*  0 */	int16_t		w_flags;
	/*  2 */	PD			*w_owner;
	/*  6 */	int16_t		w_kind;
	/*  8 */	char		*w_pname;
	/* 12 */	char		*w_pinfo;
	/* 16 */	GRECT		w_full;
	/* 24 */	GRECT		w_work;
	/* 32 */	GRECT		w_prev;
	/* 40 */	int16_t		w_hslide;
	/* 42 */	int16_t		w_vslide;
	/* 44 */	int16_t		w_hslsiz;
	/* 46 */	int16_t		w_vslsiz;
#if (AESVERSION >= 0x200) | TP_WINX
	/* 48 */    int16_t     w_tcolor[MAXOBJ];	
	/* 86 */    int16_t     w_bcolor[MAXOBJ];	
#endif
	/* 124 */	ORECT		*w_rlist;	/* owner rectangle list	*/
	/* 128 */	ORECT		*w_rnext;	/* used for search first search next */
	/* 132 */
} WINDOW;

#define NUM_ORECT (NUM_WIN * 10)	/* is this enough???	*/

/* system foreground and* background rules but transparent */
#define SYS_FG 0x1100
/* window title selected using pattern 2 & replace mode text */
#define WTS_FG 0x11a1
/* window title normal */
#define WTN_FG 0x1100

#endif

#define	ADJ3DPIX    2	/* pixel adjustment for 3D objects */

#define G_SIZE 15

/* system has 3 pds to start - app, control mgr, acc	*/

#define THEGLO struct glstr
THEGLO
{
	UDA		g_uda;			/* must be first */
	UDA2	g_2uda;
	UDA3	g_3uda;
	PD g_pd[NUM_PDS];
	CDA	g_cda[NUM_PDS];
	EVB	g_evb[NUM_EVBS];

	FPD	g_fpdx[NFORKS];

#if !NEWWIN
	ORECT 	g_olist[NUM_ORECT];
#endif

	char	g_rawstr[MAX_LEN];	/* used in gemoblib.s	*/
	char	g_tmpstr[MAX_LEN];	/* used in gemoblib.s	*/
	char	g_valstr[MAX_LEN];	/* used in gembind.s	*/
	char	g_fmtstr[MAX_LEN];	/* used in gemoblib.s	*/

	char	g_loc1[256];		/* MAX alert length	*/
	char	g_loc2[256];

	char    g_scrap[164];		/* was: WORD[82]... */

	char	g_dir[CMDLEN];		/* changed from 82 to 128 */

	uint16_t g_sysglo[G_SIZE];

	char	g_path[CMDLEN];		/* new element		*/

	char	s_cmd[CMDLEN];
#if TP_47 /* SHBUF */
	char	*s_save;
#else
	char	s_save[SIZE_AFILE];
#endif
	char	s_tail[CMDLEN];

#if !NEWWIN
	WINDOW	w_win[NUM_WIN];
#endif
};

#endif /* GEMLIB_H */
