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
/* 0x00 */
	int32_t    p_lowtpa;	   /* pointer to start of TPA */
	int32_t    p_hitpa; 	   /* pointer to end of TPA+1 */
	int32_t    p_tbase; 	   /* pointer to base of text segment */
	int32_t    p_tlen;		   /* length of text segment */
/* 0x10 */
	int32_t    p_dbase; 	   /* pointer to base of data segment */
	int32_t    p_dlen;		   /* length of data segment */
	int32_t    p_bbase; 	   /* pointer to base of bss segment */
	int32_t    p_blen;		   /* length of bss segment */
/* 0x20 */
	DTA 	*p_xdta;
	PD		*p_parent;		/* parent PD */
	int16_t p_flags;
	int16_t p_0fill[1];
	char	*p_env; 		/* pointer to environment string */
/* 0x30 */
	int8_t	p_uft[NUMSTD];	/* index into sys file table for std files */
	int8_t	p_lddrv;
	int8_t	p_curdrv;
	int32_t    p_1fill[2];
/* 0x40 */
	int8_t	  p_curdir[NUMCURDIR];	  /* index into sys dir table */
	int8_t	p_2fill[32-NUMCURDIR];
/* 0x60 */
	int32_t    p_3fill[2];
	int32_t    p_dreg[1];	   /* dreg[0] */
	int32_t    p_areg[5];	   /* areg[3..7] */
/* 0x80 */
	char	p_cmdlin[PDCLSIZE]; 	/* command line image */
};

/* p_flags values: */
#define PF_FASTLOAD     0x0001
#define PF_TTRAMLOAD    0x0002
#define PF_TTRAMMEM     0x0004

extern PD *run; /* Pointer to the basepage of the current process */

#endif /* PD_H */
