#ifndef STRUCT88_H
#define MC68K 1
#define I8086 0
#include "../aes/struct88.h"
#endif

#define WX_VERSION    0x220
#define WX_DATE       0x1b51 /* 1993/10/17 */

#define WX_QUEUE_SIZE 0x500
#define WX_MAXWIN     40

#ifndef WF_WINX
#define WF_WINX         0x5758
#define WF_WINXCFG      0x5759
#define WF_DDELAY       0x575a
#endif
#define WF_APG11        0x575b
#define WF_APG12        0x575c

#define W_SMALLER   19
#define W_BOTTOMER  20

#define WX_MAXOBJ   23

typedef struct wx_pd
{
	/*  0 */	PD	*p_link;			/* link to other process	*/
	/*  4 */	PD	*p_thread;			/* I don't think it is used	*/
	/*  8 */	UDA	*p_uda;				/* store the machine's status */

	/* ^^^ variables above are accessed from assembler code */
	
	/* 12 */	char	p_name[AP_NAMELEN];		/* processor name (not NUL terminated) */

	/* 20 */	CDA	*p_cda;				/* Tells what we are waiting 	*/
	/* 24 */	intptr_t p_ldaddr;		/* long address of load		*/
	/* 28 */	int16_t p_pid;			/* process id number		*/
	/* 30 */	int16_t	p_stat;			/* PS_RUN or PS_MWAIT		*/

	/* 32 */	EVSPEC	p_evbits;		/* event bits in use 8 max EVB	*/
	/* 34 */	EVSPEC	p_evwait;		/* event wait mask 		*/
	/* 36 */	EVSPEC	p_evflg;		/* EVB that satisfied		*/
	/* 38 */	BOOLEAN	p_msgtosend;
	/* 40 */	int16_t	p_message[9];
	/* 58 */	MFORM	p_mouse;		/* saved mouseform for M_SAVE/M_RESTORE */
	/*132 */	EVB	*p_evlist;			/* link to EVB			*/
	/*136 */	EVB	*p_qdq;
	/*140 */	EVB	*p_qnq;
	/*144 */	char *p_qaddr;			/* message queue pointer	*/
	/*148 */	int16_t	p_qindex;		/* message queue index		*/

	/* ^^^ variables above must be identical to AES PD */
	
	/*150 */	uint16_t p_flags;
} WX_PD;

typedef struct wx_window
{
	/*  0 */	int16_t		w_flags;
	/*  2 */	PD			*w_owner;
	/*  6 */	int16_t		w_kind;
	/*  8 */	char		*w_pname;
	/* 12 */	char		*w_pinfo;
	/* 16 */	GRECT		w_full;
	/* 24 */	GRECT		w_work;
	/* 32 */	GRECT		w_prev;
	/* 40 */	GRECT		w_curr;
	/* 48 */    char unused1[10];
	/* 58 */	int16_t		w_hslide;
	/* 60 */	int16_t		w_vslide;
	/* 62 */	int16_t		w_hslsiz;
	/* 64 */	int16_t		w_vslsiz;
	/* 66 */    int16_t     w_tcolor[WX_MAXOBJ]; /* object colors if window is topped */
	/* 112 */   int16_t     w_bcolor[WX_MAXOBJ]; /* object colors if window is in background */
	/* 158 */	ORECT		*w_rlist;	/* owner rectangle list	*/
	/* 162 */	ORECT		*w_rnext;	/* used for search first search next */

	/* 166 */   char unused2[124];
	/* 290 */
} WX_WINDOW;

typedef struct {
    /*     0 */ WX_WINDOW win[WX_MAXWIN];
    /* 11600 */ char queue[MAX_ACCS + 2][WX_QUEUE_SIZE];
    /* 21840 */ int16_t init_scroll;
    /* 21842 */ int16_t cont_scroll;
    /* 21844 */ int16_t cont_close;
    /* 21846 */ int16_t cont_full;
    /* 21848 */ int16_t tcolor[WX_MAXOBJ];
    /* 21894 */ int16_t bcolor[WX_MAXOBJ];
    /* 21940 */ char unused[1216];
    /* 23156 */
} WX_MEM;

struct wxinfo {
	WX_MEM *wx_mem;
	int16_t maxwin;
	uint16_t size_win;
	PD **rlr;
	uint16_t off_queue;
	uint16_t queue_size;
	/* LPTREE */ VOIDPTR *newdesk;
	int16_t *newroot;
	/* LPTREE */ VOIDPTR *stdesk;
	VOIDPTR func;
};

