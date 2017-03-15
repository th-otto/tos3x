/*
 * STACK variable summary:
 *	-1=keep all
 *	 0=keep MINSTACK bytes
 *	 1=keep 1/4 of free memory
 *	 2=keep 2/4
 *	 3=keep 3/4
 *    other=keep that many bytes (positive) or give back that many (negative)
 */

long _stksize = 0;
