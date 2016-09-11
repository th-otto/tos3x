#ifndef PD_H
#define PD_H 1

#ifndef __OSTRUCT_H__
#include <ostruct.h>
#endif

#define NUMSTD      6       /* number of standard files */

/*
 *  PD - Process Descriptor
 */

#define PDCLSIZE    0x80    /*  size of command line in bytes  */
#define NUMCURDIR   26      /* number of entries in curdir array */

typedef struct _pd PD;
struct _pd
{
	/*   0 */ int32_t    p_lowtpa;	   /* pointer to start of TPA */
	/*   4 */ int32_t    p_hitpa; 	   /* pointer to end of TPA+1 */
	/*   8 */ int32_t    p_tbase; 	   /* pointer to base of text segment */
	/*  12 */ int32_t    p_tlen;		   /* length of text segment */
	/*  16 */ int32_t    p_dbase; 	   /* pointer to base of data segment */
	/*  20 */ int32_t    p_dlen;		   /* length of data segment */
	/*  24 */ int32_t    p_bbase; 	   /* pointer to base of bss segment */
	/*  28 */ int32_t    p_blen;		   /* length of bss segment */
	/*  32 */ DTA 	*p_xdta;
	/*  36 */ PD		*p_parent;		/* parent PD */
	/*  40 */ int16_t p_flags;
	/*  42 */ int16_t p_0fill[1];
	/*  44 */ char	*p_env; 		/* pointer to environment string */
	/*  48 */ int8_t	p_uft[NUMSTD];	/* index into sys file table for std files */
	/*  54 */ int8_t	p_lddrv;
	/*  55 */ int8_t	p_curdrv;
	/*  56 */ int32_t    p_1fill[2];
	/*  64 */ int8_t	  p_curdir[NUMCURDIR];	  /* index into sys dir table */
	/*  90 */ int8_t	p_2fill[32-NUMCURDIR];
	/*  96 */ int32_t    p_3fill[2];
	/* 104 */ int32_t    p_dreg[1];	   /* dreg[0] */
	/* 108 */ int32_t    p_areg[5];	   /* areg[3..7] */
	/* 128 */ char	p_cmdlin[PDCLSIZE]; 	/* command line image */
	/* 256 */
};

/* p_flags values: */
#define PF_FASTLOAD     0x0001
#define PF_TTRAMLOAD    0x0002
#define PF_TTRAMMEM     0x0004

extern PD *run; /* Pointer to the basepage of the current process */

#endif /* PD_H */
