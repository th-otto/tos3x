/****************************************************************************
*
*		D a t a   D e c l a r a t i o n s   M o d u l e
*		-----------------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Herein lie the data definitions for the stdio.h functions.
*	Note: function __fdecls() is here so _main can force this module to
*	be loaded.
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
	{  3, 0, NULL, NULL, 0 },
	{  4, 0, NULL, NULL, 0 },
	{  5, 0, NULL, NULL, 0 },
	{  6, 0, NULL, NULL, 0 },
	{  7, 0, NULL, NULL, 0 },
	{  8, 0, NULL, NULL, 0 },
	{  9, 0, NULL, NULL, 0 },
	{ 10, 0, NULL, NULL, 0 },
	{ 11, 0, NULL, NULL, 0 },
	{ 12, 0, NULL, NULL, 0 },
	{ 13, 0, NULL, NULL, 0 },
	{ 14, 0, NULL, NULL, 0 },
	{ 15, 0, NULL, NULL, 0 }
};

VOID __fdecls(NOTHING)
{
}
