/*
 *	Seek.c -- Whitesmith's equivalent of V6 seek
 */

#include "lib.h"
#include "klib.h"


int xseek(P(int) fildes, P(int) offset, P(int) whence)
PP(int fildes;)								/* UNIX / WS file descriptor    */
PP(int offset;)								/* File offset, bytes       */
PP(int whence;)							/* Sense variable       */
{
	offset &= 0xffff;					/* Clear sign extension problems */
	if (whence > 2)					/* Not byte sense seek      */
	{
		offset = offset << 9;			/* Multiply by 512      */
		whence -= 3;					/* reset to range 0 .. 2    */
	}
	return lseek(fildes, (long) offset, whence);
}
