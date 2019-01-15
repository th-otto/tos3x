#if BINEXACT
#undef TP_01
#undef TP_02
#undef TP_03
#undef TP_04
#undef TP_05
#undef TP_06
#undef TP_07
#undef TP_08
#undef TP_09
#undef TP_10
#undef TP_11
#undef TP_12
#undef TP_13
#undef TP_14
#undef TP_15
#undef TP_16
#undef TP_17
#undef TP_18
#undef TP_19
#undef TP_20
#undef TP_21
#undef TP_22
#undef TP_23
#undef TP_24
#undef TP_25
#undef TP_26
#undef TP_27
#undef TP_28
#undef TP_29
#undef TP_30
#undef TP_31
#undef TP_32
#undef TP_33
#undef TP_34
#undef TP_35
#undef TP_36
#undef TP_37
#undef TP_38
#undef TP_39

#define TP_01 0
#define TP_02 0
#define TP_03 0
#define TP_04 0
#define TP_05 (OS_COUNTRY == CTRY_PL)
#define TP_06 0
#define TP_07 0
#define TP_08 0
#define TP_09 0
#define TP_10 0
#define TP_11 0
#define TP_12 0
#define TP_13 (OS_COUNTRY == CTRY_PL)
#define TP_14 0
#define TP_15 (OS_COUNTRY == CTRY_PL)
#define TP_16 0
#define TP_17 0
#define TP_18 0
#define TP_19 (OS_COUNTRY == CTRY_PL)
#define TP_20 0
#define TP_21 0
#define TP_22 0
#define TP_23 0
#define TP_24 0
#define TP_25 0
#define TP_26 0
#define TP_27 0
#define TP_28 0
#define TP_29 0
#define TP_30 0
#define TP_31 0
#define TP_32 0
#define TP_33 0
#define TP_34 0
#define TP_35 0
#define TP_36 0
#define TP_37 0
#define TP_38 0
#define TP_39 0

#endif

/*
 * make sure the defaults are used for patches that are not activated
 */
#if !TP_07
#undef STEP_RATE
#endif
#if !TP_08
#undef FDC_COOKIE
#endif
#if !TP_09 & !TP_10
#undef BOOT_TIME
#endif
#if !TP_20
#undef PRNTIMEOUT
#endif
#if !TP_21
#undef CONTERM
#endif

/*
 * These are the defaults that are used for patches that are not activated
 */
#ifndef STEP_RATE
#define STEP_RATE 3
#endif
#ifndef FDC_COOKIE
/* FDC_1ATC */
#define FDC_COOKIE $01415443
#endif
#ifndef BOOT_TIME
#define BOOT_TIME 80
#endif
#ifndef PRNTIMEOUT
#define PRNTIMEOUT 30
#endif
#ifndef CONTERM
#define CONTERM 7
#endif
