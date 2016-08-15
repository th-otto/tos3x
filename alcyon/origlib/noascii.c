/* NOASCII: pulls out as much ASCII disk i/o as possible. */


noascii() { ; }					/* stubroutine 'tag'	    */


char * __noascii_io_msg = "ascii disk i/o rtns";

_rdasc() { _optoff(__noascii_io_msg); }
_wrtasc() { _optoff(__noascii_io_msg); }


