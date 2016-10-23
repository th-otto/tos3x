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
