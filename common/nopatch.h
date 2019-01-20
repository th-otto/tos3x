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
#undef TP_35_6
#undef TP_35_8
#undef TP_35_16
#undef TP_35_32
#undef TP_36
#undef TP_37_1
#undef TP_37_2
#undef TP_37_3
#undef TP_37_4
#undef TP_37_5
#undef TP_37_6
#undef TP_37_7
#undef TP_37_8
#undef TP_37_9
#undef TP_37_10
#undef TP_37_11
#undef TP_37_12
#undef TP_38_0
#undef TP_38_1
#undef TP_38_2
#undef TP_38_3
#undef TP_38_4
#undef TP_38_5
#undef TP_38_6
#undef TP_38_7
#undef TP_39_1
#undef TP_39_2
#undef TP_39_3
#undef TP_40
#undef TP_41
#undef TP_42
#undef TP_43
#undef HD_WAIT
#undef HD_WAITTIME
#undef HD_WAITDEVICE
#undef TP_44
#undef TP_45
#undef TP_46
#undef TP_47
#undef TP_48
#undef TP_49

#undef SUPERTOS

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
#if OS_COUNTRY == CTRY_PL
#define TP_35_6 2
#define TP_35_8 2
#define TP_35_16 2
#else
#define TP_35_6 0
#define TP_35_8 0
#define TP_35_16 0
#endif
#define TP_35_32 0
#define TP_40 0
#define TP_41 0
#define TP_42 0
#define HD_WAIT 0
#define HD_WAITTIME 0
#define HD_WAITDEVICE 0
#define TP_46 0
#define TP_47 0
#define TP_48 0
#define TP_49 0

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
#if !TP_47
#undef SHBUFSIZE
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

#ifndef SUPERTOS
#define SUPERTOS 0
#endif
