#if TOSVERSION >= 0x100
#undef AESVERSION
#define AESVERSION 0x120
#endif

#if TOSVERSION >= 0x104
#undef AESVERSION
#define AESVERSION 0x140
#endif

#if TOSVERSION >= 0x201
#undef AESVERSION
#define AESVERSION 0x310
#endif

#if TOSVERSION >= 0x206
#undef AESVERSION
#define AESVERSION 0x320
#endif

#if TOSVERSION >= 0x301
#undef AESVERSION
#define AESVERSION 0x300
#endif

#if TOSVERSION >= 0x306
#undef AESVERSION
#define AESVERSION 0x320
#endif

#ifndef MULTITOS
#define MULTITOS 0
#endif

#if TOSVERSION >= 0x400
#undef AESVERSION
#if MULTITOS
#define AESVERSION 0x340
#else
#define AESVERSION 0x330
#endif
#endif

#define NUM_ACCS 1			/* for atari in rom		*/
#define NUM_PDS (NUM_ACCS + 2)		/* acc's + ctrlpd + dos appl.	*/
#define NUM_EVBS (NUM_PDS * 5)		/* 5 * the number of PDs	*/
#define MAX_ACCS 6			/* for atari, from disk		*/
#define EVB_PROC 5			/* for atari, number of EVB per process */

#define KBD_SIZE 8

#define CARTRIDGE	1	/* if rom cartridge exists */

#define UNLINKED	0	/* if desktop not linked with gem */

#define DOWARNING	0	/* Do we need initial warning box ?  */

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

#define GEM3 0

#if AESVERSION >= 0x330
#define	MAXOBJ	    20		/* maximum number of objects in a window */
#else
#define	MAXOBJ	    19		/* maximum number of objects in a window */
#endif

#define	NUM_WIN	8		/* # window structures per block of memory */

#if AESVERSION >= 0x330
#define STACK_SIZE  500
#define STACK2_SIZE STACK_SIZE
#define STACK3_SIZE STACK_SIZE
#define NFORKS      96
#define QUEUE_SIZE 256
#else
#define STACK_SIZE  448
#define STACK2_SIZE 300
#define STACK3_SIZE 350
#define NFORKS      32
#define QUEUE_SIZE 128
#endif

#if TP_WINX
#define WX_VERSION    0x220
#define WX_DATE       0x1b51 /* 1993/10/17 */

#define WX_QUEUE_SIZE 0x500
#define WX_MAXWIN     40
#endif

#define CMDLEN 128
