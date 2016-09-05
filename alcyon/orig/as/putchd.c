#include "as68.h"
#include <ar68.h>

/*
 * putchd - fills the buffer from the c.out header structure in
 *		the byte orientation of the target machine (68000).
 */
VOID putchd(P(FILE *) fp, P(struct hdr *) arptr) /* XXX */
PP(FILE *fp;)
PP(struct hdr *arptr;)
{
	register int i;
	register short *p;
	
	p = (short *)arptr;
	for (i = 0; i < HDSIZE; i += 2)
		xputw(*p++, fp);
}

