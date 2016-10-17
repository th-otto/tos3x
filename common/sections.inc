#include "../common/config.h"
#include "../common/ctrycodes.h"

#if TOSVERSION == 0x306

/*
 * length of BIOS text segment is different due to some extra
 * code for ALT-scancode mappings
 */

/*
 * length of BIOS text segment of the US version,
 * which does not have any extra code
 */
#define BTLEN_US    $0000641C
#define DTLEN_US    $0000FBCC


#if OS_COUNTRY == CTRY_US
#define BIOSTLEN    BTLEN_US
#define DESKTLEN    DTLEN_US
#endif

#if OS_COUNTRY == CTRY_DE
#define BIOSTLEN    BTLEN_US+$5A
#define DESKTLEN    DTLEN_US-$2A
#endif

#if OS_COUNTRY == CTRY_FR
#define BIOSTLEN    BTLEN_US+$78
#define DESKTLEN    DTLEN_US-$2A
#endif

#if OS_COUNTRY == CTRY_ES
#define BIOSTLEN    BTLEN_US+$86
#define DESKTLEN    DTLEN_US-$2A
#endif

#if OS_COUNTRY == CTRY_IT
#define BIOSTLEN    BTLEN_US+$68
#define DESKTLEN    DTLEN_US-$2A
#endif

#if OS_COUNTRY == CTRY_UK
#define BIOSTLEN    BTLEN_US
#define DESKTLEN    DTLEN_US
#endif

#if OS_COUNTRY == CTRY_SV
#define BIOSTLEN    BTLEN_US+$78
#define DESKTLEN    DTLEN_US-$2A
#endif

#if OS_COUNTRY == CTRY_PL
#define BIOSTBASE   $00380000
#define BIOSTLEN    BTLEN_US+$5A
#define DESKTLEN    DTLEN_US-$2A
#endif


/*
 * these are identical in all language versions
 */
#define BIOSDLEN    $000009D8
#define VDITLEN     $0000D06E
#define VDIDLEN     $00006EFA
#define BDOSTLEN    $000062AC
#define BDOSDLEN    $0000039E
#define AESTLEN     $0000CCD6
#define AESDLEN     $000006D8
#define DESKDLEN    $00000000

#endif /* TOSVERSION == 0x306 */


#if TOSVERSION == 0x206

#define BTLEN_US    $0000603A
#define DTLEN_US    $0000FBFC

#if OS_COUNTRY == CTRY_US
#define BIOSTLEN    BTLEN_US
#define DESKTLEN    DTLEN_US
#endif

#if OS_COUNTRY == CTRY_DE
#define BIOSTLEN    BTLEN_US+$5A
#define DESKTLEN    DTLEN_US-$5A
#endif

#if OS_COUNTRY == CTRY_FR
#define BIOSTLEN    BTLEN_US+$78
#define DESKTLEN    DTLEN_US-$5A
#endif

#if OS_COUNTRY == CTRY_ES
#define BIOSTLEN    BTLEN_US+$86
#define DESKTLEN    DTLEN_US-$5A
#endif

#if OS_COUNTRY == CTRY_IT
#define BIOSTLEN    BTLEN_US+$68
#define DESKTLEN    DTLEN_US-$5A
#endif

#if OS_COUNTRY == CTRY_UK
#define BIOSTLEN    BTLEN_US
#define DESKTLEN    DTLEN_US
#endif

/*
 * these are identical in all language versions
 */
#define BIOSDLEN    $00000000
#define VDITLEN     $0000999A
#define VDIDLEN     $00000000
#define BDOSTLEN    $000062FC
#define BDOSDLEN    $00000000
#define AESTLEN     $0000CCD6
#define AESDLEN     $00000000
#define DESKDLEN    $00000000

#endif /* TOSVERSION == 0x206 */

#ifndef BIOSTBASE
#define BIOSTBASE   $00E00000
#endif


/*
 * now calculate addresses from above
 */
#define VDITBASE  BIOSTBASE+BIOSTLEN
#define BDOSTBASE VDITBASE+VDITLEN
#define AESTBASE  BDOSTBASE+BDOSTLEN
#define DESKTBASE AESTBASE+AESTLEN
#define ETEXT     DESKTBASE+DESKTLEN

#define BIOSDBASE ETEXT
#define VDIDBASE  BIOSDBASE+BIOSDLEN
#define BDOSDBASE VDIDBASE+VDIDLEN
#define AESDBASE  BDOSDBASE+BDOSDLEN
#define DESKDBASE AESDBASE+AESDLEN
#define EDATA     DESKDBASE+DESKDLEN

#ifdef __MAKEFILE__

/*
 * the output of this is fed through bc
 */
ibase=16
obase=10

bios_tbase=BIOSTBASE
bios_tlen=BIOSTLEN
bios_dbase=BIOSDBASE
bios_dlen=BIOSDLEN
vdi_tbase=VDITBASE
vdi_tlen=VDITLEN
vdi_dbase=VDIDBASE
vdi_dlen=VDIDLEN
bdos_tbase=BDOSTBASE
bdos_tlen=BDOSTLEN
bdos_dbase=BDOSDBASE
bdos_dlen=BDOSDLEN

rombase=BIOSTBASE

print "bios_tbase=", bios_tbase, "\n"
print "bios_tlen=", bios_tlen, "\n"
print "bios_dbase=", bios_dbase, "\n"
print "bios_dlen=", bios_dlen, "\n"
print "vdi_tbase=", vdi_tbase, "\n"
print "vdi_tlen=", vdi_tlen, "\n"
print "vdi_dbase=", vdi_dbase, "\n"
print "vdi_dlen=", vdi_dlen, "\n"
print "bdos_tbase=", bdos_tbase, "\n"
print "bdos_tlen=", bdos_tlen, "\n"
print "bdos_dbase=", bdos_dbase, "\n"
print "bdos_dlen=", bdos_dlen, "\n"

print "rombase=", rombase, "\n"

quit

#endif
