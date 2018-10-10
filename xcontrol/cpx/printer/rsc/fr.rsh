/*
 * GEM resource C output of fr
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
#define FREEIMG 0
#define FREESTR 27

BYTE *rs_strings[] = {
	(BYTE *)"Dot",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Imprimante:",
	(BYTE *)"B/W",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Couleur:",
	(BYTE *)"1280",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Pts/Ligne:",
	(BYTE *)"Draft",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Qualit\202:",
	(BYTE *)"Printer",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Sortie",
	(BYTE *)"Feed",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Papier",
	(BYTE *)"Sauve",
	(BYTE *)"CONFIRME",
	(BYTE *)"Annule"
};

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
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

TEDINFO rs_tedinfo[] = {
	{ (BYTE *)0L, (BYTE *)1L, (BYTE *)2L, 3, 6, 2, 0x1180, 0x0, -1, 4,1 },
	{ (BYTE *)4L, (BYTE *)5L, (BYTE *)6L, 3, 6, 2, 0x1180, 0x0, -1, 4,1 },
	{ (BYTE *)8L, (BYTE *)9L, (BYTE *)10L, 3, 6, 2, 0x1180, 0x0, -1, 5,1 },
	{ (BYTE *)12L, (BYTE *)13L, (BYTE *)14L, 3, 6, 2, 0x1180, 0x0, -1, 6,1 },
	{ (BYTE *)16L, (BYTE *)17L, (BYTE *)18L, 3, 6, 2, 0x1180, 0x0, -1, 8,1 },
	{ (BYTE *)20L, (BYTE *)21L, (BYTE *)22L, 3, 6, 2, 0x1180, 0x0, -1, 5,1 }
};

OBJECT rs_object[] = {
	{ -1, 1, 22, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0x1100L), 0x0000,0x0000, 0x0020,0x000b },
	{ 14, 2, 11, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0000, 0x0515,0x0b08 },
	{ 5, 3, 4, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0000, 0x0515,0x0202 },
	{ 4, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_SHADOWED, C_UNION(0x0L), 0x010c,0x0700, 0x0308,0x0301 },
	{ 2, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x3L), 0x0500,0x0900, 0x000b,0x0001 },
	{ 8, 6, 7, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0302, 0x0515,0x0202 },
	{ 7, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_SHADOWED, C_UNION(0x1L), 0x010c,0x0700, 0x0308,0x0301 },
	{ 5, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x7L), 0x0500,0x0900, 0x0008,0x0001 },
	{ 11, 9, 10, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0604, 0x0515,0x0202 },
	{ 10, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_SHADOWED, C_UNION(0x2L), 0x010c,0x0700, 0x0308,0x0301 },
	{ 8, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0xBL), 0x0500,0x0900, 0x000a,0x0001 },
	{ 1, 12, 13, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0906, 0x0515,0x0202 },
	{ 13, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_SHADOWED, C_UNION(0x3L), 0x010c,0x0700, 0x0308,0x0301 },
	{ 11, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0xFL), 0x0500,0x0900, 0x0008,0x0001 },
	{ 17, 15, 16, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0615,0x0000, 0x020a,0x0504 },
	{ 16, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_SHADOWED, C_UNION(0x4L), 0x0700,0x0302, 0x0308,0x0301 },
	{ 14, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x13L), 0x0102,0x0900, 0x0006,0x0001 },
	{ 20, 18, 19, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0615,0x0604, 0x020a,0x0504 },
	{ 19, -1, -1, G_BOXTEXT, OF_TOUCHEXIT, OS_SHADOWED, C_UNION(0x5L), 0x0700,0x0402, 0x0308,0x0301 },
	{ 17, -1, -1, G_STRING, OF_NONE, OS_NORMAL, C_UNION(0x17L), 0x0701,0x0900, 0x0306,0x0101 },
	{ 22, 21, 21, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0c08, 0x050a,0x0402 },
	{ 20, -1, -1, G_BUTTON, 0x45, OS_NORMAL, C_UNION(0x18L), 0x0201,0x0a00, 0x0008,0x0001 },
	{ 0, 23, 24, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x050a,0x0c08, 0x0315,0x0402 },
	{ 24, -1, -1, G_BUTTON, 0x7, OS_NORMAL, C_UNION(0x19L), 0x0301,0x0900, 0x0508,0x0101 },
	{ 22, -1, -1, G_BUTTON, 0x25, OS_NORMAL, C_UNION(0x1AL), 0x010c,0x0a00, 0x0008,0x0001 }
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
	{ 0, 0 }
};



#define NUM_STRINGS 27
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 6
#define NUM_OBS 25
#define NUM_TREE 1

BYTE pname[] = "FRENCH.RSC";
