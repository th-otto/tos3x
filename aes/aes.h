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

#include "struct88.h"


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

int16_t	LSTCPY ((char *dst, const char *src));


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
extern int32_t gl_vdo;
extern int32_t ad_sysglo;
extern int32_t ad_armice;
extern int32_t ad_hgmice;
extern int32_t ad_stdesk;
extern int32_t ad_fsel;
extern int32_t drawstk;
extern int16_t er_num;						/* for output.s */
extern int16_t no_aes;						/* gembind.s    */
extern int16_t sh_up;						/* is the sh_start being ran yet ? */
extern int16_t autoexec;					/* autoexec a file ?    */
extern char g_autoboot[128];
extern int16_t g_flag;
extern int16_t ctldown;					/* ctrl key down ?  */
/* 8/1/92 */
extern uint16_t act3dtxt;					/* look of 3D activator text */
extern uint16_t act3dface;					/* selected look of 3D activator */
extern uint16_t ind3dtxt;					/* look of 3D indicator text */
extern uint16_t ind3dface;					/* selected look of 3D indicators */
extern uint16_t gl_indbutcol;				/* indicator button color */
extern uint16_t gl_actbutcol;				/* activator button color */
extern uint16_t gl_alrtcol;				/* alert background color */
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
int16_t gsx_xmfset PROTO((int32_t pmfnew));
int16_t gsx_mfset PROTO((int32_t pmfnew));
VOID gr_mouse PROTO((int16_t mkind, int16_t grmaddr));
int16_t gr_slidebox PROTO((OBJPTR tree, int16_t parent, int16_t obj, int16_t isvert));
int16_t gr_stilldn PROTO((int16_t out, int16_t x, int16_t y, int16_t w, int16_t h));


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
VOID movs PROTO((int16_t num, oonst char *ps, char *pd));
int16_t min PROTO((int16_t a, int16_t b));
int16_t max PROTO((int16_t a, int16_t b));
VOID bfill PROTO((int16_t num, char bval, char *addr));
char toupper PROTO((char ch));
size_t strlen PROTO((const char *p1));
int strcmp PROTO((const char *p1, const char *p2));
char *xstrpcpy PROTO((const char *src, char *dst));
char *xstrpcat PROTO((const char *src, char *dst));
char *strscn PROTO((const char *src, char *dst, char stp));
int16_t strchk PROTO((const char *s, const char *t));
VOID fmt_str PROTO((const char *instr, char *outstr));
VOID unfmt_str PROTO((const char *instr, char *outstr));
VOID fs_sset PROTO((OBJPTR tree, int16_t obj, int16_t pstr, char **ptext, int16_t *ptxtlen));
VOID inf_sset PROTO((OBJPTR tree, int16_tobj, char *pstr));
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
 * gemapib.c
 */
extern int16_t gl_bvdisk;
extern int16_t gl_bvhard;
extern int16_t gl_recd;
extern int16_t gl_rlen;
extern int32_t gl_rbuf;
extern int16_t gl_play;					/* 3/11/86  */
extern int32_t gl_store;				/* 3/11/86  */
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
uint16_t crysbind PROTO((int16_t opcode, intptr_t pglobal, uint16_t *int_in, uint16_t *int_out, VOIDPTR *addr_in));
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
int16_t ev_multi PROTO((int16_tflags, MOBLK *pmo1, MOBLK *pmo2, int32_t tmcount, intptr_t buparm, intptr_t mebuff, int16_t *prets));
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
int16_t fm_button PROTO((OBJPTRtree, int16_t new_obj, int16_t clks, int16_t *pnew_obj));
int16_t fm_do PROTO((OBJPTR tree, int16_t start_fld));
VOID fm_dial PROTO((int16_t fmd_type, GRECT *pi, GRECT *pt));
int16_t fm_show PROTO((int16_t string, int16_t *pwd, int16_t level));
int16_t eralert PROTO((int16_t n, int16_t d));
int16_t fm_error PROTO((int16_t n));
