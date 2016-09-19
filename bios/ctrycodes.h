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
/* Germany */
#define CTRY_DE  1
/* France */
#define CTRY_FR  2
/* United Kingdom */
#define CTRY_UK  3
/* Spain */
#define CTRY_ES  4
/* Italy */
#define CTRY_IT  5
/* Sweden */
#define CTRY_SE  6
/* Sweden (alternative name) */
#define CTRY_SV  CTRY_SE
/* Switzerland (French) */
#define CTRY_SF  7
/* Switzerland (German), NOT Singapore! */
#define CTRY_SG  8
/* Turkey */
#define CTRY_TR  9
/* Finland */
#define CTRY_FI 10
/* Norway */
#define CTRY_NO 11
/* Denmark */
#define CTRY_DK 12
/* Saudi Arabia */
#define CTRY_SA 13
/* Holland */
#define CTRY_NL 14
/* Czech Republic */
#define CTRY_CZ 15
/* Hungary */
#define CTRY_HU 16

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
/* Greece */
#define CTRY_GR 31

/*
 * Special value of os_conf to indicate that the display and keyboard languages
 * will be read from the NVRAM. If the NVRAM is invalid, the default settings
 * will be inferred from the COUNTRY Makefile variable.
 */
#define OS_CONF_MULTILANG 0xff
