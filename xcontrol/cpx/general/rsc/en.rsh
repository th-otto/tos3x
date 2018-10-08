/*
 * GEM resource C output of en
 *
 * created by ORCS 2.16
 */

#ifndef _LONG_PTR
#  define _LONG_PTR LONG
#endif

#ifndef OS_NORMAL
#  define OS_NORMAL 0x0000
#endif
#ifndef OS_SELECTED
#  define OS_SELECTED 0x0001
#endif
#ifndef OS_CROSSED
#  define OS_CROSSED 0x0002
#endif
#ifndef OS_CHECKED
#  define OS_CHECKED 0x0004
#endif
#ifndef OS_DISABLED
#  define OS_DISABLED 0x0008
#endif
#ifndef OS_OUTLINED
#  define OS_OUTLINED 0x0010
#endif
#ifndef OS_SHADOWED
#  define OS_SHADOWED 0x0020
#endif
#ifndef OS_WHITEBAK
#  define OS_WHITEBAK 0x0040
#endif
#ifndef OS_DRAW3D
#  define OS_DRAW3D 0x0080
#endif

#ifndef OF_NONE
#  define OF_NONE 0x0000
#endif
#ifndef OF_SELECTABLE
#  define OF_SELECTABLE 0x0001
#endif
#ifndef OF_DEFAULT
#  define OF_DEFAULT 0x0002
#endif
#ifndef OF_EXIT
#  define OF_EXIT 0x0004
#endif
#ifndef OF_EDITABLE
#  define OF_EDITABLE 0x0008
#endif
#ifndef OF_RBUTTON
#  define OF_RBUTTON 0x0010
#endif
#ifndef OF_LASTOB
#  define OF_LASTOB 0x0020
#endif
#ifndef OF_TOUCHEXIT
#  define OF_TOUCHEXIT 0x0040
#endif
#ifndef OF_HIDETREE
#  define OF_HIDETREE 0x0080
#endif
#ifndef OF_INDIRECT
#  define OF_INDIRECT 0x0100
#endif
#ifndef OF_FL3DIND
#  define OF_FL3DIND 0x0200
#endif
#ifndef OF_FL3DBAK
#  define OF_FL3DBAK 0x0400
#endif
#ifndef OF_FL3DACT
#  define OF_FL3DACT 0x0600
#endif
#ifndef OF_MOVEABLE
#  define OF_MOVEABLE 0x0800
#endif
#ifndef OF_POPUP
#  define OF_POPUP 0x1000
#endif

#ifndef G_SWBUTTON
#  define G_SWBUTTON 34
#endif
#ifndef G_POPUP
#  define G_POPUP 35
#endif
#ifndef G_EDIT
#  define G_EDIT 37
#endif
#ifndef G_SHORTCUT
#  define G_SHORTCUT 38
#endif
#ifndef G_SLIST
#  define G_SLIST 39
#endif
#ifndef G_EXTBOX
#  define G_EXTBOX 40
#endif
#ifndef G_OBLINK
#  define G_OBLINK 41
#endif

#ifndef WHITEBAK
#  define WHITEBAK OS_WHITEBAK
#endif
#ifndef DRAW3D
#  define DRAW3D OS_DRAW3D
#endif
#ifndef FL3DIND
#  define FL3DIND OF_FL3DIND
#endif
#ifndef FL3DBAK
#  define FL3DBAK OF_FL3DBAK
#endif
#ifndef FL3DACT
#  define FL3DACT OF_FL3DACT
#endif

#ifndef C_UNION
#ifdef __PORTAES_H__
#  define C_UNION(x) { (_LONG_PTR)(x) }
#endif
#ifdef __GEMLIB__
#  define C_UNION(x) { (_LONG_PTR)(x) }
#endif
#ifdef __PUREC__
#  define C_UNION(x) { (_LONG_PTR)(x) }
#endif
#ifdef __ALCYON__
#  define C_UNION(x) x
#endif
#endif
#ifndef C_UNION
#  define C_UNION(x) (_LONG_PTR)(x)
#endif

#define T0OBJ 0
#define T1OBJ 49
#define FREEBB 4
#define FREEIMG 4
#define FREESTR 46

BYTE *rs_strings[] = {
	(BYTE *)"0.00",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Repeat Rate",
	(BYTE *)"0.00",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Response Rate",
	(BYTE *)"Double Click!",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Cancel",
	(BYTE *)"OK",
	(BYTE *)"Save",
	(BYTE *)"Status",
	(BYTE *)"01234567890",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"On",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"01234567890",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"On",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"System Statistics",
	(BYTE *)"0000",
	(BYTE *)"__.__",
	(BYTE *)"9999",
	(BYTE *)"00000000",
	(BYTE *)"__/__/____",
	(BYTE *)"99999999",
	(BYTE *)"ST Free RAM:",
	(BYTE *)"0000000000",
	(BYTE *)"TT Free RAM:",
	(BYTE *)"0000000000",
	(BYTE *)"Total Free RAM:",
	(BYTE *)"0000000000",
	(BYTE *)"TOS Version:",
	(BYTE *)"TOS Date:",
	(BYTE *)"Exit",
	(BYTE *)"00000000",
	(BYTE *)"____-__-__",
	(BYTE *)"99999999"
};

