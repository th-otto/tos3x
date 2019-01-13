/*
 * TP_01: KILL_RESET
 * Omit RESET instruction for RAM-TOS on older ST's
 */
#ifndef TP_01
#define TP_01 (RAMVERSION)
#endif

/*
 * TP_02: COLOR_60HZ
 * Switch to 60Hz on color monitors
 */
#ifndef TP_02
#define TP_02 0
#endif

/*
 * TP_03: RAMRESET
 * Modified reset routine.
 * Will keep RAM-TOS resident even after Hardreset (via keyboard)
 */
#ifndef TP_03
#define TP_03 (RAMVERSION)
#endif
