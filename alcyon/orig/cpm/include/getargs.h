/*	Getargs.h	Typedefs and defines needed for getargs
 */
#define	INTEGER		0
#define BOOLEAN 	1
#define CHARACTER	2
#define STRING		3
#define PROC		4
typedef struct
{
	unsigned	arg  : 7  ;	/* Command line switch	    */
	unsigned	type : 4  ;	/* variable type	    */
	int		*variable ;	/* pointer to variable	    */
	char		*errmsg	  ;	/* pointer to error message */
}
ARG;
able type	    */
	int		*variable ;	/* pointer to variable	    */
	char		*errmsg	  ;	/* pointer to error message */
}
ARG;
