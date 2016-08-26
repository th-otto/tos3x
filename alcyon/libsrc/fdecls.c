/****************************************************************************
*
*		D a t a   D e c l a r a t i o n s   M o d u l e
*		-----------------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Herein lie the data definitions for the stdio.h functions.
*
*****************************************************************************/

#include "lib.h"

#if MAXFILES != 16
compile snafu:fix following table
#endif
struct _iobuf _iob[16] = {
	{  0, _IOREAD, NULL, NULL, 0 },
	{  1, _IOWRT + _IONBUF, NULL, NULL, 0 },
	{  2, _IOWRT + _IONBUF, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 },
	{ -1, 0, NULL, NULL, 0 }
};