struct _winxvars {
	/* $50 */ int32_t magic;         /* xAEF2 */
	/* $54 */ uint16_t version;      /* xAEF6 */
	/* $56 */ uint16_t date;         /* xAEF8 */
	/* $58 */ uint16_t global_flags; /* xAEFA */
	/* $5a */ WX_MEM *wx_mem;        /* xAEFC */
	/* $5e */ const struct wxinfo *info;   /* xAF00 */
	/* $62 */ int32_t (CDECL *winxfunc) PROTO((short funcid, VOIDPTR arg));        /* xAF04 */
	/* $66 */ int16_t wbox;          /* xAF08 */
	/* $68 */ int16_t hbox;          /* xAF0A */
	/* $6c */ intptr_t ret_pc;       /* return pc of gem_main(); not used in C version */
	/* $6e */ char xAF10;
	/* $6f */ char xAF11;
};
extern struct _winxvars winxvars;

/*
 * global flags
 */
#define WX_40WIN      0x0001 /* 40 window handles */
#define WX_RBSCREEN   0x0002 /* right button on frame goes to screen mgr */
#define WX_LBREAL     0x0004 /* left button activates real time functions */
#define WX_ARROWACT   0x0008 /* activate scroll arrows */
#define WX_ACTSLIDER  0x0010 /* activate sliders */
#define WX_ACTMOVER   0x0020 /* activate mover */
#define WX_ACTSIZER   0x0040 /* activate sizer */
#define WX_WIDESLIDER 0x0080 /* wide slider background */
#define WX_BGACT      0x0100 /* activate background windows with CONTROL */
#define WX_FGBACK     0x0200 /* deactivate foreground window on title */
#define WX_3DLOOK     0x0400 /* 3D-look for window frame */
#define WX_CORR       0x0800 /* pattern reference point correction */
#define WX_PEXEC      0x1000 /* use application name for appl_find() */
#define WX_MOUSE      0x2000 /* change mouse cursor for realtime functions */

/*
 * application flags
 */
#define WX_BGCTRL     0x0001 /* Control elements for background windows */
#define WX_MINFRAME   0x0002 /* Minimal amount of control elements */
#define WX_ONTOP      0x0004 /* Application accepts WM_ONTOP/WM_UNTOPPED */
#define WX_OPTREDRAW  0x0008 /* Optimized redraw for activation */
#define WX_OPTMOVE    0x0010 /* Optimized redraw for moving */
#define WX_OPTSIZE    0x0020 /* Optimized redraw for sizing */
#define WX_MERGE      0x0040 /* Merge redraw messages */
#define WX_ALERT      0x0080 /* Alert on wrong AES calls */
#define WX_SSCROLL    0x0100 /* Separate scroll arrows */
#define WX_BSCROLL    0x0200 /* Scroll arrows with boxes */
#define WX_NOCLIP     0x0400 /* Allow to move windows outside of screen */
#define WX_WCORR      0x0800 /* Fix wrong window positions */



int16_t wx_create PROTO((uint16_t kind, GRECT *rect));
int16_t wx_open PROTO((int16_t handle, GRECT *rect));
int16_t wx_close PROTO((int16_t handle));
int16_t wx_delete PROTO((int16_t handle));
int16_t wx_get PROTO((int16_t handle, int16_t field, int16_t *ow, const int16_t *iw));
int16_t wx_set PROTO((int16_t handle, int16_t field, int16_t *iw));
int16_t wx_find PROTO((int mx, int my));
int16_t wx_update PROTO((int code));
VOID wx_calc PROTO((int16_t type, int16_t kind, int16_t ix, int16_t iy, int16_t iw, int16_t ih, int16_t *ox, int16_t *oy, int16_t *ow, int16_t *oh));
int16_t wx_new PROTO((NOTHING));
VOID wx_setactive PROTO((NOTHING));
VOID wx_drawdesk PROTO((GRECT *dirty));
short spl7 PROTO((NOTHING));
VOID spl PROTO((short));
PD *wx_srchwp PROTO((int16_t wh, int16_t button));
VOID wx_nameit PROTO((PD *p, const char *pname));
VOID wx_rmerge PROTO((int16_t *nm, int16_t *om));
VOID wx_getmouse PROTO((NOTHING));
VOIDPTR wx_alloc PROTO((long size));
BOOLEAN wx_start PROTO((NOTHING));
BOOLEAN wx_init PROTO((NOTHING));
BOOLEAN wx_callfunc PROTO((int16_t func, VOIDPTR arg, int32_t *val));
