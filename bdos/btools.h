/*  btools.h - header file for routines which call the block oriented tools */


#define	bzero( s , n ) 		bfill( s , 0 , n )


VOID	bmove PROTO((const VOIDPTR src, VOIDPTR dst, uint16_t cnt));
VOID	lbmove PROTO((const VOIDPTR src, VOIDPTR dst, uint32_t cnt));
VOID	bfill PROTO((VOIDPTR dst, uint8_t val, uint16_t size));
