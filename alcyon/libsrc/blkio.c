/***************************************************************************
 *									    
 *			   _ b l k i o   F u n c t i o n		    
 *			   -----------------------------		    
 *	Copyright 1982,1983 by Digital Research Inc.  All rights reserved.  
 *									    
 *	The _blkio function is used to read / write a set of contiguous	    
 *	sectors in an open file.					    
 *	Note: if CPM, it will use CP/M function 44 (Set Multi-Sector Count) 
 *	if os_multisectorio is TRUE (BDOS versions 3.0 and above).	    
 *	Note 2: if PC-DOS, it will use block read and write functions.	    
 *									    
 *	Edits:								    
 *		8-Dec-83 whf	handle PC-DOS changes.			    
 *									    
 *	Calling Sequence:						    
 *		ret = _blkio(ccbp,sector,buffer,count,bdosfunc);	    
 *									    
 *	Where:								    
 *		ret	=  sectors actually read/written		    
 *		ccbp	-> the open file channel control block		    
 *		sector	=  The desired starting sector number		    
 *		buffer  =  The memory address to begin transfer		    
 *		count	=  the number of sectors to read/write		    
 *		bdosfunc=  The BDOS function number (B_READ / B_WRITE)	    
 *									    
 ****************************************************************************/

#include	<osif.h>
#include	"lib.h"

long _blkio(P(FD *) ccbp, P(long) sector, P(VOIDPTR) buffer, P(long) count, P(int) bdosfunc)
PP(FD *ccbp;)								/* open file pointer        */
PP(long sector;)							/* Sector number        */
PP(VOIDPTR buffer;)							/* User's buffer area       */
PP(long count;)								/* # of sectors to do       */
PP(int bdosfunc;)							/* BDOS function to use     */
{
	if (bdosfunc == B_READ)
		return _pc_readblk(ccbp, sector, buffer, count * SECSIZ);
	return _pc_writeblk(ccbp, sector, buffer, count * SECSIZ);
}
