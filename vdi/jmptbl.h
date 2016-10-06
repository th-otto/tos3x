/*
********************************  jmptbl.h  ***********************************
*
* $Revision: 3.1 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/jmptbl.h,v $
* =============================================================================
* $Author: lozben $	$Date: 91/01/08 16:27:20 $     $Locker:  $
* =============================================================================
*
* $Log:	jmptbl.h,v $
* Revision 3.1  91/01/08  16:27:20  lozben
* Adjusted some names to correspond to their declarations in an include file.
* 
* Revision 3.0  91/01/03  15:12:07  lozben
* New generation VDI
* 
* Revision 2.1  89/02/21  17:23:23  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
*******************************************************************************
*/

#ifndef _JMPTBL_H_
#define _JMPTBL_H_

VOID v_nop PROTO((NOTHING));             /*  VDI #0   */

VOID v_opnwk PROTO((NOTHING));           /*  VDI #1   */
VOID v_clswk PROTO((NOTHING));           /*  VDI #2   */
VOID CLEARMEM PROTO((NOTHING));          /*  VDI #3   */
VOID v_updwk PROTO((NOTHING));           /*  VDI #4   */
VOID CHK_ESC PROTO((NOTHING));           /*  VDI #5   */

VOID v_pline PROTO((NOTHING));           /*  VDI #6   */
VOID v_pmarker PROTO((NOTHING));         /*  VDI #7   */
VOID d_gtext PROTO((NOTHING));           /*  VDI #8   */
VOID v_fillarea PROTO((NOTHING));        /*  VDI #9   */
VOID v_cellarray PROTO((NOTHING));       /*  VDI #10  */

VOID v_gdp PROTO((NOTHING));             /*  VDI #11  */
VOID dst_height PROTO((NOTHING));        /*  VDI #12  */
VOID dst_rotation PROTO((NOTHING));      /*  VDI #13  */
VOID vs_color PROTO((NOTHING));          /*  VDI #14  */
VOID vsl_type PROTO((NOTHING));          /*  VDI #15  */

VOID vsl_width PROTO((NOTHING));         /*  VDI #16  */
VOID vsl_color PROTO((NOTHING));         /*  VDI #17  */
VOID vsm_type PROTO((NOTHING));          /*  VDI #18  */
VOID vsm_height PROTO((NOTHING));        /*  VDI #19  */
VOID vsm_color PROTO((NOTHING));         /*  VDI #20  */

VOID dst_font PROTO((NOTHING));          /*  VDI #21  */
VOID dst_color PROTO((NOTHING));         /*  VDI #22  */
VOID vsf_interior PROTO((NOTHING));      /*  VDI #23  */
VOID vsf_style PROTO((NOTHING));         /*  VDI #24  */
VOID vsf_color PROTO((NOTHING));         /*  VDI #25  */

VOID vq_color PROTO((NOTHING));          /*  VDI #26  */
VOID vq_cellarray PROTO((NOTHING));      /*  VDI #27  */
VOID v_locator PROTO((NOTHING));         /*  VDI #28  */
VOID v_valuator PROTO((NOTHING));        /*  VDI #29  */
VOID v_choice PROTO((NOTHING));          /*  VDI #30  */

VOID v_string PROTO((NOTHING));          /*  VDI #31  */
VOID vswr_mode PROTO((NOTHING));         /*  VDI #32  */
VOID vsin_mode PROTO((NOTHING));         /*  VDI #33  */
VOID vql_attr PROTO((NOTHING));          /*  VDI #35  */

VOID vqm_attr PROTO((NOTHING));          /*  VDI #36  */
VOID vqf_attr PROTO((NOTHING));          /*  VDI #37  */
VOID dqt_attributes PROTO((NOTHING));    /*  VDI #38  */
VOID dst_alignment PROTO((NOTHING));     /*  VDI #39  */

VOID d_opnvwk PROTO((NOTHING));          /* VDI #100  */

VOID d_clsvwk PROTO((NOTHING));          /* VDI #101  */
VOID vq_extnd PROTO((NOTHING));          /* VDI #102  */
VOID d_contourfill PROTO((NOTHING));     /* VDI #103  */
VOID vsf_perimeter PROTO((NOTHING));     /* VDI #104  */
VOID v_get_pixel PROTO((NOTHING));       /* VDI #105  */

VOID dst_style PROTO((NOTHING));         /* VDI #106  */
VOID dst_point PROTO((NOTHING));         /* VDI #107  */
VOID vsl_ends PROTO((NOTHING));          /* VDI #108  */
VOID dro_cpyfm PROTO((NOTHING));         /* VDI #109  */
VOID TRAN_FM PROTO((NOTHING));           /* VDI #110  */

VOID XFM_CRFM PROTO((NOTHING));          /* VDI #111  */
VOID dsf_udpat PROTO((NOTHING));         /* VDI #112  */
VOID vsl_udsty PROTO((NOTHING));         /* VDI #113  */
VOID dr_recfl PROTO((NOTHING));          /* VDI #114  */
VOID vqi_mode PROTO((NOTHING));          /* VDI #115  */

VOID dqt_extent PROTO((NOTHING));        /* VDI #116  */
VOID dqt_width PROTO((NOTHING));         /* VDI #117  */
VOID EX_TIMV PROTO((NOTHING));           /* VDI #118  */
VOID dt_loadfont PROTO((NOTHING));       /* VDI #119  */
VOID dt_unloadfont PROTO((NOTHING));     /* VDI #120  */

VOID drt_cpyfm PROTO((NOTHING));         /* VDI #121  */
VOID v_show_c PROTO((NOTHING));          /* VDI #122  */
VOID v_hide_c PROTO((NOTHING));          /* VDI #123  */
VOID vq_mouse_status PROTO((NOTHING));   /* VDI #124  */
VOID VEX_BUTV PROTO((NOTHING));          /* VDI #125  */

VOID VEX_MOTV PROTO((NOTHING));          /* VDI #126  */
VOID VEX_CURV PROTO((NOTHING));          /* VDI #127  */
VOID vq_key_s PROTO((NOTHING));          /* VDI #128  */
VOID s_clip PROTO((NOTHING));            /* VDI #129  */
VOID dqt_name PROTO((NOTHING));          /* VDI #130  */

VOID dqt_fontinfo PROTO((NOTHING));      /* VDI #131  */

#endif
