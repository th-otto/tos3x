/*	WINDOW.H		3/15/89 - 3/30/89	Derek Mui	*/
/*	Change w_iblk to w_ciblk	7/11/92		D.Mui		*/

#define	SUP		0
#define SDOWN	1
#define SLEFT	2
#define	SRIGHT	3


#define INFOLEN		62

#define	SCREEN	  0	/* related to background */

#define ALLITEMS (NAME|CLOSER|FULLER|MOVER|INFO|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE)

typedef struct dir
{
	int16_t	 d_att;		/* attribute */
	uint16_t d_time;	/* time */
	uint16_t d_date;	/* date */
	int32_t	 d_size;	/* size */
	char	 d_name[14]; /* name */
	int16_t	 d_state;	/* selected */
	int16_t	 d_order;	/* file's order */
} DIR;

typedef	struct dta
{
	char	reserved[20];	/* reserved area */
	DIR	dirfile;
} DTA;

typedef	struct deskwin
{
	/*   0 */ GRECT	w_sizes;			/* window current size and pos */
	/*   8 */ int16_t w_coli;			/* the horizontal column index */
	/*  10 */ int16_t w_rowi;			/* the vertical row index */
	/*  12 */ int16_t w_icon;			/* disk icon that owns this window */
	/*  14 */ char w_path[12];			/* window path name */
	/*  18 */ char w_info[INFOLEN];		/* info line */
	/*  88 */ GRECT	w_work;				/* the current working xywh */
	/*  96 */ OBJECT *w_obj;			/* pointer to objects */
	/* 100 */ int16_t w_srtitem;		/* file item # to start with */
	/* 104 */ int16_t w_vicons;			/* maximum visible icons */
	/* 106 */ DIR *w_memory;			/* file memory owns by this window */
	/* 110 */ int16_t w_items;			/* max number of file items */
	/* 112 */ int16_t w_maxicons;		/* max # of icons allocated in window */
	/* 114 */ int16_t w_icol;			/* max number of col at any time */
	/* 116 */ int16_t w_irow;			/* max number of row at any time */
	/* 118 */ TEDINFO *w_ted;
#if COLORICON_SUPPORT
	/* 122 */ CICONBLK *w_ciblk;		/* starting address of ciconblk */
#else
	/* 122 */ ICONBLK *w_iblk;			/* starting address of iconblk */
#endif
	/* 126 */ char *w_text;				/* starting address of text */
	/* 132 */ int16_t w_hvicons;		/* number of invisible icon per row */
	/* 136 */ int16_t w_vvicons;		/* number of invisible icon per col */
	/* 138 */ int16_t w_xcol;			/* number of visible column of icon */
	/* 140 */ int16_t w_xrow;			/* number of visible row of icon */
} DESKWIN;	


typedef struct myblk
{
	GRECT	*mygrect;
	char	*myptr;
} MYBLK;
