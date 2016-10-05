/*	DESKSTOR.C		6/12/89 - 6/13/89	Derek Mui	*/
/*	Take out some variable	6/28/89	- 9/13/89	D.Mui		*/
/*	Take out strXXX.h	7/7/92			D.Mui		*/

/************************************************************************/
/*	New Desktop for Atari ST/TT Computer				*/
/*	Atari Corp							*/
/*	Copyright 1989,1990 	All Rights Reserved			*/
/************************************************************************/

#include <portab.h>
#include <mobdefs.h>
#include <defines.h>
#include <window.h>
#include <gemdefs.h>
#include <deskusa.h>
#include <osbind.h>

char dr[32];							/* drives flag          */

int16_t p_timedate;						/* preserve time and date   */

WINDOW *ww_win;							/* for w_gfirst and w_gnext */

int16_t d_nrows;							/* number of rows used by show  */

int16_t d_level;							/* window path level        */

char *d_path;							/* window path buffer       */

int16_t pxyarray[10];						/* input point array        */

int16_t d_xywh[18];						/* disk icon pline points   */

int16_t f_xywh[18];						/* file icon pline points   */

OBJECT *menu_addr;						/* menu address         */

OBJECT *background;						/* desktop object address   */

GRECT dicon;							/* desktop icon size        */

GRECT r_dicon;							/* real time desktop icon size  */

OBJECT *iconaddr;						/* desktop icon dialogue address */

int16_t maxicon;							/* max number of desktop icons  */

int16_t i_status;							/* current status TURE or FALSE */

						/* int16_t	g_defdrv;	*//* save the default drive       */
int32_t gh_buffer;							/* ghost icon outline buffer address    */

IDTYPE *backid;							/* background icon type definition  */

APP *appnode;							/* app buffer array     */

APP *appfree;							/* app buffer free list     */

APP *applist;							/* app buffer list      */

DTA dtabuf;								/* dta buffer   */

WINDOW *warray[MAXWIN];					/* window structure */

/*	Variables for the desktop.inf file	*/

int16_t s_sort;							/* sort item    */

int16_t s_view;							/* view item    */

int16_t ccopy_save;						/* copy ?   */

int16_t cdele_save;						/* delete ? */

int16_t write_save;						/* write ?  */

int16_t cbit_save;							/* bitblt   */

int16_t pref_save;							/* screen pref  */

int16_t s_cache;							/* cache    */

int16_t s_stofit;							/* size to fit  */

uint16_t windspec;							/* window pattern   */

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

int16_t deskp[3];							/* desktop pattern  */

int16_t winp[3];							/* window pattern   */

char getall[] = "*.*";

char bckslsh[] = "\\";

char curall[] = ".\\*.*";

char baklvl[] = ".\\..";

char wildext[] = "A:\\*.*";

char wilds[] = "\\*.*";

char noext[] = "*.";

char Nostr[] = "";

char infdata[] = "DESKTOP.INF";

char infpath[] = "C:\\NEWDESK.INF";

char icndata[] = "C:\\DESKICON.RSC";

char Nextline[] = "\012\015";
