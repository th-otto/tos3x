/*
 *************************************************************************
 *			Revision Control System
 * =======================================================================
 *  $Author: mui $	$Date: 89/04/26 18:27:54 $
 * =======================================================================
 *
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

#define	OPEN_WORKSTATION	1
#define	CLOSE_WORKSTATION	2
#define	CLEAR_WORKSTATION	3
#define	UPDATE_WORKSTATION	4

#define	ESCAPE			5


#define	POLYLINE		6
#define	POLYMARKER		7
#define	TEXT			8
#define	FILLED_AREA		9
#define	CELL_ARRAY		10

#define	GDP			11

#define	CHAR_HEIGHT		12
#define	CHAR_UP_VECT		13
#define	SET_COLOR_REP		14
#define	S_LINE_TYPE		15
#define	S_LINE_WIDTH		16
#define	S_LINE_COLOR		17
#define	S_MARK_TYPE		18
#define	S_MARK_SCALE		19
#define	S_MARK_COLOR		20
#define	SET_FONT		21
#define	S_TEXT_COLOR		22
#define	S_FILL_STYLE		23
#define	S_FILL_INDEX		24
#define	S_FILL_COLOR		25
#define	INQUIRE_COLOR_REP	26
#define	INQ_CELL_ARRAY		27

#define	LOCATOR_INPUT		28
#define	VALUATOR_INPUT		29
#define	CHOICE_INPUT		30	
#define	STRING_INPUT		31
#define	SET_WRITING_MODE	32

#define	SET_INPUT_MODE		33
#define VQT_ATTRIBUTES      38

#define	OPEN_VWORKSTATION	100
#define	CLOSE_VWORKSTATION	101
#define	EXTENDED_INQUIRE	102
#define	CONTOUR_FILL		103

#define	ST_FILLPERIMETER	104
#define	ST_TXT_DISPLAY_MODE	105
#define	ST_TXT_STYLE		106
#define	ST_CH_HEIGHT		107
#define	RET_PIXEL_VALUE		108

#define	COPY_RASTER_FORM	109
#define	TRANSFORM_FORM		110
#define	ST_CUR_FORM		111
#define	ST_UD_FILL_PATTERN	112
#define ST_UD_LINE_STYLE	113
#define	FILL_RECTANGLE		114

#define	LOC_MULTIWAIT		115
#define	I_TEXT_EXTENT		116
#define	I_TEXT_WIDTH		117
#define	TIM_VECX		118

#define	COPY_ALPHA_TEXT		119
#define	ALTER_ALPHA_TEXT	120
#define	TRAN_RASTER_FORM	121

#define	SHOW_CUR		122
#define	HIDE_CUR		123
#define	MOUSE_ST		124
#define	BUT_VECX		125
#define	MOT_VECX		126
#define	CUR_VECX		127
#define KEY_SHST		128
#define	TEXT_CLIP		129


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
extern VOIDPTR ad_intin; /* FAR ptr to intin */



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
VOID gsx_mret PROTO((VOIDPTR *pmaddr, int32_t *pmlen));


VOID gsx_1code PROTO((int16_t code, int16_t value));
VOID gsx_ncode PROTO((int16_t code, int16_t n, int16_t m));
VOID gsx_init PROTO((NOTHING));
VOID gsx_graphic PROTO((BOOLEAN tographic));
VOID gsx_escapes PROTO((int16_t esc_code));
VOID v_hardcopy PROTO((NOTHING));
VOID gsx_wsopen PROTO((NOTHING));
VOID gsx_wsclose PROTO((NOTHING));
VOID ratinit PROTO((NOTHING));
VOID ratexit PROTO((NOTHING));
VOID bb_set PROTO((int16_t sx, int16_t sy, int16_t sw, int16_t sh, int16_t *pts1, int16_t *pts2, FDB *pfd, FDB *psrc, FDB *pdst));
VOID bb_save PROTO((GRECT *ps));
VOID bb_restore PROTO((GRECT *ps));
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
int16_t gsx_blt PROTO((int16_t *saddr, uint16_t sx, uint16_t sy, uint16_t swb, int16_t *daddr, uint16_t dx, uint16_t dy, uint16_t dwb, uint16_t w, uint16_t h, uint16_t rule, int16_t fgcolor, int16_t bgcolor));

#endif /* GSXDEFS_H */
