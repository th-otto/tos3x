/*
 * header.h - definitions for the TOS header
 */

#ifndef HEADER_H
#define HEADER_H

/*
 * config.h should define TOSVERSION and COUNTRY
 */
#include "config.h"
#include "ctrycodes.h"

/* the year used in copyright notices */
#define COPYRIGHT_YEAR "1991"

/*
 * for our purpose, use the dates from the official ROM versions
 * rather than the actual build date:
 * - OS_DATE: the build date in Binary-Coded Decimal
 * - OS_DOSDATE: the build date in GEMDOS format
 */

#if TOSVERSION >= 0x0306
#define OS_DATE $09241991
#define OS_DOSDATE $1738
#else

/* TOS version 2.08 is 2.06 for the ST-book */
#if TOSVERSION >= 0x0208
#define OS_DATE $03101992
#define OS_DOSDATE $186a
#else

#if TOSVERSION >= 0x0206
#define OS_DATE $11141991
#define OS_DOSDATE $176e
#endif
#endif
#endif


/* the PAL/NTSC flag */
#if OS_COUNTRY == CTRY_US
#define OS_PAL 1
#else
#define OS_PAL 0
#endif

#endif /* HEADER_H */
