#define PATHLEN		128
#define NAMELEN     14
#define	MAXWIN		8	/* max number of window	*/

#ifndef BINEXACT
#define BINEXACT 1
#endif

#ifndef MULTILANG_SUPPORT
#define MULTILANG_SUPPORT (OS_COUNTRY == OS_CONF_MULTILANG)
#endif

#ifndef SUBMENUS
#define SUBMENUS (AESVERSION >= 0x340)
#endif

#ifndef AES3D
#define AES3D (AESVERSION >= 0x330)
#endif

#ifndef COLORICON_SUPPORT
#define COLORICON_SUPPORT (AESVERSION >= 0x330)
#endif

#ifndef POPUP_SUPPORT
#define POPUP_SUPPORT (AESVERSION >= 0x330)
#endif

/* max number of menus  cjg - 07/07/92 */
#if TOSVERSION >= 0x400
/* Minus 2 for sparrow */
#define MAXMENU         29
#else
#define MAXMENU         30
#endif
