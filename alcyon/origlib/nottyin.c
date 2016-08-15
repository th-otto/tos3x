/* NOTTYIN: eliminates console input functions from read(). */


nottyin() { ; }					/* stubroutine 'tag'	    */

static char * __nottyin_msg = "tty input";

_ttyin() { _optoff(__nottyin_msg); }
