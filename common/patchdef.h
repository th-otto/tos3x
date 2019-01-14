/*
 * Defaults which patches should be applied.
 * This is only for reference.
 * DO NOT EDIT THIS - create a file common/localcnf.h instead
 * with your preferred settings
 */
 
/*
 * TP_01: KILL_RESET: Omit RESET instruction for RAM-TOS on older ST's
 */
#ifndef TP_01
#define TP_01 (RAMVERSION)
#endif

/*
 * TP_02: COLOR_60HZ: Switch to 60Hz on color monitors
 */
#ifndef TP_02
#define TP_02 0
#endif

/*
 * TP_03: RAMRESET: Modified reset routine.
 * Will keep RAM-TOS resident even after Hardreset (via keyboard)
 */
#ifndef TP_03
#define TP_03 (RAMVERSION)
#endif

/*
 * TP_04: MEM_INIT: Clear memory from $100-$400
 * Clear the memory starting at $100 instead of $400 on reset,
 * to remove any junk.
 * This patch is disabled if the RAMRESET patch is also active.
 */
#ifndef TP_04
#define TP_04 0
#endif

/*
 * TP_05: SHELL_P: Clear _shell_p
 * Clear the _shell_p system variable on reset.
 */
#ifndef TP_05
#define TP_05 1
#endif

/*
 * TP_06: MSTE_CACHE_ON: Turn on 16Mhz for Mega-STE
 * Turns on 16 MHz and cache on Mega-STE already when booting
 */
#ifndef TP_06
#define TP_06 0
#endif

/*
 * TP_07: SET_STEP: Set steprate for floppy drives A: and B:
 * 0 = 6 ms
 * 1 = 12 ms
 * 2 = 2 ms
 * 3 = 3 ms
 */
#ifndef TP_07
#define TP_07 0
#define STEP_RATE 3
#endif
#if !TP_07
#undef STEP_RATE
#endif

/*
 * TP_08: SET_FDC_COOKIE: Install HD cookie
 *
 * Do not use this on Mega-STE/TT, because
 * there it will be autodetected
 *
 * Format: 0xkkkkkk / normal (Mega STE/TT): $01415443
 * kkkkkk: ID who installed the cookie.
 * x=0: normal density (DD 720kB/360kB)
 * x=1: high density (HD 1.44MB)
 * x=2: extra-high density (ED 2.88MB)
 * x>2: reserved
 */
#ifndef TP_08
#define TP_08 0
#define FDC_COOKIE $01415443
#endif
#if !TP_08
#undef FDC_COOKIE
#endif

/*
 * TP_09: SET_TIME_CRC: Change delay for CRC test
 * TP_10: SET_TIME_RAM: Change delay for RAM test
 * BOOT_TIME: time (in s) for above
 * TP_11: SKIP_WAIT: Skip display of ramtest bar
 * TP_12: SKIP_RAM: Show only amount of memory
 *
 * Reduce the delay after which - on system start -
 * a CRC test or a RAM test is run. Additionally, the
 * RAM test can be configured to display only the
 * memory configuration or the bar.
 */
#ifndef TP_09
#define TP_09 0
#endif
#ifndef TP_10
#define TP_10 0
#endif
#ifndef TP_11
#define TP_11 0
#endif
#ifndef TP_12
#define TP_12 0
#endif
#ifndef BOOT_TIME
#define BOOT_TIME 0
#endif

/*
 * TP_13: BOOT_DEV: Fix boot device error (see ST-Computer 1/90)
 */
#ifndef TP_13
#define TP_13 1
#endif

/*
 * TP_14: CT_HD: Boot function for c't "Billigloesung"
 */
#ifndef TP_14
#define TP_14 0
#endif

/*
 * TP_15: STACK_POINTER: Fix stack pointer in autoexec
 * Fixes a bug the autoexec routine (see ST-Computer 1/90)
 */
#ifndef TP_15
#define TP_15 1
#endif

/*
 * TP_16: PILZE: Alternative image for bombs:
 * Replaces the Atari bomb images with the original mushrooms
 */
#ifndef TP_16
#define TP_16 0
#endif

/*
 * TP_17: LOCK_CLOCK: Lock Mega-ST clock
 * 
 * Prevents the hardware clock from being set by TOS.
 * A separate program is then needed to update it.
 */
#ifndef TP_17
#define TP_17 0
#endif
