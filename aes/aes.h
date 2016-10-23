/*
 *************************************************************************
 *			Revision Control System
 * =======================================================================
 *  $Revision: 2.2 $	$Source: /u2/MRS/osrevisions/aes/machine.h,v $
 * =======================================================================
 *  $Author: mui $	$Date: 89/04/26 18:29:04 $	$Locker: kbad $
 * =======================================================================
 *  $Log:	machine.h,v $
 * Revision 2.2  89/04/26  18:29:04  mui
 * TT
 * 
 * Revision 2.1  89/02/22  05:02:47  kbad
 * *** TOS 1.4  FINAL RELEASE VERSION ***
 * 
 * Revision 1.5  89/02/16  18:01:27  kbad
 * Moved M132 define here from flavor.h
 * 
 * Revision 1.4  88/09/08  18:52:58  kbad
 * Moved build-specific stuff to flavor.h
 * 
 *
 * Revision 1.3  88/08/02  13:02:49  kbad
 * Set with parameters of 1.04d 8/1/88 build
 * add DENMARK, fix NORWAY, FINLAND
 *
 * Revision 1.2  88/07/15  16:07:50  mui
 * add flag DOWARNING to do AES startup alert box
 * 
 * Revision 1.1  88/06/02  12:35:52  lozben
 * Initial revision
 * 
 *************************************************************************
 */
/*	MACHINE.H		09/29/84 - 10/09/84	Lee Lorenzen	
 *	for 68k			10/10/84 - 04/22/85	Lowell Webster	
 *	Update			05/07/85		Derek Mui	
 *	Add more define		07/12/85		Derek Mui	
 *	add MULRES		3/25/86			Derek Mui
 *	add SWISSGER & SWISSFRA defines 8/3/87		Slavik Lozben
 *	add MEXICO defines      9/28/87 		Slavik Lozben
 *	Clean up		11/23/87		Derek Mui
 * 	Added 132 column switch	12/18/87		D.Mui		
 *	add FINLAND		1/8/88			D.Mui	
 */
	

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __STDINT_H__
#include <stdint.h>
#endif

#include "../common/config.h"
#include "aesdefs.h"

#if TOSVERSION >= 0x0100
#undef AESVERSION
#define AESVERSION 0x0120
#endif

#if TOSVERSION >= 0x0104
#undef AESVERSION
#define AESVERSION 0x0140
#endif

#if TOSVERSION >= 0x0201
#undef AESVERSION
#define AESVERSION 0x0310
#endif

#if TOSVERSION >= 0x0206
#undef AESVERSION
#define AESVERSION 0x0320
#endif

#if TOSVERSION >= 0x0301
#undef AESVERSION
#define AESVERSION 0x0300
#endif

#if TOSVERSION >= 0x0306
#undef AESVERSION
#define AESVERSION 0x0320
#endif

#if TOSVERSION >= 0x0400
#undef AESVERSION
#define AESVERSION 0x0340
#endif

typedef int BOOLEAN;
#define FALSE 0
#define TRUE  1


#define AES3D (AESVERSION >= 0x340)


#include "struct88.h"
#include "obdefs.h"
#include "gemlib.h"


/*
 * coerce short ptr to low word  of long
 */
#define LW(x) ( (int32_t)((uint16_t)(x)) )

/*
 * coerce short ptr to high word  of lonh
 */
#define HW(x) ((int32_t)((uint16_t)(x)) << 16)

/*
 * return low word of	a long value
 */
#define LLOWD(x) ((uint16_t)(x))

/*
 * return high word of a long value
 */
#define LHIWD(x) ((uint16_t)(x >> 16))

/*
 * return high byte of a word value
 * added 12/03/84
 */
#define LLOBT(x) ((char)(x))

/*
 * return 0th byte of a long value given
 * a short pointer to the long value
 * added 12/03/84
 */
#define LHIBT(x) ((char)(x >> 8))

/*
 * return a long address of a short pointer
 */
#define ADDR(x) x

/*
 * return long address of the data seg
 */
#define LLDS() ((int32_t)0)

/*
 * return long address of the code seg
 */
#define LLCS() ((int32_t)0)

/*
 * return a single byte	pointed at by long ptr
 */
#define LBGET(x) ( (UBYTE) *((char * )(x)) )

/*
 * set a single byte pointed at by long ptr, LBSET(lp, bt)
 */
#define LBSET(x, y)  ( *((char *)(x)) = y)

/*
 * return a single word pointed at by long ptr
 */
#define LWGET(x) ( (int16_t) *((int16_t *)(x)) )

/*
 * set a single word pointed at by long ptr, LWSET(lp, bt)
 */
