/* dt.h - common definitions for DATE and TIME utilities
*/

typedef struct basep {
    char	*p_lowtpa;	/* pointer to self (bottom of TPA) */
    char	*p_hitpa;	/* pointer to top of TPA + 1 */
    char	*p_tbase;	/* base of text segment */
    long	p_tlen;		/* length of text segment */
    char	*p_dbase;	/* base of data segment */
    long	p_dlen;		/* length of data segment */
    char	*p_bbase;	/* base of BSS segment */
    long	p_blen;		/* length of BSS segment */
    char	*p_dta;		/* (UNOFFICIAL, DON'T USE) */
    struct basep *p_parent;	/* pointer to parent's basepage */
    long	p_flags;	/* memory usage flags */
    char	*p_env;		/* pointer to environment string */
    char	p_devx[6];		/* real handles of the standard devices */
    char    p_resrvd1;		/* reserved */
    char    p_defdrv;		/* default drv */
    long	p_undef[17];	/* scratch area... don't touch */
	long	p_usp;			/* a fake USP to make dLibs programs happy */
    char	p_cmdlin[128];	/* command line image */
} BASEPAGE;

long bdos PROTO((short code, ...));
int get_num PROTO((char **p));
VOID put_num PROTO((unsigned int i));
