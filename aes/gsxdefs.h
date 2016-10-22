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

extern WS gl_ws;

typedef struct fdbstr
{
	int32_t		fd_addr;
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