#define LWSET(x, y)  ( *((int16_t *)(x)) = y)

/*
 * return a single long pointed at by long ptr
 */
#define LLGET(x) ( *((int32_t *)(x)))

/*
 * set a single long pointed at by long ptr, LLSET(lp, bt)
 */
#define LLSET(x, y) ( *((int32_t *)(x)) = y)

/*
 * return 0th byte of a long value given a short pointer to the long value
 */
#define LBYTE0(x) ( *((x)+3) )

/*
 * return 1st byte of a long value given a short pointer to the long value
 */
#define LBYTE1(x) ( *((x)+2) )

/*
 * return 2nd byte of a long value given a short pointer to the long value
 */
#define LBYTE2(x) ( *((x)+1) )

/*
 * return 3rd byte of a long value given a short pointer to the long value
 */
#define LBYTE3(x) (*(x))


/*
 * large.s
 */

/* return length of string pointed a by long pointer */
int16_t	LSTRLEN PROTO ((const char *));
/* copy n words from src long ptr to dst long ptr */
int16_t	LWCOPY PROTO((VOIDPTR dst, const VOIDPTR src, int cnt));
/* copy n bytes from src long ptr to dst long ptr */
VOID LBCOPY PROTO((VOIDPTR dst, const VOIDPTR src, int cnt));
int16_t	LBWMOV PROTO((VOIDPTR dst, const VOIDPTR src));
int16_t	LSTCPY PROTO((char *dst, const char *src));


/*
 * dosif.s
 */
VOID cli PROTO((NOTHING));
VOID sti PROTO((NOTHING));
VOID takecpm PROTO((NOTHING));
VOID givecpm PROTO((NOTHING));


/*
 * geminit.c
 */
extern int16_t do_once;
extern EVB evx;
extern intptr_t gl_vdo;
extern intptr_t ad_sysglo;
extern intptr_t ad_armice;
extern intptr_t ad_hgmice;
extern intptr_t ad_stdesk;
extern intptr_t ad_fsel;
extern intptr_t drawstk;
extern int16_t er_num;						/* for output.s */
extern int16_t no_aes;						/* gembind.s    */
extern int16_t sh_up;						/* is the sh_start being ran yet ? */
extern int16_t autoexec;					/* autoexec a file ?    */
extern char g_autoboot[128];
extern int16_t g_flag;
extern int16_t ctldown;						/* ctrl key down ?  */
/* 8/1/92 */
extern uint16_t act3dtxt;					/* look of 3D activator text */
extern uint16_t act3dface;					/* selected look of 3D activator */
extern uint16_t ind3dtxt;					/* look of 3D indicator text */
extern uint16_t ind3dface;					/* selected look of 3D indicators */
extern uint16_t gl_indbutcol;				/* indicator button color */
extern uint16_t gl_actbutcol;				/* activator button color */
extern uint16_t gl_alrtcol;					/* alert background color */
extern int16_t crt_error;					/* critical error handler semaphore     */

/* set in jbind.s, checked by dispatcher    */

extern int16_t adeskp[3];					/* desktop colors & backgrounds */
extern int16_t awinp[3];					/* window colors & backgrounds */
#if DOWARNING
extern BOOLEAN dowarn;
#endif

/*
 * Should really be OBJECT *.
 * This definition is an old relict from the x86 version.
 */
typedef intptr_t OBJPTR;

VOID setres PROTO((NOTHING));
VOID main PROTO((NOTHING));
VOID pinit PROTO((PD *ppd, CDA *pcda));
int32_t set_cache PROTO((int32_t newcacr));
int16_t pred_dinf PROTO((NOTHING));
int16_t gsx_malloc PROTO((NOTHING));
VOID set_defdrv PROTO((NOTHING));
int16_t gsx_xmfset PROTO((MFORM *pmfnew));
int16_t gsx_mfset PROTO((MFORM *pmfnew));
VOID gr_mouse PROTO((int16_t mkind, int16_t grmaddr));


/*
 * optimize.s/function.c
 */
