/*
*************************************************************************
*			Revision Control System
* =======================================================================
*  $Revision: 2.2 $	$Source: /u2/MRS/osrevisions/aes/gsxdefs.h,v $
* =======================================================================
*  $Author: mui $	$Date: 89/04/26 18:27:54 $	$Locker: kbad $
* =======================================================================
*  $Log:	gsxdefs.h,v $
* Revision 2.2  89/04/26  18:27:54  mui
* TT
* 
* Revision 2.1  89/02/22  05:30:18  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  88/06/02  12:35:29  lozben
* Initial revision
* 
*************************************************************************
*/
/*	GSXDEFS.H	05/06/84 - 12/08/84	Lee Lorenzen		*/

#ifndef GSXDEFS_H
#define GSXDEFS_H 1

typedef struct wsstr
{
	int16_t		ws_xres;
	int16_t		ws_yres;
	int16_t		ws_noscale;
	int16_t		ws_wpixel;
	int16_t		ws_hpixel;
	int16_t		ws_ncheights;
	int16_t		ws_nlntypes;
	int16_t		ws_nlnwidths;
	int16_t		ws_nmktypes;
	int16_t		ws_nmksizes;
	int16_t		ws_nfaces;
	int16_t		ws_npatts;
	int16_t		ws_nhatchs;
	int16_t		ws_ncolors;
	int16_t		ws_ngdps;
	int16_t		ws_supgdps[10];
	int16_t		ws_attgdps[10];
	int16_t		ws_color;
	int16_t		ws_rotate;
	int16_t		ws_fill;
	int16_t		ws_cell;
	int16_t		ws_npals;
	int16_t		ws_nloc;
	int16_t		ws_nval;
	int16_t		ws_nchoice;
	int16_t		ws_nstring;
	int16_t		ws_type;
	int16_t		ws_pts0;
	int16_t		ws_chminh;
	int16_t		ws_pts2;
	int16_t		ws_chmaxh;
	int16_t		ws_lnminw;
	int16_t		ws_pts5;
	int16_t		ws_lnmaxw;
	int16_t		ws_pts7;
	int16_t		ws_pts8;
	int16_t		ws_mkminw;
	int16_t		ws_pts10;
	int16_t		ws_mkmaxw;
} WS;

typedef struct fdbstr
{
	VOIDPTR     fd_addr;
	int16_t		fd_w;
	int16_t		fd_h;
	int16_t		fd_wdwidth;
	int16_t		fd_stand;
	int16_t		fd_nplanes;
	int16_t		fd_r1;
	int16_t		fd_r2;
	int16_t		fd_r3;
} FDB;


typedef struct mfstr
{
	int16_t	mf_xhot;
	int16_t	mf_yhot;
	int16_t	mf_nplanes;
	int16_t	mf_fg;
	int16_t	mf_bg;
	int16_t	mf_mask[16];
	int16_t	mf_data[16];
} MFORM;

extern FDB gl_src;
extern FDB gl_dst;
extern FDB gl_tmp;

extern WS gl_ws;

/*
 * VDI arrays, not ours..
 */
extern int16_t contrl[];
extern int16_t intin[];
extern int16_t intout[];
extern int16_t ptsin[];
extern int16_t ptsout[10];
extern intptr_t ad_intin;



/*
 * gemgsxif.[cS]
 */
/* counting semaphore  == 0 implies ON  >  0 implies OFF    */
extern int gl_moff;
extern size_t gl_mlen;
extern BOOLEAN gl_graphic;
extern BOOLEAN gl_mouse;					/* mouse on flag        */
extern intptr_t tikaddr;
extern intptr_t tiksav;

VOID gsx_mfree PROTO((NOTHING));
VOID gsx_moff PROTO((NOTHING));
VOID gsx_mon PROTO((NOTHING));


int16_t gsx_chkclip PROTO((GRECT *c));
VOID gsx_1code PROTO((int16_t code, int16_t value));
VOID gsx_ncode PROTO((int16_t code, int16_t n, int16_t m));
VOID gsx_init PROTO((NOTHNG));
VOID gsx_graphic PROTO((BOOLEAN tographic));
VOID gsx_escapes PROTO((int16_t esc_code));
VOID v_hardcopy PROTO((NOTHING));
VOID gsx_wsopen PROTO((NOTHING));
VOID gsx_wsclose PROTO((NOTHING))
VOID ratinit PROTO((NOTHING));
VOID ratexit PROTO((NOTHING));
VOID bb_set PROTO((int16_t sx, int16_t sy, int16_t sw, int16_t sh, int16_t *pts1, int16_t *pts2, FDB *pfd, FDB *psrc, FDB *pdst));
VOID bb_save PROTO((GRECT *ps));
VOID gsx_resetmb PROTO((NOTHING));
int16_t gsx_tick PROTO((intptr_t tcode, intptr_t *ptsave));
VOID gsx_mxmy PROTO((int16_t *pmx, int16_t *pmy));
int16_t gsx_button PROTO((NOTHING));
VOID gsx_moff PROTO((NOTHING));
VOID gsx_mon PROTO((NOTHING));

int16_t v_opnwk PROTO((int16_t *pwork_in, int16_t *phandle, int16_t *pwork_out));
VOID v_pline PROTO((int16_t count, int16_t *pxyarray));
VOID vst_clip PROTO((int16_t clip_flag, int16_t pxyarray));
VOID vst_height PROTO((int16_t height, int16_t *pchr_width, int16_t *pchr_height, int16_t *pcell_width, int16_t *pcell_height));
VOID vr_recfl PROTO((int16_t *pxyarray, FDB *pdesMFDB)); /* ??? why MFDB ??? */
VOID vro_cpyfm PROTO((int16_t wr_mode, int16_t *pxyarray, FDB *psrcMFDB, FDB *pdesMFDB));
VOID vrt_cpyfm PROTO((int16_t wr_mode, int16_t *pxyarray, FDB *psrcMFDB, FDB *pdesMFDB, int16_t fgcolor, int16_t bgcolor));
VOID vrn_trnfm PROTO((FDB *psrcMFDB, FDB *pdesMFDB));
VOID vsl_width PROTO((int16_t width));

/*
 * apgsxif.[cS]
 */
VOID gsx_fix PROTO((FDB *pfd, int16_t *theaddr, int16_t wb, int16_t h));

#endif /* GSXDEFS_H */