static WORD IMAG0[] = {
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0x7F30, 
0x0, 0x4, 0xC190, 0x0, 0xC, 0x8098, 0x0, 0x59, 
0x80CD, 0x0, 0xD1, 0x45, 0x8000, 0x91, 0x44, 0x8000, 
0x1B3, 0x66, 0xC000, 0x122, 0x22, 0x4000, 0x122, 0x22, 
0x4000, 0x366, 0x33, 0x6000, 0x244, 0x11, 0x2000, 0x244, 
0x11, 0x2000, 0x6CC, 0x19, 0xB000, 0xC88, 0x8, 0x9800, 
0x1998, 0xC, 0xCC00, 0x3110, 0x4, 0x4600, 0x2730, 0x6, 
0x7200, 0x2C60, 0x3, 0x1A00, 0x9DF, 0xFFFD, 0xC800, 0x900, 
0x0, 0x4800, 0x9FF, 0xFFFF, 0xC800, 0xC7F, 0xFFFF, 0x1800, 
0x0, 0x3600, 0x0, 0x0, 0x1C00, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0};

static WORD IMAG1[] = {
0xFF40, 0x2FF, 0x8000, 0x1, 0xBF00, 0xFD, 0xA000, 0x5, 
0xAFFF, 0xFFF5, 0xAC00, 0x35, 0xAE00, 0x55, 0xAFFF, 0xFF95, 
0xFFF, 0xFF90, 0x8F80, 0x191, 0xF80, 0x190, 0xF87, 0xF190, 
0xF8F, 0xF990, 0xF9E, 0x1990, 0xF9C, 0x190, 0xF9C, 0x190, 
0xF9C, 0x190, 0xF9C, 0x190, 0xF9C, 0x190, 0xF9E, 0x1990, 
0xF8F, 0xF990, 0xF87, 0xF190, 0x8F80, 0x191, 0xFFF, 0xFF90, 
0xAEFF, 0xFF95, 0xADFF, 0xFFD5, 0xABFF, 0xFFF5, 0xAFFF, 0xFFF5, 
0xA000, 0x5, 0xBF00, 0xFD, 0x8000, 0x1, 0xFF40, 0x2FF};