const char *scasb PROTO((const char *p, char b));
VOID r_get PROTO((int16_t *pxywh, int16_t *px, int16_t *py, int16_t *pw, int16_t *ph));
VOID r_set PROTO((int16_t *pxywh, int16_t x, int16_t y, int16_t w, int16_t h));
VOID rc_copy PROTO((int16_t *psxywh, int16_t *pdxywh));
uint16_t inside PROTO((int16_t x, int16_t y, const GRECT *pt));
int16_t rc_equal PROTO((const int16_t *p1, const int16_t *p2));
int16_t rc_intersect PROTO((const GRECT *p1, GRECT *p2));
VOID rc_union PROTO((const GRECT *p1, GRECT *p2));
VOID rc_constrain PROTO((const GRECT *pc, GRECT *pt));
VOID movs PROTO((int16_t num, const char *ps, char *pd));
int16_t min PROTO((int16_t a, int16_t b));
int16_t max PROTO((int16_t a, int16_t b));
VOID bfill PROTO((int16_t num, char bval, char *addr));
int toupper PROTO((int ch));
size_t strlen PROTO((const char *p1));
int strcmp PROTO((const char *p1, const char *p2));
char *xstrpcpy PROTO((const char *src, char *dst));
char *xstrpcat PROTO((const char *src, char *dst));
char *strscn PROTO((const char *src, char *dst, char stp));
int16_t strchk PROTO((const char *s, const char *t));
VOID fmt_str PROTO((const char *instr, char *outstr));
VOID unfmt_str PROTO((const char *instr, char *outstr));
VOID fs_sset PROTO((OBJPTR tree, int16_t obj, char *pstr, char **ptext, int16_t *ptxtlen));
VOID inf_sset PROTO((OBJPTR tree, int16_t obj, char *pstr));
VOID fs_sget PROTO((OBJPTR tree, int16_t obj, intptr_t pstr));
VOID inf_sget PROTO((OBJPTR tree, int16_t obj, char *pstr));
VOID inf_fldset PROTO((OBJPTR tree, int16_t obj, uint16_t testfld, uint16_t testbit, uint16_t truestate, uint16_t falsestate));
int16_t inf_gindex PROTO((OBJPTR tree, int16_t baseobj, int16_t numobj));
int16_t inf_what PROTO((OBJPTR tree, int16_t ok, int16_t cncl));
int16_t merge_str PROTO((char *pdst, char *ptmp, uint16_t *parms));
int16_t wildcmp PROTO((const char *pwild, const char *ptest));


/*
 * gemaints.c
 */
VOID signal PROTO((EVB *e));
VOID zombie PROTO((EVB *e));

/*
 * gemaplib.c
 */
extern int16_t gl_bvdisk;
extern int16_t gl_bvhard;
extern int16_t gl_recd;
extern int16_t gl_rlen;
extern intptr_t gl_rbuf;
extern int16_t gl_play;					/* 3/11/86  */
extern intptr_t gl_store;				/* 3/11/86  */
extern int16_t gl_mx;					/* 3/12/86  */
extern int16_t gl_my;					/* 3/12/86  */

int16_t ap_init PROTO((intptr_t pglobal));
int16_t ap_exit PROTO((NOTHING));
int16_t rd_mymsg PROTO((VOIDPTR buffer));
int16_t ap_rdwr PROTO((int16_t code, int16_t id, int16_t length, intptr_t pbuff));
int16_t ap_find PROTO((intptr_t pname));
VOID ap_tplay PROTO((intptr_t pbuff, int16_t length, int16_t scale));
int16_t ap_trecd PROTO((intptr_t pbuff, int16_t length));


/*
 * gemasync.c
 */
extern int16_t tbutton;
extern int16_t wwait;

EVB *get_evb PROTO((NOTHING));
VOID evinsert PROTO((EVB *e, EVB **root));
VOID takeoff PROTO((EVB *p));
EVSPEC mwait PROTO((EVSPEC mask));
EVSPEC iasync PROTO((int16_t afunc, intptr_t aparm));
uint16_t aret PROTO((EVSPEC mask));
EVSPEC acancel PROTO((EVSPEC m));


/*
 * gembase.c
 */
extern PD *rlr;
extern PD *drl;
extern PD *nrl;
extern EVB *eul;
extern EVB *dlr;
extern EVB *zlr;
extern int32_t elinkoff;
extern char indisp;
extern char infork;
extern int16_t fpt;
extern int16_t fph;
extern int16_t fpcnt;									/* forkq tail, head,    */
extern SPB wind_spb;
extern CDA *cda;
extern int16_t curpid;


/*
 * gembind.c
 */
VOID xif PROTO((intptr_t pcrys_blk));


/*
 * gemcli.c
 */
extern ACCPD *gl_pacc[MAX_ACCS];		/* total of 6 desk acc, 1 from rom  */
extern int16_t gl_naccs;
extern char *gl_adacc[MAX_ACCS];		/* addresses of accessories */
extern char *sys_adacc;
extern const char stacc[];

VOID release PROTO((NOTHING));
VOID all_run PROTO((NOTHING));
int16_t sndcli PROTO((char *pfilespec, int16_t acc));
VOID ldaccs PROTO((NOTHING));
VOID free_accs PROTO((NOTHING));
int16_t cre_aproc PROTO((NOTHING));

