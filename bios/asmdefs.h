/*
 * asmdefs.h - definitions to include in top of all assembler files
 *
 * Copyright (C) 2001-2015 The EmuTOS development team
 *
 * Authors:
 *  LVL   Laurent Vogel
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */

/* this symbol will be defined in all ASM source.
 * use it to filter out definitions that would generate syntax
 * errors in asm.
 */
#define ASM_SOURCE

/* generate constants for opcodes that are 68010 and above */
#define MOVEW_CCR_D0        dc.w $42c0            /* 68010-68060 */

#define MOVEC_CACR_D0       dc.l $4e7a0002        /* 68020-68060 */
#define MOVEC_CACR_D1       dc.l $4e7a1002

#define MOVEC_PCR_D0        dc.l $4e7a0808        /* 68060 */
#define MOVEC_PCR_D1        dc.l $4e7a1808

#define MOVEC_D0_CACR       dc.l $4e7b0002        /* 68020-68060 */
#define MOVEC_D1_CACR       dc.l $4e7b1002
#define MOVEC_D2_CACR       dc.l $4e7b2002

#define MOVEC_D0_VBR        dc.l $4e7b0801        /* 68010-68060 */
#define MOVEC_D0_CAAR       dc.l $4e7b0802        /* 68020-68030 */
#define MOVEC_D0_PCR        dc.l $4e7b0808        /* 68060 */

#define MOVEC_D0_TC         dc.l $4e7b0003        /* 68040-68060 (except 68ec040) */
#define MOVEC_D0_ITT0       dc.l $4e7b0004        /* 68040-68060 */
#define MOVEC_D0_ITT1       dc.l $4e7b0005        /* 68040-68060 */
#define MOVEC_D0_DTT0       dc.l $4e7b0006        /* 68040-68060 */
#define MOVEC_D0_DTT1       dc.l $4e7b0007        /* 68040-68060 */

#define PMOVE_FROM_TC(addr) dc.l $f0394200,addr   /* 68030 (except 68ec030) */

#define PMOVE_TO_TC(addr)   dc.l $f0394000,addr   /* 68030 (except 68ec030) */
#define PMOVE_TO_CRP(addr)  dc.l $f0394c00,addr   /* 68030 (except 68ec030) */
#define PMOVE_TO_TTR0(addr) dc.l $f0390800,addr   /* 68030 */
#define PMOVE_TO_TTR1(addr) dc.l $f0390c00,addr   /* 68030 */

#define FNOP                dc.l $f2800000        /* 6888X, 68040-68060 */
#define FSAVE_MINUS_SP      dc.w $f327            /* 6888X, 68040-68060 */
#define FRESTORE            dc.w $f35f            /* 6888X, 68040-68060 */

#define CINVA_IC            dc.w $f498            /* 68040 */
