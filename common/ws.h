#ifndef WS_H
#define WS_H 1

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

#endif