/*
 * gemctrl.c
 */
extern int16_t tmpmoff;
extern int16_t tmpmon;
extern MOBLK gl_ctwait;
extern int16_t appl_msg[8];
extern int16_t deskwind;							/* added 7/25/91 window handle of DESKTOP   */
extern int16_t rets[6];							/* added 2/4/87     */

VOID ct_msgup PROTO((int16_t message, int16_t owner, int16_t wh, int16_t m1, int16_t m2, int16_t m3, int16_t m4));
VOID hctl_window PROTO((int16_t w_handle, int16_t mx, int16_t my));
int16_t hctl_button PROTO((int16_t mx, int16_t my));
int16_t hctl_rect PROTO((int16_t mx, int16_t my));
VOID hctl_msg PROTO((int16_t *msgbuf));
VOID drawdesk PROTO((int16_t x, int16_t y, int16_t w, int16_t h));
VOID ct_chgown PROTO((PD *ppd, GRECT *pr));
int16_t ctlmgr PROTO((NOTHING));
PD *ictlmgr PROTO((int16_t pid));
int16_t ctlmouse PROTO((int16_t mon));
VOID take_ownership PROTO((int16_t beg_ownit));


/*
 * gemdisp.c
 */
extern PD *dpd;								/* critical error process   */
extern PD *slr;

VOID forkq PROTO((FCODE f_code, int32_t f_data));
VOID disp_act PROTO((PD *p));
VOID suspend_act PROTO((PD *p));
VOID forker PROTO((NOTHING));
VOID chkkbd PROTO((NOTHING));
VOID disp PROTO((NOTHING));

/*
 * gemevlib.c
 */
extern int16_t gl_dclick;
extern int16_t gl_ticktime;

VOID ev_rets PROTO((int16_t *rets));
int16_t ev_block PROTO((int16_t code, int32_t lvalue));
uint16_t ev_keybd PROTO((NOTHING));
uint16_t ev_button PROTO((int16_t bflgclks, uint16_t bmask, uint16_t bstate, int16_t *rets));
uint16_t ev_mouse PROTO((MOBLK *pmo, int16_t *rets));
int16_t ev_mesag PROTO((intptr_t pbuff));
int16_t ev_timer PROTO((int32_t count));
int16_t ev_mchk PROTO((MOBLK *pmo));
int16_t ev_multi PROTO((int16_t flags, MOBLK *pmo1, MOBLK *pmo2, int32_t tmcount, intptr_t buparm, intptr_t mebuff, int16_t *prets));
int16_t ev_dclick PROTO((int16_t rate, int16_t setit));


/*
 * gemflag.c
 */
VOID tchange PROTO((int16_t p1, int16_t p2));
int16_t tak_flag PROTO((SPB *sy));
VOID amutex PROTO((EVB *e, SPB *sy));
VOID unsync PROTO((SPB *sy));
int16_t fm_strbrk PROTO((OBJPTR tree, intptr_t palstr, int16_t stroff, int16_t *pcurr_id, int16_t *pnitem, int16_t *pmaxlen));
VOID fm_parse PROTO((OBJPTR tree, intptr_t palstr, int16_t *picnum, int16_t *pnummsg, int16_t *plenmsg, int16_t *pnumbut, int16_t *plenbut));
VOID fm_build PROTO((OBJPTR tree, int16_t haveicon, int16_t nummsg, int16_t mlenmsg, int16_t numbut, int16_t mlenbut));
int16_t fm_alert PROTO((int16_t defbut, intptr_t palstr));


/*
 * gemfmlib.c
 */
int16_t find_obj PROTO((OBJPTR tree, int16_t start_obj, int16_t which));
int16_t fm_keybd PROTO((OBJPTR tree, int16_t obj, int16_t *pchar, int16_t *pnew_obj));
int16_t fm_button PROTO((OBJPTR tree, int16_t new_obj, int16_t clks, int16_t *pnew_obj));
int16_t fm_do PROTO((OBJPTR tree, int16_t start_fld));
VOID fm_dial PROTO((int16_t fmd_type, GRECT *pi, GRECT *pt));
int16_t fm_show PROTO((int16_t string, int16_t *pwd, int16_t level));
int16_t eralert PROTO((int16_t n, int16_t d));
int16_t fm_error PROTO((int16_t n));


/*
 * gemfslib.c
 */
int16_t fs_input PROTO((char *pipath, intptr_t pisel, int16_t *pbutton, char *lstring));
VOID ini_fsel PROTO((NOTHING));


/*
 * gemglobe.c
 */
