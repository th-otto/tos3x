/* NOBINARY: pulls out as much BINARY disk i/o as possible. */


nobinary() { ; }					/* stubroutine 'tag'	    */


char * __nobinary_io_msg = "binary disk i/o rtns";

_rdbin() { _optoff(__nobinary_io_msg); }
_wrtbin() { _optoff(__nobinary_io_msg); }


