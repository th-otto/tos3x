#ifndef STRUCT88_H
#define MC68K 1
#define I8086 0
#include "../aes/struct88.h"
#endif

struct wxinfo {
	VOIDPTR *wx_mem;
	int16_t maxwin;
	int16_t max_xx;
	PD **rlr;
	int16_t max_yy;
	uint16_t queue_size;
	/* LPTREE */ VOIDPTR *newdesk;
	int16_t *newroot;
	/* LPTREE */ VOIDPTR *stdesk;
	VOIDPTR func;
};

struct _winxvars {
	/* $50 */ int32_t magic;         /* xAEF2 */
	/* $54 */ uint16_t version;      /* xAEF6 */
	/* $56 */ uint16_t date;         /* xAEF8 */
	/* $58 */ uint16_t global_flags; /* xAEFA */
	/* $5a */ VOIDPTR wx_mem;        /* xAEFC */
	/* $5e */ const struct wxinfo *info;   /* xAF00 */
	/* $62 */ int32_t (CDECL *winxfunc) PROTO((short funcid, ...));        /* xAF04 */
	/* $66 */ int16_t xAF08;
	/* $68 */ char unused2[6];
	/* $6e */ char xAF10;
	/* $6f */ char xAF11;
};
extern struct _winxvars winxvars;

int16_t wx_create PROTO((uint16_t kind, GRECT *rect));
int16_t wx_open PROTO((int16_t handle, GRECT *rect));
int16_t wx_close PROTO((int16_t handle));
int16_t wx_delete PROTO((int16_t handle));
int16_t wx_get PROTO((int16_t handle, int16_t field, int16_t *ow));
int16_t wx_set PROTO((int16_t handle, int16_t field, int16_t *iw));
int16_t wx_find PROTO((int mx, int my));
int16_t wx_update PROTO((int code));
VOID wx_calc PROTO((int16_t type, int16_t kind, int16_t ix, int16_t iy, int16_t iw, int16_t ih, int16_t *ox, int16_t *oy, int16_t *ow, int16_t *oh));
int16_t wx_new PROTO((NOTHING));
VOID wx_setactive PROTO((NOTHING));
VOID wx_drawdesk PROTO((GRECT *dirty));
short spl7 PROTO((NOTHING));
VOID spl PROTO((short));
PD *wx_srchwp PROTO((int16_t wh, int16_t button));
VOID wx_nameit PROTO((PD *p, const char *pname));
VOID wx_rmerge PROTO((int16_t *nm, int16_t *om));
VOID wx_getmouse PROTO((NOTHING));
VOIDPTR wx_alloc PROTO((long size));
BOOLEAN wx_start PROTO((NOTHING));