extern THEGLO D;
int16_t size_theglo PROTO((NOTHING));


/*
 * geminput.c
 */

extern int16_t button;
extern int16_t xrat;
extern int16_t yrat;
extern int16_t kstate;
extern int16_t mclick;
extern int16_t mtrans;

extern int16_t pr_button;
extern int16_t pr_xrat;
extern int16_t pr_yrat;
extern int16_t pr_mclick;

extern PD *gl_mowner;
extern PD *gl_kowner;
extern PD *gl_cowner;
extern PD *ctl_pd;
extern GRECT ctrl;
extern int16_t gl_bclick;
extern int16_t gl_bpend;
extern int16_t gl_button;
extern int16_t gl_bdesired;
extern int16_t gl_btrue;
extern int16_t gl_bdelay;

PD *mowner PROTO((int16_t new));
#if UNLINKED
int16_t chk_ctrl PROTO((int16_t mx, int16_t my));
VOID b_click PROTO((int16_t state));
VOID b_delay PROTO((int16_t amnt));
VOID set_ctrl PROTO((GRECT *pt));
VOID get_ctrl PROTO((GRECT *pt));
VOID get_mkown PROTO((PD **pmown, PD **pkown));
VOID set_mkown PROTO((PD *mp, PD *kp));
VOID nq PROTO((uint16_t ch, CQUEUE *qptr));
VOID akbin PROTO((EVB *e, intptr_t addr));
VOID adelay PROTO((EVB *e, int32_t c));
int16_t abutton PROTO((EVB *e, int32_t p));
VOID amouse PROTO((EVB *e, int32_t pmo));
#endif
uint16_t dq PROTO((CQUEUE *qptr));
VOID fq PROTO((NOTHING));
VOID evremove PROTO((EVB *e, uint16_t ret));
VOID kchange PROTO((int16_t ch, int16_t kstat));
VOID post_keybd PROTO((PD *p, uint16_t ch));
VOID bchange PROTO((int16_t new, int16_t clicks));
int16_t downorup PROTO((int16_t new, intptr_t buparm));
VOID mchange PROTO((int16_t rx1, int16_t ry1));
VOID post_mouse PROTO((PD *p, int16_t grx, int16_t gry));
int16_t inorout PROTO((EVB *e, int16_t rx, int16_t ry));


/*
 * gemmnlib.c
 */
extern OBJPTR gl_mntree;
/* BUG: accessed as short, but allocated as long */
#if !BINEXACT
extern int16_t gl_mnpid;
#endif
extern GRECT gl_rmnactv;
extern intptr_t desk_acc[MAX_ACCS];
extern int16_t desk_pid[MAX_ACCS];
extern int16_t gl_dacnt;
extern int16_t gl_dabase;
extern int16_t gl_dabox;

uint16_t do_chg PROTO((OBJPTR tree, int16_t iitem, uint16_t chgvalue, int16_t dochg, int16_t dodraw, int16_t chkdisabled));
int16_t menu_set PROTO((OBJPTR tree, int16_t last_item, int16_t cur_item, int16_t setit));
VOID menu_sr PROTO((int16_t saveit, OBJPTR tree, int16_t imenu));
int16_t menu_down PROTO((OBJPTR tree, int16_t ititle));
int16_t mn_do PROTO((int16_t *ptitle, int16_t *pitem));
int16_t mn_bar PROTO((OBJPTR tree, int16_t showit));
int16_t mn_bar PROTO((OBJPTR tree, int16_t showit));
VOID mn_clsda PROTO((NOTHING));
int16_t mn_register PROTO((int16_t pid, char *pstr));
VOID ch_wrect PROTO((GRECT *r, GRECT *n));


/*
 * gemobed.c
 */
int16_t ob_getsp PROTO((OBJPTR tree, int16_t obj, TEDINFO *pted));
VOID ob_center PROTO((OBJPTR tree, GRECT *pt));
int16_t scan_to_end PROTO((char *pstr, int16_t idx, char chr));
VOID ins_char PROTO((char *str, int16_t pos, char chr, int16_t tot_len));
int16_t find_pos PROTO((char *str, int16_t pos));
VOID pxl_rect PROTO((OBJPTR tree, int16_t obj, int16_t ch_pos, GRECT *pt));
VOID curfld PROTO((OBJPTR tree, int16_t obj, int16_t new_pos, int16_t dist));
int16_t instr PROTO((char chr, const char *str));
int16_t check PROTO((const char *in_char, char valchar));
VOID ob_stfn PROTO((int16_t idx, int16_t *pstart, int16_t *pfinish));
int16_t ob_delit PROTO((int16_t idx));
int16_t ob_edit PROTO((OBJPTR tree, int16_t obj, int16_t in_char, int16_t *idx, int16_t kind));

