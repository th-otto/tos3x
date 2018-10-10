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
#define T1OBJ 20
#define FREEBB 0
#define FREEIMG 0
#define FREESTR 36

BYTE *rs_strings[] = {
	(BYTE *)"012345678901234567890123456",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"012345678901234567890123456",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"012345678901234567890123456",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"012345678901234567890123456",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"012345678901234567890123456",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"012345678901234567890123456",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"012345678901234567890123456",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Add",
	(BYTE *)"Delete",
	(BYTE *)"Save",
	(BYTE *)"Exit",
	(BYTE *)"Environment String(s)",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"Modify Environment String",
	(BYTE *)"",
	(BYTE *)"",
	(BYTE *)"0123456789012345678901234567890123456789",
	(BYTE *)"STRING: ________________________________________",
	(BYTE *)"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	(BYTE *)"OK",
	(BYTE *)"Cancel"
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
	{ (BYTE *)0L, (BYTE *)1L, (BYTE *)2L, 3, 6, 0, 0x1180, 0x0, -1, 28,1 },
	{ (BYTE *)3L, (BYTE *)4L, (BYTE *)5L, 3, 6, 0, 0x1180, 0x0, -1, 28,1 },
	{ (BYTE *)6L, (BYTE *)7L, (BYTE *)8L, 3, 6, 0, 0x1180, 0x0, -1, 28,1 },
	{ (BYTE *)9L, (BYTE *)10L, (BYTE *)11L, 3, 6, 0, 0x1180, 0x0, -1, 28,1 },
	{ (BYTE *)12L, (BYTE *)13L, (BYTE *)14L, 3, 6, 0, 0x1180, 0x0, -1, 28,1 },
	{ (BYTE *)15L, (BYTE *)16L, (BYTE *)17L, 3, 6, 0, 0x1180, 0x0, -1, 28,1 },
	{ (BYTE *)18L, (BYTE *)19L, (BYTE *)20L, 3, 6, 0, 0x1180, 0x0, -1, 28,1 },
	{ (BYTE *)25L, (BYTE *)26L, (BYTE *)27L, 3, 6, 2, 0x1180, 0x0, -1, 22,1 },
	{ (BYTE *)28L, (BYTE *)29L, (BYTE *)30L, 3, 6, 2, 0x1180, 0x0, -1, 26,1 },
	{ (BYTE *)31L, (BYTE *)32L, (BYTE *)33L, 3, 6, 2, 0x1180, 0x0, -1, 41,49 }
};

OBJECT rs_object[] = {
	{ -1, 1, 19, G_BOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x1141L), 0x000c,0x0602, 0x0020,0x000b },
	{ 14, 2, 13, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0101,0x0a01, 0x061d,0x0007 },
	{ 10, 3, 9, G_BOX, OF_NONE, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0000, 0x001b,0x0007 },
	{ 4, -1, -1, G_TEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x0L), 0x0000,0x0000, 0x001b,0x0001 },
	{ 5, -1, -1, G_TEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x1L), 0x0000,0x0001, 0x001b,0x0001 },
	{ 6, -1, -1, G_TEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x2L), 0x0000,0x0002, 0x001b,0x0001 },
	{ 7, -1, -1, G_TEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x3L), 0x0000,0x0003, 0x001b,0x0001 },
	{ 8, -1, -1, G_TEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x4L), 0x0000,0x0004, 0x001b,0x0001 },
	{ 9, -1, -1, G_TEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x5L), 0x0000,0x0005, 0x001b,0x0001 },
	{ 2, -1, -1, G_TEXT, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x6L), 0x0000,0x0006, 0x001b,0x0001 },
	{ 12, 11, 11, G_BOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0xFF1111L), 0x011b,0x0101, 0x0502,0x0e04 },
	{ 10, -1, -1, G_BOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0xFF1100L), 0x0000,0x0202, 0x0502,0x0d00 },
	{ 13, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x1FF1100L), 0x011b,0x0000, 0x0502,0x0001 },
	{ 1, -1, -1, G_BOXCHAR, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0x2FF1100L), 0x011b,0x0006, 0x0502,0x0001 },
	{ 19, 15, 18, G_BOX, OF_TOUCHEXIT, OS_NORMAL, C_UNION(0xFF1100L), 0x0101,0x0f08, 0x061d,0x0b01 },
	{ 16, -1, -1, G_BUTTON, 0x5, OS_NORMAL, C_UNION(0x15L), 0x0500,0x0600, 0x0006,0x0001 },
	{ 17, -1, -1, G_BUTTON, 0x5, OS_NORMAL, C_UNION(0x16L), 0x0507,0x0600, 0x0307,0x0001 },
	{ 18, -1, -1, G_BUTTON, 0x5, OS_NORMAL, C_UNION(0x17L), 0x0010,0x0600, 0x0006,0x0001 },
	{ 14, -1, -1, G_BUTTON, 0x7, OS_NORMAL, C_UNION(0x18L), 0x0117,0x0600, 0x0006,0x0001 },
	{ 0, -1, -1, G_BOXTEXT, 0x60, OS_NORMAL, C_UNION(0x7L), 0x0101,0x0400, 0x061d,0x0201 },
	{ -1, 1, 4, G_BOX, OF_NONE, OS_OUTLINED, C_UNION(0x21100L), 0x0000,0x0000, 0x0036,0x0a06 },
	{ 2, -1, -1, G_BOXTEXT, OF_NONE, OS_SHADOWED, C_UNION(0x8L), 0x000c,0x0c00, 0x001e,0x0001 },
	{ 3, -1, -1, G_FBOXTEXT, OF_EDITABLE, OS_NORMAL, C_UNION(0x9L), 0x0002,0x0802, 0x0032,0x0001 },
	{ 4, -1, -1, G_BUTTON, 0x7, OS_NORMAL, C_UNION(0x22L), 0x040c,0x0804, 0x000b,0x0001 },
	{ 0, -1, -1, G_BUTTON, 0x25, OS_NORMAL, C_UNION(0x23L), 0x041f,0x0804, 0x000b,0x0001 }
};

_LONG_PTR rs_trindex[] = {
	0L,
	20L
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



#define NUM_STRINGS 36
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 10
#define NUM_OBS 25
#define NUM_TREE 2

BYTE pname[] = "EN.RSC";
