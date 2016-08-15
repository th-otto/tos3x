/* maketask call for unix  */

int maketask(fname,flags,priority,argptrs)
char *fname;
int flags,priority;
char **argptrs;
{

	register int pid;

	if(flags==0) {		/* fork and exec */
		if((pid=fork()) == 0) {		/* child process */
doexecv:
			execv(fname,argptrs);
			printf("execv failed on %s\n",fname);
			exit(-1);
		}
		return(pid);		/* return child process id */
	}
	if(flags==2) {		/* fork only */
		return(fork());
	}
	if(flags==4) {		/* exec only */
		goto doexecv;
	}
	return(-1);
}