/*
 * gemobjop.c
 */
char ob_sst PROTO((OBJPTR tree, int16_t obj, intptr_t *pspec, int16_t *pstate, int16_t *ptype, int16_t *pflags, GRECT *pt, int16_t *pth));
typedef VOID (*EVERYOBJ_CALLBACK) PROTO((OBJPTR tree, int16_t obj, int16_t sx, int16_t sy));
VOID everyobj PROTO((OBJPTR tree, int16_t this, int16_t last, EVERYOBJ_CALLBACK routine, int16_t startx, int16_t starty, int16_t maxdep));
int16_t get_par PROTO((OBJPTR tree, int16_t obj));


/*
 * gemoblib.c
 */
extern TEDINFO edblk;
extern BITBLK bi;
extern ICONBLK ib;

int16_t ob_sysvar PROTO((uint16_t mode, uint16_t which, uint16_t inval1, uint16_t inval2, uint16_t *outval1, uint16_t outval2));
VOID ob_format PROTO((int16_t just, char *raw_str, char *tmpl_str, char *fmt_str));
int16_t ob_user PROTO((OBJPTR tree, int16_t obj, GRECT *pt, intptr_t userblk, int16_t curr_state, int16_t new_state));
VOID draw_hi PROTO((GRECT *prect, int16_t state, int16_t clip, int16_t th, int16_t icol));
VOID ob_draw PROTO((OBJPTR tree, int16_t obj, int16_t depth));
int16_t ob_find PROTO((OBJPTR tree, int16_t currobj, int16_t depth, int16_t mx, int16_t my));
VOID ob_add PROTO((OBJPTR tree, int16_t parent, int16_t child));
VOID ob_delete PROTO((OBJPTR tree, int16_t obj));
VOID ob_order PROTO((OBJPTR tree, int16_t mov_obj, int16_t new_pos));
VOID ob_change PROTO((OBJPTR tree, int16_t obj, int16_t new_state, int16_t redraw));
uint16_t ob_fs PROTO((OBJPTR tree, int16_t ob, int16_t pflag));
VOID ob_actxywh PROTO((OBJPTR tree, int16_t obj, GRECT *pt));
VOID ob_relxywh PROTO((OBJPTR tree, int16_t obj, GRECT *pt));
VOID ob_setxywh PROTO((OBJPTR tree, int16_t obj, GRECT *pt));
VOID ob_offset PROTO((OBJPTR tree, int16_t obj, int16_t *pxoff, int16_t *pyoff));
VOID ob_dxywh PROTO((OBJPTR tree, int16_t obj, int16_t *pdx, int16_t *pdy, int16_t *pdw, int16_t *pdh));
VOID ob_gclip PROTO((OBJPTR tree, int16_t obj, int16_t *pxoff, int16_t *pyoff, int16_t *pxcl, int16_t *pycl, int16_t *pwcl, int16_t *phcl));
int16_t get_prev PROTO((OBJPTR tree, int16_t parent, int16_t obj));
CICON *match_planes PROTO((CICON *iconlist, int planes));
CICON *find_eq_or_less PROTO((CICON *iconlist, int planes));
VOID gr_cicon PROTO((int16_t state, int16_t *pmask, int16_t *pdata, const char *ptext, int16_t ch, int16_t chx, int16_t chy, GRECT *pi, GRECT *pt, CICONBLK *cicon));
int16_t gsx_cblt PROTO((P(int16_t *) saddr, P(uint16_t) sx, P(uint16_t) sy, P(uint16_t) swb, P(int16_t *) daddr, P(uint16_t) dx, P(uint16_t) dy, P(uint16_t) dwb, P(uint16_t) w, P(uint16_t) h, P(uint16_t) rule, P(int16_t) numplanes));
VOID get_color_rsc PROTO((CICONBLK **cicondata));


/*
 * apgrlib.[cS]
 */
VOID gr_inside PROTO((GRECT *pt, int16_t th));
VOID gr_rect PROTO((uint16_t icolor, uint16_t ipattern, GRECT *pt));
int16_t gr_just PROTO((int16_t just, int16_t font, const char *ptext, int16_t w, int16_t h, GRECT *pt));
VOID gr_gtext PROTO((int16_t just, int16_t font, const char *ptext, GRECT *pt, int16_t tmode));
VOID gr_crack PROTO((uint16_t color, int16_t *pbc, int16_t *ptc, int16_t *pip, int16_t *pic, int16_t *pmd));
VOID gr_gicon PROTO((int16_t state, int16_t *pmask, int16_t *pdata, const char *ptext, int16_t ch, int16_t chx, int16_t chy, GRECT *pi, GRECT *pt));
VOID gr_box PROTO((int16_t x, int16_t y, int16_t w, int16_t h, int16_t th));


