#include "../common/config.h"

#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __STDINT_H__
#include <stdint.h>
#endif

#ifdef __GNUC__
#define unreachable() __builtin_unreachable()
#else
#define unreachable()
#endif

typedef int16_t BOOLEAN;
#define FALSE 0
#define TRUE 1

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#define VIDEL_SUPPORT (TOSVERSION >= 0x404)


#include	"scrndev.h"

/*
 * init.c
 */
VOID init_st_tt_sp PROTO((NOTHING));
VOID InitST PROTO((NOTHING));
VOID InitTT PROTO((NOTHING));
VOID InitSTSpLut PROTO((NOTHING));
VOID Init16Pal PROTO((NOTHING));
VOID Init32Pal PROTO((NOTHING));
VOID InitColReqArray PROTO((NOTHING));
VOID InitTTLut PROTO((NOTHING));

/*
 * isin.c
 */
int16_t Isin PROTO((int16_t ang));
int16_t Icos PROTO((int16_t ang));

/*
 * jmptbl.c
 */
VOID SCREEN PROTO((NOTHING));

/*
 * monobj.c
 */
VOID vsl_type PROTO((NOTHING));
VOID vsl_width PROTO((NOTHING));
VOID vsl_ends PROTO((NOTHING));
VOID vsl_color PROTO((NOTHING));
VOID vsm_height PROTO((NOTHING));
VOID vsm_type PROTO((NOTHING));
VOID vsm_color PROTO((NOTHING));
VOID vsf_interior PROTO((NOTHING));
VOID vsf_style PROTO((NOTHING));
VOID vsf_color PROTO((NOTHING));
VOID v_locator PROTO((NOTHING));
VOID v_show_c PROTO((NOTHING));
VOID v_hide_c PROTO((NOTHING));
VOID v_valuator PROTO((NOTHING));
VOID v_choice PROTO((NOTHING));
VOID v_string PROTO((NOTHING));
VOID vq_key_s PROTO((NOTHING));
VOID vswr_mode PROTO((NOTHING));
VOID vsin_mode PROTO((NOTHING));
VOID vqi_mode PROTO((NOTHING));
VOID vsf_perimeter PROTO((NOTHING));
VOID vsl_udsty PROTO((NOTHING));
VOID s_clip PROTO((NOTHING));
VOID arb_corner PROTO((int16_t *corners, int16_t type));
VOID dro_cpyfm PROTO((NOTHING));
VOID drt_cpyfm PROTO((NOTHING));
VOID dr_trn_fm PROTO((NOTHING));
VOID dr_recfl PROTO((NOTHING));

/*
 * monout.c
 */
VOID vq_extnd PROTO((NOTHING));
VOID v_clswk PROTO((NOTHING));
VOID v_pline PROTO((NOTHING));
VOID v_pmarker PROTO((NOTHING));
VOID v_fillarea PROTO((NOTHING));
VOID v_gdp PROTO((NOTHING));
VOID vql_attr PROTO((NOTHING));
VOID vqm_attr PROTO((NOTHING));
VOID vqf_attr PROTO((NOTHING));
VOID pline PROTO((NOTHING));
BOOLEAN clip_line PROTO((NOTHING));
int16_t code PROTO((int16_t x,int16_t y));
VOID plygn PROTO((NOTHING));
VOID gdp_rbox PROTO((NOTHING));
VOID gdp_arc PROTO((NOTHING));
VOID clc_nsteps PROTO((NOTHING));
VOID gdp_ell PROTO((NOTHING));
VOID clc_arc PROTO((NOTHING));
VOID Calc_pts PROTO((int16_t j));
VOID st_fl_ptr PROTO((NOTHING));
VOID cir_dda PROTO((NOTHING));
VOID wline PROTO((NOTHING));
VOID perp_off PROTO((int16_t *px,int16_t *py));
VOID quad_xform PROTO((int quad, int x, int y, int16_t *tx, int16_t *ty));
VOID do_circ PROTO((int16_t cx, int16_t cy));
VOID s_fa_attr PROTO((NOTHING));
VOID r_fa_attr PROTO((NOTHING));
VOID do_arrow PROTO((NOTHING));
VOID arrow PROTO((int16_t *xy,int16_t inc));
VOID init_wk PROTO((NOTHING));
VOID d_opnvwk PROTO((NOTHING));
VOID d_clsvwk PROTO((NOTHING));
VOID dsf_udpat PROTO((NOTHING));
VOID vq_color PROTO((NOTHING));
VOID vs_color PROTO((NOTHING));

/*
 * opnwk.c
 */
VOID v_opnwk PROTO((NOTHING));
const SCREENDEF *FindDevice PROTO((int16_t devId));
VOID SetCurDevice PROTO((int16_t curRez));
VOID InitFonts PROTO((NOTHING));
VOID InitDevTabInqTab PROTO((NOTHING));

/*
 * seedfill.c
 */
VOID d_contourfill PROTO((NOTHING));
VOID seedfill PROTO((NOTHING));
VOID crunch_Q PROTO((NOTHING));
VOID v_get_pixel PROTO((NOTHING));

/*
 * spcolor.c
 */
VOID sp_vs_color PROTO((NOTHING));
VOID sp_vq_color PROTO((NOTHING));

/*
 * text.c
 */
VOID d_gtext PROTO((NOTHING));
VOID text_init PROTO((NOTHING));
VOID dst_height PROTO((NOTHING));
VOID copy_name PROTO((const char *source, char *dest));
VOID make_header PROTO((NOTHING));
VOID dst_point PROTO((NOTHING));
VOID dst_style PROTO((NOTHING));
VOID dst_alignment PROTO((NOTHING));
VOID dst_rotation PROTO((NOTHING));
VOID dst_font PROTO((NOTHING));
VOID dst_color PROTO((NOTHING));
VOID dqt_attributes PROTO((NOTHING));
VOID dqt_extent PROTO((NOTHING));
VOID dqt_width PROTO((NOTHING));
VOID dqt_name PROTO((NOTHING));
VOID dqt_fontinfo PROTO((NOTHING));
VOID d_justified PROTO((NOTHING));
VOID dt_loadfont PROTO((NOTHING));
VOID dt_unloadfont PROTO((NOTHING));

/*
 * trucolor.c
 */
VOID vs_32_color PROTO((NOTHING));
VOID vq_32_color PROTO((NOTHING));
VOID vs_16_color PROTO((NOTHING));
VOID vq_16_color PROTO((NOTHING));


/*
 * ttcolor.c
 */
VOID tt_vs_color PROTO((NOTHING));
VOID tt_vq_color PROTO((NOTHING));


/*
 * spcolor.c
 */
VOID sp_vs_color PROTO((NOTHING));
VOID sp_vq_color PROTO((NOTHING));


/*
 * 
 */
int16_t MONO8XHT PROTO((NOTHING));


/*
 * xfrmform.S
 */
VOID TRAN_FM PROTO((NOTHING));           /* 110  */
