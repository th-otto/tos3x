/*	WINDOW.H		3/15/89 - 3/30/89	Derek Mui	*/
/*	Change w_iblk to w_ciblk	7/11/92		D.Mui		*/

#define	SUP	0
#define SDOWN	1
#define SLEFT	2
#define	SRIGHT	3

#define	MAXWIN		8	/* max number of window	*/

#define PATHLEN		128
#define INFOLEN		62

#define MAX_DEPTH  8		/* Max folder level	*/

#define	SCREEN	  0	/* related to background			*/

#define NAME		0x0001
#define CLOSE		0x0002
#define FULL		0x0004
#define MOVE		0x0008
#define INFO		0x0010
#define SIZE		0x0020
#define UPARROW		0x0040
#define DNARROW		0x0080
#define VSLIDE		0x0100
#define LFARROW		0x0200
#define RTARROW		0x0400
#define HSLIDE		0x0800

#define ALLITEMS 0x0FFF

typedef struct	grect
{
	int16_t	x;
	int16_t	y;
	int16_t	w;
	int16_t	h;
} GRECT;


typedef	struct	window
{
	char	*w_next;		/* link pointer		*/
	uint16_t	w_id;			/* window handle	*/
	uint16_t	w_free;			/* window free to use	*/
	char	*w_path;		/* window path name	*/
	char	*w_buf;			/* scratch buffer	*/
	int16_t	w_level;		/* max level allowed in this path */
	char	w_info[INFOLEN];	/* info line		*/
	GRECT	w_sizes;	/* window current size and pos	*/
	GRECT	w_work;		/* the current working xywh	*/
	OBJECT	*w_obj;		/* pointer to objects			*/
	int16_t	w_srtitem;	/* file item # to start with		*/
	int16_t	w_icon;		/* disk icon that owns this window	*/
	int16_t	w_vicons;	/* maximum visible icons		*/
	char	*w_memory;	/* file memory owns by this window	*/
	int16_t	w_items;	/* max number of file items		*/
	int16_t	w_maxicons;	/* max # of icons allocated in window	*/
	int16_t	w_icol;		/* max number of col at any time	*/
	int16_t	w_irow;		/* max number of row at any time	*/
	TEDINFO	*w_ted;
	CICONBLK *w_ciblk;	/* starting address of ciconblk		*/
	char	*w_text;	/* starting address of text		*/
	int16_t	w_coli;		/* the horizontal column index 		*/
	int16_t	w_hvicons;	/* number of invisible icon per row	*/
	int16_t	w_rowi;		/* the vertical row index		*/
	int16_t	w_vvicons;	/* number of invisible icon per col	*/
	int16_t	w_xcol;		/* number of visible column of icon	*/
	int16_t	w_xrow;		/* number of visible row of icon	*/
} WINDOW;	


struct	dir
{
	int16_t	d_att;		/* attribute	*/
	uint16_t	d_time;		/* time		*/
	uint16_t	d_date;		/* date		*/
	int32_t	d_size;		/* size		*/
	char	d_name[14];	/* name		*/
	int16_t	d_state;	/* selected	*/
	int16_t	d_order;	/* file's order	*/
};

#define DIR struct dir

typedef	struct	dta
{
	char	reserved[20];	/* reserved area*/
	DIR	dirfile;
} DTA;

typedef struct myblk
{
	GRECT	*mygrect;
	char	*myptr;
} MYBLK;
