#include <stdio.h>
#include "util.h"

int efseek(P(FILE *) sp, P(long) offs, P(int) sense)
PP(FILE *sp;)
PP(long offs;)
PP(int sense;)
{
	if (sp->_flag & _IOWRT) /* WTF */
		fflush(sp);
	if (sense & 1)
		offs -= sp->_cnt;
	sp->_cnt = 0;
	sp->_ptr = sp->_base;
	return lseek(fileno(sp), offs, sense);
}


