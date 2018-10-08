/*
 * GEM resource C output of it
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
#define FREEBB 0
#define FREEIMG 2
#define FREESTR 21

BYTE *rs_strings[] = {
	(BYTE *)"01234567.123",
	(BYTE *)"0123456789012345",
	(BYTE *)"________________",
	(BYTE *)"XXXXXXXXXXXXXXXX",
	(BYTE *)"Opzioni CPX",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Colore testo:",
	(BYTE *)"Colore icona:",
	(BYTE *)"00",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"00",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Residente in RAM:",
	(BYTE *)"Si",
	(BYTE *)"No",
	(BYTE *)"Salva",
	(BYTE *)"OK",
	(BYTE *)"Annulla"
};

static WORD IMAG0[] = {
0x0, 0x0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

static WORD IMAG1[] = {
0x0, 0x0, 0xFFFF, 0xFFFF, 0x8000, 0x1, 0x8000, 0x1, 
0x8007, 0xF001, 0x800F, 0xF801, 0x801F, 0xEC01, 0x8018, 0x401, 
0x8018, 0x401, 0x8010, 0x401, 0x801E, 0x3C01, 0x8017, 0x5401, 
0x8011, 0x401, 0x800B, 0x2801, 0x800D, 0xD801, 0x8006, 0x2801, 
0x8007, 0xD001, 0x802E, 0x1001, 0x8039, 0xE001, 0x8038, 0x1, 
0x8000, 0x1, 0x8000, 0x1, 0x8000, 0x1, 0xFFFF, 0xFFFF};

LONG rs_frstr[] = {
	0
};

BITBLK rs_bitblk[] = {
	{ 0, 0, 0, 0, 0, 0 }
};

LONG rs_frimg[] = {
	0
};

ICONBLK rs_iconblk[] = {
	{ (WORD *)0L, (WORD *)1L, (BYTE *)0L, 4096,21,18, 20,0,32,24, 0,24,72,8 }
};

TEDINFO rs_tedinfo[] = {
	{ (BYTE *)1L, (BYTE *)2L, (BYTE *)3L, 3, 6, 0, 0x1180, 0x0, -1, 17,17 },
	{ (BYTE *)4L, (BYTE *)5L, (BYTE *)6L, 3, 6, 2, 0x1180, 0x0, -1, 12,1 },
	{ (BYTE *)9L, (BYTE *)10L, (BYTE *)11L, 3, 6, 2, 0x1180, 0x0, -1, 3,1 },
	{ (BYTE *)12L, (BYTE *)13L, (BYTE *)14L, 3, 6, 2, 0x1180, 0x0, -1, 3,1 }
};

OBJECT rs_object[] = {
	{ -1, 1, 21, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0x1121L), 0x000c,0x0602, 0x0020,0x000b },
	{ 21, 2, 17, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1141L), 0x0100,0x0000, 0x061f,0x0609 },
	{ 5, 3, 4, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0302,0x0400, 0x071a,0x0202 },
	{ 4, -1, -1, G_ICON, OF_NONE, OS_NORMAL, C_UNION(0x0L), 0x0200,0x0200, 0x0009,0x0002 },
	{ 2, -1, -1, G_FTEXT, OF_EDITABLE, OS_NORMAL, C_UNION(0x0L), 0x020a,0x0900, 0x0010,0x0001 },
	{ 6, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x3FF1100L), 0x031d,0x0400, 0x0501,0x0202 },
	{ 7, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x4FF1100L), 0x0500,0x0400, 0x0501,0x0202 },
	{ 8, -1, -1, G_BOXTEXT, OF_NONE, OS_NORMAL, C_UNION(0x1L), 0x0500,0x0b02, 0x031e,0x0301 },
	{ 17, 9, 16, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0500,0x0204, 0x031e,0x0e02 },
	{ 10, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x7L), 0x0001,0x0300, 0x000d,0x0001 },
	{ 11, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x8L), 0x0001,0x0901, 0x000d,0x0001 },
	{ 12, -1, -1, G_BOXTEXT, OF_NONE, OS_NORMAL, C_UNION(0x2L), 0x0314,0x0400, 0x0604,0x0001 },
	{ 13, -1, -1, G_BOXTEXT, OF_NONE, OS_NORMAL, C_UNION(0x3L), 0x0314,0x0a01, 0x0604,0x0001 },
	{ 14, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x3FF1100L), 0x0219,0x0400, 0x0202,0x0001 },
	{ 15, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x3FF1100L), 0x0219,0x0a01, 0x0202,0x0001 },
	{ 16, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x4FF1100L), 0x0012,0x0400, 0x0202,0x0001 },
	{ 8, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x4FF1100L), 0x0012,0x0a01, 0x0202,0x0001 },
	{ 1, 18, 20, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0500,0x0507, 0x031e,0x0a01 },
	{ 19, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0xFL), 0x0001,0x0400, 0x0011,0x0001 },
	{ 20, -1, -1, G_BUTTON, 0x11, OS_NORMAL, C_UNION(0x10L), 0x0113,0x0500, 0x0504,0x0f00 },
	{ 17, -1, -1, G_BUTTON, 0x11, OS_NORMAL, C_UNION(0x11L), 0x0119,0x0500, 0x0304,0x0f00 },
	{ 0, 22, 25, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0100,0x0609, 0x061f,0x0a01 },
	{ 24, 23, 23, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0000, 0x000b,0x0a01 },
	{ 22, -1, -1, G_BUTTON, 0x5, OS_NORMAL, C_UNION(0x12L), 0x0401,0x0500, 0x0008,0x0001 },
	{ 25, -1, -1, G_BUTTON, 0x7, OS_NORMAL, C_UNION(0x13L), 0x040c,0x0500, 0x0008,0x0001 },
	{ 21, -1, -1, G_BUTTON, 0x25, OS_NORMAL, C_UNION(0x14L), 0x0416,0x0500, 0x0008,0x0001 }
};

_LONG_PTR rs_trindex[] = {
	0L
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
	{ 0, &IMAG1[0] }
};



#define NUM_STRINGS 21
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 2
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 1
#define NUM_CIB 0
#define NUM_TI 4
#define NUM_OBS 26
#define NUM_TREE 1

BYTE pname[] = "IT.RSC";