/*
 * gemgrlib.[cS]
 */
VOID gr_stepcalc PROTO((int16_t orgw, int16_t orgh, GRECT *pt, int16_t *pcx, int16_t *pcy, int16_t *pcnt, int16_t *pxstep, int16_t *pystep));
VOID gr_growbox PROTO((GRECT *po, GRECT *pt));
VOID gr_shrinkbox PROTO((GRECT *po, GRECT *pt));
VOID gr_xor PROTO((int16_t clipped, int16_t cnt, int16_t cx, int16_t cy, int16_t cw, int16_t ch, int16_t xstep, int16_t ystep, int16_t dowdht));
VOID gr_movebox PROTO((int16_t w, int16_t h, int16_t srcx, int16_t srcy, int16_t dstx, int16_t dsty));
VOID gr_scale PROTO((int16_t xdist, int16_t ydist, int16_t *pcnt, int16_t *pxstep, int16_t *pystep));
int16_t gr_watchbox PROTO((OBJECT *tree, int16_t obj, int16_t instate, int16_t outstate));
int16_t gr_stilldn PROTO((int16_t out, int16_t x, int16_t y, int16_t w, int16_t h));
VOID gr_draw PROTO((int16_t have2box, GRECT *po, GRECT *poff));
int16_t gr_wait PROTO((GRECT *po, GRECT *poff, int16_t mx, int16_t my));
VOID gr_setup PROTO((int16_t color));
VOID gr_rubbox PROTO((int16_t xorigin, int16_t yorigin, int16_t wmin, int16_t hmin, int16_t *pwend, int16_t *phend));
VOID gr_rubwind PROTO((int16_t xorigin, int16_t yorigin, int16_t wmin, int16_t hmin, int16_t *poff, int16_t *pwend, int16_t *phend));
VOID gr_dragbox PROTO((int16_t w, int16_t h, int16_t sx, int16_t sy, int16_t *pc, int16_t *pdx, int16_t *pdy));
VOID gr_clamp PROTO((int16_t xorigin, int16_t yorigin, int16_t wmin, int16_t hmin, int16_t *pneww, int16_t *pnewh));
int16_t gr_slidebox PROTO((OBJPTR tree, int16_t parent, int16_t obj, int16_t isvert));
VOID gr_mkstate PROTO((int16_t *pmx, int16_t *pmy, int16_t *pmstat, int16_t *pkstat));

/*
 * apgsxif.[cS]
 */
#define ORGADDR 0x0L

#define vsf_interior( x )	gsx_1code(S_FILL_STYLE, x)
#define vsl_type( x )		gsx_1code(S_LINE_TYPE, x)
#define vsf_style( x )		gsx_1code(S_FILL_INDEX, x)
#define vsf_color( x )		gsx_1code(S_FILL_COLOR, x)
#define vsl_udsty( x )		gsx_1code(ST_UD_LINE_STYLE, x)

extern int16_t gl_nplanes;					/* number of planes in current res */
extern int16_t gl_width;
extern int16_t gl_height;
extern int16_t gl_nrows;
extern int16_t gl_ncols;
extern int16_t gl_wchar;
extern int16_t gl_hchar;
extern int16_t gl_wschar;
extern int16_t gl_hschar;
extern int16_t gl_wptschar;
extern int16_t gl_hptschar;
extern int16_t gl_wsptschar;
extern int16_t gl_hsptschar;
extern int16_t gl_wbox;
extern int16_t gl_hbox;
extern int16_t gl_xclip;
extern int16_t gl_yclip;
extern int16_t gl_wclip;
extern int16_t gl_hclip;
extern int16_t gl_nplanes;
extern int16_t gl_handle;
extern int16_t gl_mode;
extern int16_t gl_mask;
extern int16_t gl_tcolor;
extern int16_t gl_lcolor;
extern int16_t gl_fis;
extern int16_t gl_patt;
extern int16_t gl_font;
extern GRECT gl_rscreen;
extern GRECT gl_rfull;
extern GRECT gl_rzero;
extern GRECT gl_rcenter;
extern GRECT gl_rmenu;

