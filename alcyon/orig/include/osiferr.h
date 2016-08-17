
/* OSIF Error handling 			*************************************/
extern int errno;			/* error place for assigning	    */
extern int __cpmrv;			/* the last BDOS return value (AX)  */
extern int _errcpm;			/* place to save __cpmrv	    */
#define RETERR(val,err) {errno=(err);_errcpm=__cpmrv;return(val);}

