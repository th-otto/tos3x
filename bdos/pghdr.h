/*  pghdr.h - header file for program loaders, 'size' pgms, etc.	*/
/*  PGMHDR01, PGMINFO */




#define	PGMHDR01	struct	pgmhdr01
PGMHDR01
{
	/*  magic number is already read  */
	int32_t	h01_tlen ;	/*  length of text segment		*/
	int32_t	h01_dlen ;	/*  length of data segment		*/
	int32_t	h01_blen ;	/*  length of bss  segment		*/
	int32_t	h01_slen ;	/*  length of symbol table		*/
	int32_t	h01_res1 ;	/*  reserved - always zero		*/
	int32_t	h01_res2 ;	/*  ?					*/
	int16_t	h01_abs ;	/*  not zero if no relocation		*/
};

#define	PGMINFO		struct	pgminfo
PGMINFO
{
	int32_t	pi_tpalen ;		/*  length of tpa area		*/
	char	*pi_tbase ;		/*  start addr of text seg	*/
	int32_t	pi_tlen ;		/*  length of text seg		*/
	char	*pi_dbase ;		/*  start addr of data seg	*/
	int32_t	pi_dlen ;		/*  length of data seg		*/
	char	*pi_bbase ;		/*  start addr of bss  seg	*/
	int32_t	pi_blen ;		/*  length of bss  seg		*/
	int32_t	pi_slen ;		/*  length of symbol table	*/
};