VOID gsx_sclip PROTO((GRECT *pt));
VOID gsx_gclip PROTO((GRECT *pt));
BOOLEAN gsx_chkclip PROTO((GRECT *pt));
VOID gsx_pline PROTO((int16_t offx, int16_t offy, int16_t cnt, int16_t pts));
VOID gsx_cline PROTO((uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2));
VOID gsx_attr PROTO((uint16_t text, uint16_t mode, uint16_t color));
VOID gsx_bxpts PROTO((GRECT *pt));
VOID gsx_box PROTO((GRECT *pt));
VOID bb_screen PROTO((int16_t scrule, int16_t scsx, int16_t scsy, int16_t scdx, int16_t scdy, int16_t scw, int16_t sch));
VOID gsx_trans PROTO((int16_t *saddr, uint16_t swb, int16_t *daddr, uint16_t dwb, uint16_t h, int16_t fg, int16_t bg));
VOID gsx_start PROTO((NOTHING));
VOID bb_fill PROTO((int16_t mode, int16_t fis, int16_t patt, int16_t hx, int16_t hy, int16_t hw, int16_t hh));
int16_t gsx_tcalc PROTO((int16_t font, const char *ptext, int16_t ptextw, int16_t ptexth, int16_t pnumchs));
int16_t gsx_tblt PROTO((int16_t tb_f, int16_t x, int16_t y, int16_t tb_nc));
VOID gsx_xbox PROTO((GRECT *pt));
VOID gsx_xcbox PROTO((GRECT *pt));
VOID gsx_xline PROTO((int16_t ptscount, int16_t ppoints));
uint16_t ch_width PROTO((int16_t fn));
uint16_t ch_height PROTO((int16_t fn));


/*
 * optimize.S
 */
unsigned int reverse PROTO((int index));
VOID expand_data PROTO((int16_t *saddr, int16_t *daddr, int16_t *mask, int splanes, int dplanes, int w, int h));


/*
 * gempd.c
 */
PD *fpdnm PROTO((const char *pname, uint16_t pid));
VOID p_nameit PROTO((PD *p, const char *pname));
PD *pstart PROTO((VOIDPTR pcode, const char *pfilespec, intptr_t ldaddr));
VOID insert_process PROTO((PD *pi, PD **root));


/*
 * gemqueue.c
 */
VOID aqueue PROTO((BOOLEAN isqwrite, EVB *e, int32_t lm));


/*
 * gemrlist.c
 */
RLIST *newrect PROTO((NOTHING));
BOOLEAN delrect PROTO((RLIST *rp, VOIDPTR rlist));
RLIST *genrlist PROTO((uint16_t handle, uint16_t area));
RLIST *mkrect PROTO((uint16_t pc, GRECT *trect, GRECT *brect));


/*
 * gemrom.c
 */
extern RSHDR *gemptr;					/* GEM's rsc pointer, in RAM */
extern RSHDR *deskptr;					/* DESKTOP'S rsc pointer, in RAM */
extern char *infptr;
extern uint16_t infsize;
extern uint16_t gemsize;
extern uint16_t desksize;
extern VOIDPTR gl_pglue;
extern BOOLEAN nodesk;
extern BOOLEAN nogem;
extern int16_t st_lang;					/* Language code    */
extern uint16_t st_time;				/* time code        */
extern uint16_t st_date;
extern uint16_t st_dchar;
extern int16_t st_keybd;

int16_t rom_ram PROTO((int which, intptr_t pointer));
VOID rsc_free PROTO((NOTHING));
VOID rsc_read PROTO((NOTHING));


/*
 * gemrslib.c
 */
extern RSHDR *rs_hdr;
extern intptr_t rs_global;

VOID rs_obfix PROTO((OBJPTR tree, P(int16_t) curob));
char *rs_str PROTO((int16_t stnum));
int16_t rs_free PROTO((intptr_t pglobal));
int16_t rs_gaddr PROTO((intptr_t pglobal, uint16_t rtype, uint16_t rindex, VOIDPTR *rsaddr));
VOID rs_saddr PROTO((intptr_t pglobal, uint16_t rtype, uint16_t rindex, intptr_t rsaddr));
VOID do_rsfix PROTO((intptr_t hdr, int16_t size));
VOID rs_fixit PROTO((intptr_t pglobal));
int16_t rs_load PROTO((intptr_t pglobal, const char *rsfname));


/*
 * gemsclib.c
 */
int16_t sc_read PROTO((char *pscrap));
int16_t sc_write PROTO((const char *pscrap));



/*
 * jdos.S
 */
extern int16_t DOS_ERR;
extern int16_t DOS_AX;

int dos_open PROTO((const char *name, int mode));
long dos_lseek PROTO((int fd, int whence, long offset));
int dos_read PROTO((int fd, long size, VOIDPTR buf));
int dos_close PROTO((int fd));
VOIDPTR dos_alloc PROTO((long size));
int dos_free PROTO((VOIDPTR ptr));
