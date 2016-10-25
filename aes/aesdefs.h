#if TOSVERSION >= 0x0100
#undef AESVERSION
#define AESVERSION 0x0120
#endif

#if TOSVERSION >= 0x0104
#undef AESVERSION
#define AESVERSION 0x0140
#endif

#if TOSVERSION >= 0x0201
#undef AESVERSION
#define AESVERSION 0x0310
#endif

#if TOSVERSION >= 0x0206
#undef AESVERSION
#define AESVERSION 0x0320
#endif

#if TOSVERSION >= 0x0301
#undef AESVERSION
#define AESVERSION 0x0300
#endif

#if TOSVERSION >= 0x0306
#undef AESVERSION
#define AESVERSION 0x0320
#endif

#ifndef MULTITOS
#define MULTITOS 0
#endif

#if TOSVERSION >= 0x0400
#undef AESVERSION
#if MULTITOS
#define AESVERSION 0x0340
#else
#define AESVERSION 0x0330
#endif
#endif

#define NUM_ACCS 1			/* for atari in rom		*/
#define NUM_PDS (NUM_ACCS + 2)		/* acc's + ctrlpd + dos appl.	*/
#define NUM_EVBS (NUM_PDS * 5)		/* 5 * the number of PDs	*/
#define MAX_ACCS 6			/* for atari, from disk		*/
#define EVB_PROC 5			/* for atari, number of EVB per process */

#define KBD_SIZE 8
#define QUEUE_SIZE 256
#define NFORKS 96

#define CARTRIDGE	1	/* if rom cartridge exists */

#define UNLINKED	0	/* if desktop not linked with gem */

#define DOWARNING	0	/* Do we need initial warning box ?  */

#ifndef BINEXACT
#define BINEXACT 1
#endif

#ifndef MULTILANG_SUPPORT
#define MULTILANG_SUPPORT (OS_COUNTRY == OS_CONF_MULTILANG)
#endif

#define SUBMENUS (AESVERSION >= 0x340)
#define AES3D (AESVERSION >= 0x340)

#define STACK_SIZE 500
