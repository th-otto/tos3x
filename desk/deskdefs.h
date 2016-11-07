#define INFSIZE 4192            /* inf file size        */

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
