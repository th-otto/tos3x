/*      DESKSTOR.C              6/12/89 - 6/13/89       Derek Mui       */
/*      Take out some variable  6/28/89 - 9/13/89       D.Mui           */
/*      Take out strXXX.h       7/7/92                  D.Mui           */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"

char dr[32];							/* drives flag          */
BOOLEAN p_timedate;						/* preserve time and date   */
WINDOW *ww_win;							/* for w_gfirst and w_gnext */
int16_t d_nrows;						/* number of rows used by show  */
int16_t d_level;						/* window path level        */
char *d_path;							/* window path buffer       */
int16_t pxyarray[10];					/* input point array        */ /* unused */
int16_t d_xywh[18];						/* disk icon pline points   */
int16_t f_xywh[18];						/* file icon pline points   */
OBJECT *menu_addr;						/* menu address         */
OBJECT *background;						/* desktop object address   */
GRECT dicon;							/* desktop icon size        */
GRECT r_dicon;							/* real time desktop icon size  */
OBJECT *iconaddr;						/* desktop icon dialogue address */
int16_t maxicon;						/* max number of desktop icons  */
BOOLEAN i_status;						/* current status TURE or FALSE */ /* unused */
/* int16_t g_defdrv; */					/* save the default drive   */
intptr_t gh_buffer;						/* ghost icon outline buffer address */
IDTYPE *backid;							/* background icon type definition  */
APP *appnode;							/* app buffer array     */
APP *appfree;							/* app buffer free list     */
APP *applist;							/* app buffer list      */
DTA dtabuf;								/* dta buffer   */

WINDOW *warray[MAXWIN];					/* window structure */

/*
 * Variables for the desktop.inf file
 */
int16_t s_sort;							/* sort item    */
int16_t s_view;							/* view item    */
BOOLEAN ccopy_save;						/* copy ?   */
BOOLEAN cdele_save;						/* delete ? */
int16_t write_save;						/* write ?  */
BOOLEAN cbit_save;						/* bitblt   */
int16_t pref_save;						/* screen pref  */
BOOLEAN s_cache;						/* cache    */
BOOLEAN s_stofit;						/* size to fit  */
uint16_t windspec;						/* window pattern   */

/************************************************/

char autofile[PATHLEN];
char path1[PATHLEN];					/* utility path     */
char path2[PATHLEN];
char *path3;
char inf_path[PATHLEN];					/* store the inf path   */
char g_buffer[160];						/* merge string buffer  */
char comtail[PATHLEN];					/* comtail tail buffer */
WINDOW winpd[MAXWIN];					/* window process structure */
WINDOW *winhead;						/* head of window list      */
GRECT full;								/* full window size value   */
GRECT fobj;								/* file object  */
int16_t deskp[3];						/* desktop pattern  */
int16_t winp[3];						/* window pattern   */
char const getall[] = "*.*";
char const bckslsh[] = "\\";
char const curall[] = ".\\*.*";
char const baklvl[] = ".\\..";
char const wildext[] = "A:\\*.*";
char const wilds[] = "\\*.*";
char const noext[] = "*.";
char const Nostr[] = "";
char const infdata[] = "DESKTOP.INF";
char const infpath[] = "C:\\NEWDESK.INF";
char const icndata[] = "C:\\DESKICON.RSC";
char const Nextline[] = "\012\015";
