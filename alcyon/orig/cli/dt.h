/* dt.h - common definitions for DATE and TIME utilities
*/

#define	C_ConOut	0x02
#define	C_ConWS		0x09
#define	T_GetDate	0x2A
#define	T_SetDate	0x2B
#define	T_GetTime	0x2C
#define	T_SetTime	0x2D
#define	P_Term		0x4c

long bdos PROTO((short code, ...));
int get_num PROTO((char **p));
VOID put_num PROTO((unsigned int i));
