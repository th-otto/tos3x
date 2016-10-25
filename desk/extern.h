/*	EXTERN.H		6/12/89 - 6/13/89	Derek Mui	*/
/*	Take out some variable	6/28/89					*/


char *get_fstring();
char *get_string();
char *lp_fill();

APP *app_icon();
APP *app_key();
APP *app_xtype();
APP *app_alloc();
int32_t av_mem();

OBJECT *get_tree();
ICONBLK *get_icon();
int16_t in_parent();
APP *app_xtype();
DIR *get_dir();
char *put_name();

WINDOW *get_win();
WINDOW *get_top();
WINDOW *w_gfirst();
WINDOW *w_gnext();

char *g_name();
char *r_slash();
char toupper();
char *scasb();
int16_t strlen();					/* cjg 09/21/92 */
int16_t XDeselect();


/* extern	int16_t	namecon; */	/* name conflict flag		*/
extern	char	dr[];
extern	int16_t	p_timedate;		/* preserve time and date	*/


extern	WINDOW	*ww_win;

extern int16_t	d_nrows;
extern int16_t	d_level;
extern char	*d_path;

extern int16_t	gl_hchar;
extern int16_t	gl_wchar;
extern int16_t	gl_hbox;
extern int16_t	gl_wbox;

extern int16_t	gl_nrows;
extern int16_t	gl_ncols;

/* extern int16_t	work_in[];	*/	/* Input to GSX parameter array */
/* extern int16_t	work_out[];	*/	/* Output from GSX parameter array */
extern int16_t	pxyarray[];		/* input point array 		*/

extern int16_t	d_xywh[];		/* disk icon pline points	*/
extern int16_t	f_xywh[];		/* file icon pline points	*/

extern OBJECT	*menu_addr;		/* menu address			*/
extern OBJECT	*background;		/* desktop object address	*/	

extern GRECT	dicon;		/* desktop icon size			*/
extern GRECT	r_dicon;	/* real time desktop icon size		*/
extern OBJECT	*iconaddr;	/* desktop icon dialogue address	*/
extern int16_t	maxicon;	/* max number of desktop icons		*/

extern int16_t	i_status;	/* current status TURE or FALSE		*/

/* extern int16_t	g_defdrv;	*/ /* save the default drive		*/

extern int32_t	gh_buffer;

extern IDTYPE	*backid;	/* background icon type definition	*/

extern APP	*appnode;
extern APP	*appfree;
extern APP	*applist;

extern char	path1[];
extern char	path2[];
extern char	*path3;
extern char	inf_path[];

extern DTA	dtabuf;		/* dta buffer	*/

extern WINDOW	*warray[];	


/*	Variables for the desktop.inf file	*/

extern int16_t	s_sort;		/* sort item	*/
extern int16_t	s_view;		/* view item	*/
extern int16_t	ccopy_save;	/* copy ?	*/
extern int16_t	cdele_save;	/* delete ?	*/
extern int16_t	write_save;	/* write ?	*/
extern int16_t	cbit_save;	/* bitblt 	*/
extern int16_t	pref_save;	/* screen pref	*/
extern int16_t	s_cache;	/* cache 	*/
extern int16_t	s_stofit;	/* size to fit	*/
extern uint16_t	windspec;	/* window pattern spec	*/

/************************************************/

extern char	autofile[];

extern char	path2[];
extern char	g_buffer[];

/* extern char	mbuffer[];	*/

extern char	comtail[];

extern WINDOW	winpd[];		/* window process structure	*/
extern WINDOW	*winhead;
extern GRECT	full;			/* full window size value	*/

extern GRECT	fobj;		/* file object	*/

extern	int16_t	deskp[];
extern	int16_t	winp[];

extern	char	wildext[];
extern	char	infdata[];
extern	char	infpath[];
extern	char	wilds[];
extern	char	icndata[];
extern	char	noext[];

extern	char 	getall[];
extern	char 	bckslsh[];
extern	char 	curall[];
extern	char 	baklvl[];

/*	extern	char	Alloc[];	*/

extern	char	Nextline[];
extern	char	Nostr[];

extern int16_t d_display;

extern int16_t f_rename;

extern WINDOW *o_win;

extern int16_t d_dir;

extern WINDOW *x_win;

extern int16_t x_type;

extern int16_t gl_restype;

extern BOOLEAN gl_rschange;

extern int16_t d_exit;

extern int32_t gl_vdo;

extern int16_t pref_save;

extern int16_t m_cpu;

extern int16_t s_defdir;

extern int16_t s_fullpath;

extern int16_t numicon;

extern int16_t x_status;

extern int16_t x_type;

extern char mkeys[];

extern char restable[];

extern int16_t ftab[];

extern int16_t font_save;

extern int16_t m_st;

extern int16_t tb3[];

extern char mentable[];

extern uint16_t d_rezword;					/* set_mode() value ( system ) */

