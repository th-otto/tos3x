/*
 * header.h - definitions for the TOS header
 */

#ifndef HEADER_H
#define HEADER_H

/*
 * config.h should define TOSVERSION and COUNTRY
 */
#include "../common/config.h"
#include "../common/ctrycodes.h"

/* the year used in copyright notices */
#define COPYRIGHT_YEAR "1991"

/*
 * for our purpose, use the dates from the official ROM versions
 * rather than the actual build date:
 * - OS_DATE: the build date in Binary-Coded Decimal
 * - OS_DOSDATE: the build date in GEMDOS format
 */

#if TOSVERSION >= 0x0100
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $02061986
#define OS_DOSDATE $176E
#endif

#if TOSVERSION >= 0x0102
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $22041987
#define OS_DOSDATE $176E
#endif

#if TOSVERSION >= 0x0104
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $06041989
#define OS_DOSDATE $176E
#endif

#if TOSVERSION >= 0x0106
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $29071989
#define OS_DOSDATE $176E
#endif

#if TOSVERSION >= 0x0162
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $01011990
#define OS_DOSDATE $176E
#endif

#if TOSVERSION >= 0x0205
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $05121990
#define OS_DOSDATE $176E
#endif

#if TOSVERSION >= 0x0206
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $11141991
#define OS_DOSDATE $176E
#endif

/* TOS version 2.08 is 2.06 for the ST-book */
#if TOSVERSION >= 0x0208
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $03101992
#define OS_DOSDATE $186A
#endif

#if TOSVERSION >= 0x0301
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $29081990
#define OS_DOSDATE $1738
#endif

#if TOSVERSION >= 0x0306
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $09241991
#define OS_DOSDATE $1738
#endif

#if TOSVERSION >= 0x0404
#undef OS_DATE
#undef OS_DOSDATE
#define OS_DATE $03081993
#define OS_DOSDATE $1A68
#endif


/* the PAL/NTSC flag */
#if OS_COUNTRY == CTRY_US
#define OS_PAL 0
#else
#define OS_PAL 1
#endif

#endif /* HEADER_H */
