/*
 * ctrycodes.h - a mere list of country codes
 *
 * Copyright (C) 2003-2016 The EmuTOS development team
 *
 * Authors:
 *  LVL     Laurent Vogel
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */

/*
 * The country codes were defined by Atari. They do not need to be contiguous.
 * They are used as the country identifier in the ROM header.
 * They are also used in NVRAM to select the UI language and keyboard layout.
 */

/* these are documented in the Compendium */
/* USA */
#define CTRY_US  0
#define CTRY_us  CTRY_US
/* Germany */
#define CTRY_DE  1
#define CTRY_de  CTRY_DE
/* France */
#define CTRY_FR  2
#define CTRY_fr  CTRY_FR
/* United Kingdom */
#define CTRY_UK  3
#define CTRY_uk  CTRY_UK
/* Spain */
#define CTRY_ES  4
#define CTRY_es  CTRY_ES
/* Italy */
#define CTRY_IT  5
#define CTRY_it  CTRY_IT
/* Sweden */
#define CTRY_SV  6
#define CTRY_sv  CTRY_SV
/* Sweden (old name) */
#define CTRY_SE  CTRY_SE
#define CTRY_se  CTRY_SE
/* Switzerland (French) */
#define CTRY_SF  7
#define CTRY_sf  CTRY_SF
/* Switzerland (German), NOT Singapore! */
#define CTRY_SG  8
#define CTRY_sg  CTRY_sg
/* Turkey */
#define CTRY_TR  9
#define CTRY_tr  CTRY_tr
/* Finland */
#define CTRY_FI 10
#define CTRY_fi CTRY_FI
/* Norway */
#define CTRY_NO 11
#define CTRY_no CTRY_NO
/* Denmark */
#define CTRY_DK 12
#define CTRY_dk CTRY_dk
/* Saudi Arabia */
#define CTRY_SA 13
#define CTRY_sa CTRY_SA
/* Holland */
#define CTRY_NL 14
#define CTRY_nl CTRY_nl
/* Czech Republic */
#define CTRY_CZ 15
#define CTRY_cz CTRY_cz
/* Hungary */
#define CTRY_HU 16
#define CTRY_hu CTRY_HU
/* Poland */
#define CTRY_PL 17
#define CTRY_pl CTRY_PL

/*
 * The following country codes were not defined by Atari.
 * Before defining new ones, be sure to register them in tos.hyp:
 * http://toshyp.atari.org/en/003007.html#Cookie_2C_20_AKP
 * Note that those codes are also used in FreeMiNT/XaAES and must match:
 * http://sparemint.atariforge.net/cgi-bin/cvsweb/freemint/sys/keyboard.c?rev=1.114&content-type=text/x-cvsweb-markup
 * http://sparemint.atariforge.net/cgi-bin/cvsweb/freemint/xaaes/src.km/init.c?rev=1.125&content-type=text/x-cvsweb-markup
 */
/* Russia */
#define CTRY_RU 19
#define CTRY_ru CTRY_RU
/* Greece */
#define CTRY_GR 31
#define CTRY_gr CTRY_GR

/* Mexico (Spanish) */
/* (unknown, checked for in sources, but never saw an official value for this) */
#define CTRY_MX 99
#define CTRY_mx CTRY_MX

/*
 * Special value of os_conf to indicate that the display and keyboard languages
 * will be read from the NVRAM. If the NVRAM is invalid, the default settings
 * will be inferred from the COUNTRY Makefile variable.
 */
#define OS_CONF_MULTILANG 0xff

#define CTRY_any OS_CONF_MULTILANG
#define CTRY_ANY OS_CONF_MULTILANG
