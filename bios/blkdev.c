#include "bios.h"


/* 306de: 00e055da */
VOID bhdv_init(NOTHING)
{
}


/* 306de: 00e0565c */
ERROR bhdv_getbpb(P(int16_t) dev)
PP(int16_t dev;)
{
	return 0;
}


/* 306de: 00e058ae */
ERROR bhdv_mediach(P(int16_t) dev)
PP(int16_t dev;)
{
	return 0;
}


/* 306de: 00e05a96 */
ERROR bhdv_rwabs(P(int16_t) rw, P(char *) buf, P(int16_t) cnt, P(RECNO) recnr, P(int16_t) dev, P(LRECNO) lrecnr)
PP(int16_t rw;)
PP(char *buf;)
PP(int16_t cnt;)
PP(RECNO recnr;)
PP(int16_t dev;)
PP(LRECNO lrecnr;)
{
	return 0;
}


/* 306de: 00e05d88 */
ERROR bhdv_boot(NOTHING)
{
	return 0;
}


