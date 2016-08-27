
#include "../libsrc/lib.h"
#include <fcntl.h>

int _main(P(char *) com, P(int) len)
PP(char *com;)							/* Command address      */
PP(int len;)								/* Command length       */
{
	register char *s;						/* Temp char pointer        */

	/* Initialize channels */
	_chinit();
	open(__tname, O_RDONLY);				/* Open STDIN           */
	open(__tname, O_WRONLY);				/* Open STDOUT          */
	open(__tname, O_WRONLY);				/* Open STDERR          */

	com[(long)len] = 0;						/* Insure null at line end  */
	
	UNUSED(s);
	return __main(com, len);
}
