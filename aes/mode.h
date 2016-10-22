/*
 * mode.h
 * ======================================================================
 * Falcon/030 setmode call and defines.
 * 
 * 920608 towns	created.
 *
 */

/* NOTE: These XBIOS numbers are subject to CHANGE. In fact, I know 
 *       they will CHANGE. DO NOT RELY ON THEM.
 */

#define VsetMode(a)	(int)xbios(88,a)
#define VgetMonitor(a)	(int)xbios(89)
#define VsetSync(a)	(void)xbios(90,a)

#define VERTFLAG	0x100
#define STMODES		0x080
#define OVERSCAN	0x040
#define PAL			0x020
#define	VGA			0x010
#define TV			0

#define	COL80		0x08
#define	COL40		0x00
#define NUMCOLS		7	/* This is the bit mask for bit/plane numbers */

#define BPS16		4
#define BPS8		3
#define BPS4		2
#define BPS2		1
#define BPS1		0