static WORD IMAG2[] = {
0xFFFF, 0xC000, 0x8000, 0x4000, 0x8000, 0x4000, 0x8380, 0x4000, 
0x87C0, 0x4000, 0x87DC, 0x4000, 0x87FE, 0x4000, 0x83BE, 0x4000, 
0x87FE, 0x4000, 0x8FFC, 0x4000, 0x9FF8, 0x4000, 0xBFC0, 0x4000, 
0xBF80, 0x4000, 0x9FE0, 0x4000, 0x83F0, 0x4000, 0x87F8, 0x0, 
0x8FFC, 0x7800, 0x9FFC, 0x6400, 0xB7FE, 0xC400, 0x8DFF, 0xC000, 
0xEDFF, 0x8000, 0x8FF, 0x8000, 0x3F, 0x0, 0xFC, 0x0, 
0x1E0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

static WORD IMAG3[] = {
0x3, 0xFFFF, 0x2, 0x1, 0x2, 0x1, 0x2, 0x1, 
0x2, 0x1, 0x2, 0xE39, 0x2, 0x1F79, 0x2, 0x1FF1, 
0x2, 0x1FE1, 0xF, 0xCFF1, 0x3F, 0xE7F1, 0x7F, 0xFFF9, 
0x7F, 0xFFF9, 0xFF, 0xFF99, 0x1FF, 0xFF81, 0x1FF, 0xFF81, 
0x1FF, 0xFDC1, 0x1FF, 0xF9E1, 0x1FF, 0xDC61, 0x1BF, 0xCF61, 
0x187, 0xC6FF, 0xC3, 0xE000, 0x61, 0xF000, 0x20, 0x0, 
0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

LONG rs_frstr[] = {
	0
};

BITBLK rs_bitblk[] = {
	{ (WORD *)0L, 6, 28, 0, 0, 1 },
	{ (WORD *)1L, 4, 32, 0, 0, 1 },
	{ (WORD *)2L, 4, 32, 0, 0, 1 },
	{ (WORD *)3L, 4, 32, 0, 0, 1 }
};

LONG rs_frimg[] = {
	0
};

ICONBLK rs_iconblk[] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

TEDINFO rs_tedinfo[] = {
	{ (BYTE *)0L, (BYTE *)1L, (BYTE *)2L, 3, 6, 2, 0x1180, 0x0, -1, 5,1 },
	{ (BYTE *)4L, (BYTE *)5L, (BYTE *)6L, 3, 6, 2, 0x1180, 0x0, -1, 5,1 },
	{ (BYTE *)8L, (BYTE *)9L, (BYTE *)10L, 3, 6, 2, 0x1180, 0x0, -2, 14,1 },
	{ (BYTE *)15L, (BYTE *)16L, (BYTE *)17L, 3, 6, 2, 0x1180, 0x0, -1, 12,1 },
	{ (BYTE *)18L, (BYTE *)19L, (BYTE *)20L, 3, 6, 2, 0x1180, 0x0, -1, 3,1 },
	{ (BYTE *)21L, (BYTE *)22L, (BYTE *)23L, 3, 6, 2, 0x1180, 0x0, -1, 12,1 },
	{ (BYTE *)24L, (BYTE *)25L, (BYTE *)26L, 3, 6, 2, 0x1180, 0x0, -1, 3,1 },
	{ (BYTE *)28L, (BYTE *)29L, (BYTE *)30L, 3, 6, 0, 0x1180, 0x0, -1, 5,6 },
	{ (BYTE *)31L, (BYTE *)32L, (BYTE *)33L, 3, 6, 0, 0x1180, 0x0, -1, 9,11 },
	{ (BYTE *)43L, (BYTE *)44L, (BYTE *)45L, 3, 6, 0, 0x1180, 0x0, -1, 9,11 }
};

OBJECT rs_object[] = {
	{ -1, 1, 42, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x000c,0x0602, 0x0020,0x000b },
	{ 16, 2, 9, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x050e,0x0000, 0x0311,0x0e05 },
	{ 9, 3, 7, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0003, 0x0311,0x0e02 },
	{ 4, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x4FF1100L), 0x0400,0x0601, 0x0601,0x0201 },
	{ 5, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x3FF1100L), 0x020f,0x0601, 0x0601,0x0201 },
	{ 7, 6, 6, G_BOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0xFF1111L), 0x0302,0x0601, 0x060c,0x0201 },
	{ 5, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x0L), 0x0000,0x0000, 0x0704,0x0201 },
	{ 2, 8, 8, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0000, 0x0311,0x0001 },
	{ 7, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x3L), 0x0103,0x0000, 0x010b,0x0001 },
	{ 1, 10, 14, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0000, 0x0311,0x0e02 },
	{ 12, 11, 11, G_BOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0xFF1111L), 0x0302,0x0701, 0x060c,0x0201 },
	{ 10, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x1L), 0x0000,0x0000, 0x0704,0x0201 },
	{ 13, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x3FF1100L), 0x020f,0x0701, 0x0601,0x0201 },
	{ 14, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x4FF1100L), 0x0400,0x0701, 0x0601,0x0201 },
	{ 9, 15, 15, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0000, 0x0311,0x0001 },
	{ 14, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x7L), 0x0102,0x0000, 0x000d,0x0001 },
	{ 20, 17, 19, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0901, 0x0007,0x0602 },
	{ 19, 18, 18, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0700,0x0200, 0x0205,0x0102 },
	{ 17, -1, -1, G_IMAGE, OF_NONE, OS_NORMAL, C_UNION(0x0L), 0x0100,0x0400, 0x0105,0x0c01 },
	{ 16, -1, -1, G_IBOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x1100L), 0x0000,0x0000, 0x0007,0x0602 },
	{ 24, 21, 23, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0207,0x0901, 0x0107,0x0602 },
	{ 23, 22, 22, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0700,0x0200, 0x0205,0x0102 },
	{ 21, -1, -1, G_IMAGE, OF_NONE, OS_NORMAL, C_UNION(0x1L), 0x0500,0x0100, 0x0004,0x0002 },
	{ 20, -1, -1, G_IBOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x1100L), 0x0000,0x0000, 0x0107,0x0602 },
	{ 34, 25, 33, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x050e,0x0006, 0x0311,0x0503 },
	{ 27, 26, 26, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0e01, 0x0311,0x0701 },
	{ 25, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x2L), 0x0101,0x0300, 0x020f,0x0001 },
	{ 33, 28, 32, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0x1100L), 0x0002,0x0100, 0x030d,0x0a01 },
	{ 29, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x30FF1100L), 0x0601,0x0300, 0x0501,0x0301 },
	{ 30, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x31FF1100L), 0x0004,0x0300, 0x0401,0x0301 },
	{ 31, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x32FF1100L), 0x0106,0x0300, 0x0401,0x0301 },
	{ 32, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x33FF1100L), 0x0208,0x0300, 0x0401,0x0301 },
	{ 27, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x34FF1100L), 0x030a,0x0300, 0x0401,0x0301 },
	{ 24, -1, -1, G_IMAGE, OF_NONE, OS_NORMAL, C_UNION(0x2L), 0x030e,0x0300, 0x0003,0x0b01 },
	{ 35, -1, -1, G_IMAGE, OF_NONE, OS_NORMAL, C_UNION(0x3L), 0x070d,0x0306, 0x0004,0x0002 },
	{ 40, 36, 38, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0709, 0x0020,0x0901 },
	{ 37, -1, -1, G_BUTTON, 0x5, OS_NORMAL, C_UNION(0xBL), 0x0117,0x0400, 0x0008,0x0001 },
	{ 38, -1, -1, G_BUTTON, 0x7, OS_NORMAL, C_UNION(0xCL), 0x060c,0x0400, 0x0008,0x0001 },
	{ 35, 39, 39, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0000, 0x070a,0x0901 },
	{ 38, -1, -1, G_BUTTON, 0x5, OS_NORMAL, C_UNION(0xDL), 0x0501,0x0400, 0x0008,0x0001 },
	{ 42, 41, 41, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0000, 0x030e,0x0701 },
	{ 40, -1, -1, G_BUTTON, 0x45, OS_NORMAL, C_UNION(0xEL), 0x0502,0x0300, 0x0109,0x0001 },
	{ 0, 43, 46, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0104, 0x030e,0x0405 },
	{ 46, 44, 45, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0000, 0x030e,0x0a02 },
	{ 45, -1, -1, G_BOXTEXT, OF_NONE, OS_NORMAL, C_UNION(0x3L), 0x0000,0x0000, 0x030e,0x0001 },
	{ 43, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_SHADOWED, C_UNION(0x4L), 0x0702,0x0401, 0x0408,0x0001 },
	{ 42, 47, 48, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0c02, 0x030e,0x0802 },
	{ 48, -1, -1, G_BOXTEXT, OF_NONE, OS_NORMAL, C_UNION(0x5L), 0x0000,0x0000, 0x030e,0x0001 },
	{ 46, -1, -1, G_BOXTEXT, 0x60, OS_SHADOWED, C_UNION(0x6L), 0x0702,0x0301, 0x0408,0x0001 },
	{ -1, 1, 1, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0000,0x0000, 0x0020,0x000b },
	{ 0, 2, 3, G_BOX, OF_NONE, OS_OUTLINED, C_UNION(0x11100L), 0x0301,0x0201, 0x011d,0x0b08 },
	{ 3, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x1BL), 0x0006,0x0000, 0x0011,0x0001 },
	{ 1, 4, 16, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0100,0x0201, 0x071c,0x0807 },
	{ 5, -1, -1, G_FTEXT, OF_NONE, OS_NORMAL, C_UNION(0x7L), 0x0615,0x0000, 0x0305,0x0301 },
	{ 6, -1, -1, G_FTEXT, OF_NONE, OS_NORMAL, C_UNION(0x8L), 0x0610,0x0301, 0x000a,0x0001 },
	{ 13, 7, 12, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0600,0x0702, 0x031b,0x0003 },
	{ 8, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x22L), 0x0001,0x0000, 0x000c,0x0001 },
	{ 9, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x23L), 0x0011,0x0000, 0x000a,0x0001 },
	{ 10, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x24L), 0x0001,0x0001, 0x000c,0x0001 },
	{ 11, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x25L), 0x0011,0x0001, 0x000a,0x0001 },
	{ 12, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x26L), 0x0001,0x0002, 0x000f,0x0001 },
	{ 6, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x27L), 0x0011,0x0002, 0x000a,0x0001 },
	{ 14, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x28L), 0x0501,0x0200, 0x000e,0x0001 },
	{ 15, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x29L), 0x0501,0x0301, 0x0009,0x0001 },
	{ 16, -1, -1, G_BUTTON, 0x7, OS_NORMAL, C_UNION(0x2AL), 0x030a,0x0006, 0x0008,0x0001 },
	{ 3, -1, -1, G_FTEXT, 0xA0, OS_NORMAL, C_UNION(0x9L), 0x060f,0x0301, 0x000a,0x0001 }
};

_LONG_PTR rs_trindex[] = {
	0L,
	49L
};

#ifndef __foobar_defined
#define __foobar_defined 1
struct foobar {
	WORD 	dummy;
	WORD 	*image;
};
#endif
struct foobar rs_imdope[] = {
	{ 0, &IMAG0[0] },
	{ 0, &IMAG1[0] },
	{ 0, &IMAG2[0] },
	{ 0, &IMAG3[0] }
};



#define NUM_STRINGS 46
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 4
#define NUM_BB 4
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 10
#define NUM_OBS 66
#define NUM_TREE 2

BYTE pname[] = "USA.RSC";
