
/* NOSTART.C - provides a routine to prevent the linkage of the		    */
/*	'normal' RTL startup routine, '_main()'.			    */
/*	When this module is linked in, there will be no redirected	    */
/*	i/o, the command line will not be parsed into argc/argv, standard   */
/*	i/o channels will not be opened,  and files will not be closed 	    */
/*	automatically on termination.					    */
/* NOTE: 'argc/argv' is replaced by 'cmd/len', where cmd -> command tail    */
/*	and len is the length of this string.				    */

#include "lib.h"
#include <stdlib.h>
#include <option.h>

/* stubroutine tag */
VOID nostartup(NOTHING) { ; }

int main PROTO((char *, int));

/* stubroutine for startup '_main()' */
int _main(P(char *) cmd, P(int) len)
PP(char *cmd;)			/* ptr to CP/M command tail	    */
PP(int len;)			/* length of CP/M command tail	    */
{
	_exit(main(cmd, len));		/* call the main program	    */
}
