/* NOFILESZ: eliminates routine to calculate filesize (used in lseek()) */


nofilesz() { ; }				/* stubroutine 'tag'	    */


int	_filesz() { return 32767; }

