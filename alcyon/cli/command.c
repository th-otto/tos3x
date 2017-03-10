/*	command.c

GEMDOS command interpreter

Originally written by JSL.

MODIFICATION HISTORY - As of version 0.20 as modified by LTG

   06-03-85 0.21 LTG Fixed bug in chkDir, not copying null in wildExp.
   06-03-85 0.22 LTG Fixed bug in rename command.
   06-05-85 0.23 LTG Fixed type command - msg 6.
                     Batch files now must end in .BAT.
   06-07-85 0.24 LTG Fixed bug in ERA which erased all with <foo.foo as input
                     Freed up batch files in inhouse version.
   06-07-85 0.25 LTG Move batch files to general consumption version
                     Built enviornment strings
   06-12-85 0.26 LTG Fixed bug with ctl c.
                     Fixed bug with show,  enhanced show command.
                     Added MOVE command.
                     Added Quoted (") strings to REM
                     Added ECHO as a synonym of REM
   06-13-85 0.27 LTG Fixed bug in program execution causing Command Not found
                     message when program returs negative word
   06-17-85 0.29 LTG Fixed bug that caused atari to crash.
   06-17-85 0.30 LTG Added ENV command.
   06-26-85 0.31 LTG Added HELP facility.
   07-22-85 0.32 LTG Added \r \n \0 to REM and ECHO.
   07-25-85 0.33 LTG Added ERR and CHMOD commands.
                     Added err message to CD if directory not found.
                     Fixed bug in diectory display.
   08-06-85 0.34 LTG Changed '\' to '/' in chk_sub() and chk_str().
                     Fixed bug in copy displaying two msg for file not found.
   08-07-85 0.35 LTG Fixed bug in chk_sub() that caused a problem in parameter
                     substitution involving NULL strings.
   08-12-85 0.36 LTG Added code to abort processing if any .prg file returns
                     an error condition.  Thus batch files are terminated show
                     one of its commands fail.
   08-15-85 0.37 LTG Fixed bug running multiple batch files.  Move dup(0) and
                     dup(1) into code specific to I/Oa redirection.
                     Implemented PRGERR to enable/disable command processing abort
                     on non zero completion code from .PRG files.
   08-16-85 0.38 LTG Added querry function to erase.
   08-19-85 0.39 LTG Fixed bug in @CMD function that passed args from previous cmd.
   10-16-85 0.40 LTG Changed CLS to ^h^j
                     Fixed bug with abort when output redirected which stays so
                     Fixed bug in copy which does not respond properly to read error.
                     Added Comment facility.
                     Added WIDE list option to DIR.
                     Added message to warn when ren or copy wild carded name
                     Fixed bug in REN that caused list of renamed files to be
                     put one after the other on display.
   11-08-85 0.41 LTG Added ability to parse command tail passed to command.prg
   11-14-85 0.42 LTG Added code to erase partially created file when copy aborts
                     Fixed bug that disallowed copy \pathn \pathx
                     Added code to allow spec of multiple names in del command
   11-21-85 0.43 LTG Added multiple files to type command.
                     Changed code so that commands output to screen when > used.
   12-17-85 0.44 LTG Fixed bug in REM.
   01-17-85 0.45 LTG Added stay STAY and total files to dir.
   02-25-86 0.46 MLC Fixed bug in xCmdLn when removing root directory, crashed.
   		     Fixed bug in ioredirection when an illegal file name is 
		     there, creating the file has problem.
		     Add time, type, date, diff, era, find and format to help
		     message and corrected some misspelling.
   03-07-86 0.47 MLC Fixed bug in SHOW command with illegal drive name.
		     Fixed bug in CHMOD command without mode specification.
		     Fixed bug in I/O redirection, checking if files can be created.
   		     Fixed bug in "copy a:  " command, where a is current directory.
   03-14-86 0.48 MLC Fixed bug in chk_sub. When %% is enter.
    		     date.prg and time.prg are also modified seperately.
   03-18-86 0.49 MLC Fixed bug in HELP command: displays too many lines before
    		     pausing and also pausing when output was redirect.
    		     Fixed bug in dspCL: command line was not completely 
    		     displayed, the redirection symbol and the file name missing
    		     Fixed bug in CHMOD command: when the mode was not 0,
    		     not message was displayed; wildcards was not processed
    		     correctly: only one file get changed mode.
    		     Fixed bug in DIR command: displayed hidden and system file.
    		     Fixed bug in DEL command: displayed "DONE",even the file
    		     is a read only file.
    		     Fixed bug in RD command: when "rd a:".
    		     Fixed bug in REM command: it not only displayed the string
    		     but also the whole command.
    		     Fixed bug in nesting too many levels of batch files causing
    		     the system crash without warning.
   03-25-86 0.50 MLC Fixed scrolling in help command.
   03-31-86 0.51 MLC Fixed bug in RD command. 
    		     Fixed bug in @ directive.
    		     Changed wording in SHOW error message.
    		     Added "Done" message to "RD" and "MD" command when finish.
   04-15-86 0.52 MLC Fixed bug in copy file to it self.
   04-21-86 0.53 MLC Fixed bug in copy command. Put the current directory in
    		     the path name when none is supplied. Also fixed the bug
    		     in a command line specified @ as output file.(e.g. >@)
   05-01-86 0.54 MLC Fixed bug in chk_sub, so that if % is contained in a quoted
    		     string, it will not be processed as parameter substitute.
    		     Fixed bug in rename and copy command.  Also fixed bug in
    		     readDsk. Fixed bug in outputting "command not found".
   05-07-86 0.55 MLC When wildcard in the TYPE command, print error message.
    		     Added cr/lf to the end of help message. Fixed COPY command 
    		     such that it copy the date stamp from the old file. Deleted
    		     the repeated lines in help message.  Fixed DIR command for
    		     the hidden/system file.
   06-03-86 0.57 MLC Fixed bug in COPY command. Fixed invalid drive/path name
    		     in COPY, MOVE, CHANGE DIRECTORY, INIT, PATH commands.
   08-06-86 0.58 MLC Fixed bug in chk_redirect.  Fixed bug in COPY command
    		     so the file attributes is set as the source file.  Fixed
    		     bugs in write error in TYPE command and add wrerr routine.
    		     Also fixed bug in chdir.
   09-15-86 0.59 MGA Fixed bug in creation of the copy of environment strings
		     for an xexec'd process: one byte too few being allocated
		     for the environment.
   10-28-86 0.60 MLC Fixed bug in deleting a file name -.  - is for query option
    		     for the delete command before, since no doc mentions it,
    		     it will be taken away.
   10-30-86 0.61 MLC Fixed bug in dir command with subdirectory names longer
    		     than 67.
   10-31-86 0.62 MLC Fixed bug in file names specified by "?" in the commands
    		     COPY, DIR, MV, DEL, CD, RD etc.  Fixed bug in SHOW when
    		     an illegal drive name is entered.  Correct the spelling
    		     in Help command.  When no file is copied, change "Done"
    		     to "No file found" in COPY command.  Also took care of "*",
    		     "?" in PATH command.
   11-06-86 0.63 MLC Not allow '*' or '?' in PATH, CD, MD and RD commands. 
   11-07-86 0.64 MLC RD did not print the error message when there is a * or ?.
   11-11-86 0.65 MLC '*' or '?' in .bat file or .prg file name, issue error message.
   11-13-86 0.66 MLC Make buffer larger to hold directory/path/file name.  If
    		     no file are deleted, issue "no file deleted" instead of "done"

NAMES

	JSL	Jason S. Loveman
	LGT	Lou T. Garavaglia
	SCC	Steven C. Cavender
	MLC	Mei L. Chung
	MGA	Mark G. Alexander

*/

#define _NO_IOBUF

#include <stdio.h>
#include <string.h>
#include <osbind.h>
#include <fcntl.h>

#define _(x) x

struct xjmpbuf {
	long fp;
	long sp;
	long pc;
};

/*
 * from coma.s
 */
VOID in_term PROTO((NOTHING));
VOID rm_term PROTO((NOTHING));
VOID super PROTO((NOTHING));
VOID user PROTO((NOTHING));
VOID xlongjmp PROTO((struct xjmpbuf *jb, int code));
int xsetjmp PROTO((struct xjmpbuf *jb));
VOID cpmopen PROTO((char *path));
VOID cpmcreate PROTO((char *path));
VOID cpmclose PROTO((NOTHING));
int cpmread PROTO((char *buf));
int cpmwrite PROTO((const char *buf));
VOID xbrkpt PROTO((NOTHING));
VOID devector PROTO((NOTHING));

#define FALSE 0
#define TRUE 1
#define MAXARGS 20

struct rdb								/*IO redirection info block     */
{
	int nso;
	int nsi;
	int nonstdin;
	int echocmd;
	short oldso;
	short oldsi;
};

struct rdb *rd_ptr;

char zero = { '0' };

char hexch[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
int drv;
int exeflg;
int rtrnFrmBat;
int prgerr;
int cmderr;
int batlev;								/* batch file nested level */
struct xjmpbuf jb;
long compl_code;

#undef BUFSIZ
#define BUFSIZ 10000
char buf[BUFSIZ];

char lin[130];

char srchb[44];
char prgTail[5] = { ".PRG" };
char batTail[5] = { ".BAT" };
char autoBat[13] = { "AUTOEXEC.BAT" };
char pthSymb[6] = { "PATH=" };

char drvch;

char *basePage;

char *prntEnvPtr;

/* Declarations for Wild Card processing: */
char *WSrcReq;

int WAttCode;
char wildExp[4] = { "*.*" };

#define PATH_MAX 140

char srcFlNm[PATH_MAX];						/* src file name */
char dstFlNm[PATH_MAX];						/* destination file name */
char srcDir[PATH_MAX];						/* src dir path */
char dstDir[PATH_MAX];						/* dst dir path */
char srcNmPat[13];						/* src file name specified in path */
char dstNmPat[13];						/* dst file name specified in path */
char path[PATH_MAX];							/* lst of default path names       */

/* Forward declarations. */
VOID chk_redirect PROTO((struct rdb *r));
VOID errout PROTO((NOTHING));
int xncmps PROTO((int n, const char *s, const char *d));
VOID prthex PROTO((unsigned int h));
VOID prtdecl PROTO((long d));
VOID pdl PROTO((long d));
VOID prtDclFmt PROTO((long d, int cnt, char *ch));
VOID ucase PROTO((char *s));
int chk_wild PROTO((char *s));
int gtFlNm PROTO((NOTHING));
int chkDir PROTO((char *pathExp, char *dirExp, char *filExp, int check));
int chkDst PROTO((NOTHING));
int mkDst PROTO((NOTHING));
VOID mkSrc PROTO((NOTHING));
VOID wrerr PROTO((NOTHING));
VOID wrt PROTO((const char *msg));
VOID wrtln PROTO((const char *msg));
VOID wrtch PROTO((char ch));
VOID cr2cont PROTO((NOTHING));
VOID dspMsg PROTO((int msg));
VOID dspHelp PROTO((int stand_out));
int getYes PROTO((NOTHING));
int copyCmd PROTO((char *src, char *dst, int move));
VOID renmCmd PROTO((char *src, char *dst));
long dirCmd PROTO((char **argv));
long chmodCmd PROTO((char **argv));
long typeCmd PROTO((char **argv));
long delCmd PROTO((char **argv));
VOID dspCL PROTO((NOTHING));
VOID preCmd PROTO((struct rdb *rd, long newsi, long newso));
long setPath PROTO((char *p));
long execPrgm PROTO((char *s, char *cmdtl));
int execBat PROTO((char *s, char **parms));
int mknum PROTO((char *str));
int chk_sub PROTO((char *tl, char **parm));
VOID chk_str PROTO((char **parm));
long readSi PROTO((char *lin));
int readDsk PROTO((char *lin, int flHnd));
VOID xCmdLn PROTO((char **parm, int *pipeflg, int nonStdIn, char *outsd_tl));

/* entry point called from coma.S */
VOID cmain PROTO((char *bp));


/***************************************************************************/
/*
   chk_redirect - determines it input or output has been redirected, if so
                  restoring it to previous value.
*/
/***************************************************************************/
VOID chk_redirect(P(struct rdb *) r)
PP(register struct rdb *r;)
{
	/* if a new standard in specified ...*/
	if (r->nsi)
	{
		Fclose(0);
		Fforce(0, r->oldsi);
		Fclose(r->oldsi);
		r->nsi = FALSE;
	}
	/* if a new standard out specified.*/
	if (r->nso)
	{
		Fclose(1);
		Fforce(1, r->oldso);
		Fclose(r->oldso);
		r->nso = FALSE;
	}
}

/***************************************************************************/
VOID errout(NOTHING)
{
	chk_redirect(rd_ptr);
	xlongjmp(&jb, -1);
}

/***************************************************************************/
int xncmps(P(int) n, P(const char *) s, P(const char *) d)
PP(register int n;)
PP(register const char *s;)
PP(register const char *d;)
{
	while (n--)
		if (*s++ != *d++)
			return 0;
	return 1;
}

/***************************************************************************/
VOID prthex(P(unsigned int) h)
PP(unsigned int h;)
{
	unsigned int h2;

	if ((h2 = (h >> 4)) != 0)
		prthex(h2);
	else
		Fwrite(1, 1L, "0");
	Fwrite(1, 1L, &hexch[h & 0x0f]);
}

/***************************************************************************/
static size_t xstrlen(P(const char *) s)
PP(register const char *s;)
{
	register int n;

	for (n = 0; *s++; n++)
		;
	return n;
}

/***************************************************************************/
/***************************************************************************/
VOID prtdecl(P(long) d)
PP(long d;)
{
	if (d)
		pdl(d);
	else
		Fwrite(1, 1L, "0");
}

/***************************************************************************/
/***************************************************************************/
VOID pdl(P(long) d)
PP(long d;)
{
	long d2;

	if ((d2 = d / 10) != 0 )
		pdl(d2);
	Fwrite(1, 1L, &hexch[d % 10]);
}

/***************************************************************************/
/***************************************************************************/
VOID prtDclFmt(P(long) d, P(int) cnt, P(char *) ch)
PP(long d;)
PP(int cnt;)
PP(char *ch;)
{
	register int i;
	register long k, j;

	k = (d ? d : 1);
	j = 1;
	for (i = 1; i < cnt; i++)
		j *= 10;
	while (k < j)
	{
		Fwrite(1, 1L, ch);
		k *= 10;
	}
	prtdecl(d);
}

/***************************************************************************/
/***************************************************************************/
VOID ucase(P(char *) s)
PP(register char *s;)
{
	for (; *s; s++)
		if (*s >= 'a' && *s <= 'z')
			*s &= ~0x20;
}

/***************************************************************************/
/* chk_wild checks wildcard * and ? in the string s.  If s contains * or ?,*/
/* it issue error message and return false, else it return true.	       */
/***************************************************************************/
int chk_wild(P(char *) s)
PP(char *s;)
{
	char *q;

	q = s;								/*scan input to see any * or ? embeded */
	while (*q && *q != '*' && *q != '?')
		q++;
	if (*q)
	{
		dspMsg(18);
		wrt(s);
		compl_code = -1L;
		return FALSE;
	} else
		return TRUE;
}

/***************************************************************************/
/*
   gtFlNm - get file name of the next file in the directory match a 
      path\wildcat specification. The first invocation makes a call to
      Fsfirst.  Each subsequent invocation uses Fsnext().  To invoke
      the routine, the wildcarded path name is put into WSrcReq and the
      routine called.  For this and each subseqent call the descriptor block
      for the found file (if one was found) is pointed to by WThisSrc.


   returns 0 if no match
   returns -1 if file found

*/
/***************************************************************************/

int gtFlNm(NOTHING)
{
	/* First file request? */
	if (WSrcReq != NULL)
	{
		if (Fsfirst(WSrcReq, WAttCode))
			return FALSE;

		WSrcReq = NULL;
	}

	/* Subsequent file request	*/
	else
	{
		if (Fsnext())
			return FALSE;
	}
	return TRUE;
}

/***************************************************************************/
/* 
  chkDir - Evaluates pathExp to determine if it specifies a directory, 
     or a file name.  For convenienc sake it stuffs the directory part in
     dirExp and the file name into filExp.   If a file name (ambiguous or
     not) was found, it is place in filExp.  If no file name was found,
     filExp points to "*.*".

  int chkDir (pathExp, dirExp, filExp, check)

returns	  -4 if Illegal syntax : was found.
          -3 if wild cards specified in path name
          -2 if pathExp does NOT specify a file, directory, or device
          -1 if pathExp evaluates to a subdirectory, or a volume label
           0 if pathExp evaluates to a normal, read-only, or system file
           1 if pathExp evaluates to a normal, read-only, or system file but
             was specified with wild cards

  pathExp - ptr to path expression, w/wo wild cards to be evaluated
  dirExp  - ptr to dir part of pathExp
  filExp  - ptr to file.ext part of pathExp
  check	  - flag to check if the file exists.
*/
/***************************************************************************/

int chkDir(P(char *) pathExp, P(char *) dirExp, P(char *) filExp, P(int) check)
PP(char *pathExp;)
PP(char *dirExp;)
PP(char *filExp;)
PP(int check;)
{
	int pathLen = xstrlen(pathExp);
	int dirLen;
	int wildLen = xstrlen(wildExp);
	int wildFnd = FALSE;
	int i = 0;
	int flExsists;
	char c;
	char pathbuf[PATH_MAX];
	char *p;
	char *q;

	/* directory length = path length to start with. */
	dirLen = pathLen;
	pathbuf[0] = pathbuf[2] = 0;
	ucase(pathExp);

	if (!pathLen)
	{									/*if no directory was specified, use current drive */
		pathExp[0] = Dgetdrv() + 'A';
		pathExp[1] = ':';
		Dgetpath(pathbuf, 0);		/*get current path name */
		for (dirLen = pathLen = 2, p = pathbuf, q = pathExp + 2; (*q = *p) != '\0'; dirLen++, pathLen++, p++, q++) ;
		if (*(q - 1) != '\\')
		{
			*q++ = '\\';
			*q = 0;						/*terminated by null */
			pathLen++;
			dirLen++;
		}
	} else
	{
		/* Loop thru path expression from end looking for delimeters and the 1st char.*/
		do
		{
			if ((c = pathExp[dirLen]) == '*' || c == '?')
				wildFnd = TRUE;
		} while ((c != ':') && (c != '\\') && (dirLen--));
		if (dirLen++ && c == '\\')		/*contains subdiretory name,but not from root */
			i = dirLen;
		if (c == ':')
		{
			if (pathExp[1] != ':')
				return (-4);
			pathbuf[0] = pathExp[0];
			pathLen -= 2;				/*drive name & : */
			q = pathExp + 2;
			Dgetpath(&pathbuf[2], pathExp[0] - 'A' + 1);
			goto cp_dir;
		} else if (!dirLen || (pathExp[1] != ':'))
		{								/*no drive was specified */
			pathbuf[0] = Dgetdrv() + 'A';	/*get the default drive name */
			q = pathExp;
			if (*q == '\\')
			{							/*root was specified, don't get the default directory */
				pathLen--;
				if (i > 1)
					--i;
				q++;
				goto cp_dir;
			}
			Dgetpath(&pathbuf[2], 0);
		  cp_dir:
			pathbuf[1] = ':';
			for (dirLen = 2, p = pathbuf + 2; *p; p++, dirLen++)
				;
			if (*(p - 1) != '\\')
			{
				*p++ = '\\';
				dirLen++;
			}
			dirLen += i;
			for (; pathLen > 0; pathLen--)	/*copy file name to pathbuf */
				*p++ = *q++;
			*p = 0;						/*terminated by null */
			for (p = pathbuf, q = pathExp; (*q++ = *p++) != '\0'; pathLen++)	/* copy to pathExp */
				;
		}
	}
	/* IF nothing specified defalult to wild expresion.*/
	if (pathExp[pathLen - 1] == '\\' || pathExp[pathLen - 1] == ':')
	{
		i = 0;
		while ((pathExp[pathLen] = wildExp[i]) != '\0')
		{
			pathLen++;
			i++;
		}
		wildFnd = TRUE;
	}

	/* return file not found if not found.*/
	flExsists = Fsfirst(pathExp, WAttCode) ? -2 : 0;

	/* if wild cards were specified in file name and file exsists return 1 */
	flExsists = wildFnd ? (flExsists ? -2 : 1) : flExsists;

	if (check && wildFnd && flExsists == -2)
		return (flExsists);

	/* If no wild cards/file name found check if directory. */
	if (!wildFnd)
	{
		/* if a file structure exsists... */
		if (!flExsists)
		{
			/* if it is a directory or a volume label... */
			if (srchb[21] & 0x18)
			{
				/* Set up dirLen to encompas entire path specification. */
				dirLen = pathLen;

				/*Tackon a path seperator. */
				pathExp[dirLen++] = '\\';

				/* copy wild card expresion into spec'd path and file name. */
				for (i = 0; i <= wildLen; i++)
					/* onto end of path expresion. */
					pathExp[i + dirLen] = wildExp[i];

				flExsists = -1;
			}
		}
	}
	/* copy path exp into directory expresion. */
	for (pathLen = 0; pathLen < dirLen; pathLen++)
		dirExp[pathLen] = pathExp[pathLen];

	/* chop off file nm from dirExp.*/
	dirExp[dirLen] = '\0';

	/* copy file name into return var.*/
	i = 0;
	while ((c = filExp[i++] = pathExp[pathLen++]) != '\0')
		if (c == '*')
			flExsists = -3;

	return flExsists;
}

/***************************************************************************/
/* 
   int chkDst ();

   chkDst - Checks dst file name for validity. If there are any wild cards in
      the source file name the only valid dst names are "*", or "*.*".  Any
      thing else results in an error.

   returns 0 if no error
           -1 if unable to make dst file name

*/
/***************************************************************************/
int chkDst(NOTHING)
{
	int i = 0;
	char c;

	/* check for proper use of wild chards.*/
	while ((c = srcNmPat[i++]) != 0)
	{
		/* Look for wild card chars. */
		switch (c)
		{
		case '*':
		case '?':

		  doDstChk:
			/* If dst file name longer than wild exp, must be error. */
			if (xstrlen(dstNmPat) > xstrlen(wildExp))
				return -1;

			/* Loop till end of dst fil nam to see if it matches wild exp. */
			for (i = 0; (c = dstNmPat[i]); i++)
				if (c != wildExp[i])
					return -1;

			/* return ok. */
			return 0;

		default:
			break;
		}
	}

	/* if any wild cards in dst, check for validity.*/
	for (i = 0; (c = dstNmPat[i]); i++)
		if (c == '*' || c == '?')
			goto doDstChk;

	/* return ok.*/
	return 0;
}

/***************************************************************************/
/*
   int mkDst - Make dst file name.

   returns 0 if dst other than src
   returns -1 if dst same as src

   srcFlNm - ptr to string from search first on path name
   dstFlNm - ptr to string that will recieve destination file name
   srcDir  - ptr to src dir path
   dstDir  - ptr to dst dir path
   srcNmPat- ptr to string that contains the file name specified in path
   dstNmPat- ptr to string that contains the dst pattern

*/
/***************************************************************************/
int mkDst(NOTHING)
{
	register int i, k, ndx;
	register int srcEqDst;

	i = 0;
	ucase(&srchb[30]);

	/* determine If dst dir path = src dir path.*/
	while ((srcEqDst = (srcDir[i] == dstDir[i])) && srcDir[i] && dstDir[i])
		i++;

	/* if they do...*/
	if (srcEqDst)
	{
		i = 0;

		/* if the dst is not a wild card (in which case auto match)... */
		if (!(srcEqDst = (dstNmPat[0] == '*')))

			/* loop, chk each src=dst file.ext for match, setting srcEqDst. */
			while ((srcEqDst = (srchb[30 + i] == dstNmPat[i])) && srchb[30 + i] && dstNmPat[i])
				i++;
	}
	i = 0;
	ndx = 0;
	/* copy dst dir path into dst file name.*/
	while ((dstFlNm[ndx] = dstDir[ndx]) != '\0')
		ndx++;

	/* if the entire name matches create a dst file name with '.&&&' as ext */
	if (srcEqDst)
	{
		while ((dstFlNm[ndx + i] = srchb[30 + i]) != '\0' && dstFlNm[ndx + i] != '.')
			i++;
		dstFlNm[ndx + i++] = '.';

		for (k = 0; k <= 2; k++)
			dstFlNm[ndx + i + k] = '&';
	} else
	{
		/* if dst file pat is wild card, copy src file name into dst file name. */
		if (dstNmPat[0] == '*')
			while ((dstFlNm[ndx + i] = srchb[30 + i]) != 0)
				i++;

		else
			/* copy dst name pat directly into dst file name. */
			while ((dstFlNm[ndx + i] = dstNmPat[i]) != 0)
				i++;
	}
	return srcEqDst;
}

/***************************************************************************/
/* 
   mkSrc - make source file name from directory path and file name.

*/
/***************************************************************************/
VOID mkSrc(NOTHING)
{
	register int i;
	register int j = 0;

	/* copy src directroy into src directory file name.*/
	for (i = 0; (srcFlNm[i] = srcDir[i]); i++)
		;

	/* copy source file name from search first/next into src file name.*/
	while ((srcFlNm[i + j] = srchb[30 + j]) != 0)
		j++;
	ucase(srcFlNm);
}

/***************************************************************************/
/*
    wrerr - write error
*/
/***************************************************************************/
VOID wrerr(NOTHING)
{
	int fd;

	fd = Fopen("CON:", O_RDWR);
	Fwrite(fd, 24L, "\r\nFile write error....\r\n");
	Fclose(fd);
	errout();
}

/***************************************************************************/
/*
   wrt - write to standard output
*/
/***************************************************************************/
VOID wrt(P(const char *) msg)
PP(const char *msg;)
{
	long count, bytes;

	bytes = xstrlen(msg);
	count = Fwrite(1, bytes, msg);
	if (count != bytes)
		wrerr();
}

VOID wrtln(P(const char *) msg)
PP(const char *msg;)
{
	wrt("\r\n");
	wrt(msg);
}

VOID wrtch(P(char) ch)
PP(char ch;)
{
	char str[2];

	str[0] = ch;
	str[1] = 0;
	wrt(str);
}

#if 0
VOID wrtEol(NOTHING)
{
	wrt("\r\n");
}

VOID to(P(int) n)
PP(int n;)
{
	char ch;
	ch = n + '0';
	wrt(" OUT<< ");
	Fwrite (1, 1L, &ch);
	wrtln("");
}

VOID ti(P(int) n)
PP(int n;)
{
	char ch;
	
	ch = n + '0';
	wrt(" IN>> ");
	Fwrite(1, 1L, &ch);
	wrtln("");
}
#endif

/***************************************************************************/
/*
   cr2cont - wait for cariage return before continuing.
*/
/***************************************************************************/
VOID cr2cont(NOTHING)
{
	wrt("CR to continue...");
	lin[0] = 126;
	Cconrs(lin);
}

/***************************************************************************/
/*
   dspHelp - display help message
*/
/***************************************************************************/

VOID dspHelp(P(int) stand_out)
PP(int stand_out;)
{
	wrtln("# in the first non blank column in a batch file is a comment.");
	wrtln("CAT or TYPE filenm.ext [[filenm.ext]...]");
	wrtln("    Writes filenm.ext(s) to standard output.");
	wrtln("CD [pathnm]");
	wrtln("    With pathnm it sets default for working directory.");
	wrtln("    Without pathnm displays current working directory.");
	wrtln("CHMOD [pathnm/]filenm mode");
	wrtln("    Changes the mode of the file specified in filenm to the");
	wrtln("    value of mode.  Acceptable values are <= 7:");
	wrtln("       0 - Normal File Entry");
	wrtln("       1 - File is Read Only");
	wrtln("       2 - File is Hidden from directory Search");
	wrtln("       4 - File is System File");
	wrtln("       3, 5, 6, 7 - are combinations of the above value");
	wrtln("CLS");
	wrtln("    Clears the screen.");
	wrtln("COPY source_file [destination_file]");
	wrtln("    Copies source to destination.");
	wrtln("DATE [mm/dd/yy]");
	wrtln("    Sets or displays the date that you enter into the system");
	wrtln("DIFF [-b] filespec1 filespec2");
	wrtln("    Compares two files character by character.");
	wrtln("    -b - ingnores spaces in the files and compares text only");
	if (stand_out)
	{
		wrtln("");
		cr2cont();
	}
	wrtln("DIR or LS [filenm.ext] [-f] [-d] [-t] [-w]");
	wrtln("    -f - anything but directories.");
	wrtln("    -d - directories only.");
	wrtln("    -t - terse: names only.");
	wrtln("    -w - wide: names only displayed horizontally.");
	wrtln("ERA [d:][path]filespec");
	wrtln("    Removes one or more files from a disk directory");
	wrtln("ERR ");
	wrtln("    Displays the value of the Completion Code for the last command.");
	wrtln("EXIT");
	wrtln("    Exits CLI to invoking program.");
	wrtln("FIND \"string\"filename1 [filename2...filenamelast]");
	wrtln("    Search for the occurrence of a character string in one or more files");
	wrtln("FORMAT d:");
	wrtln("    Prepares a disk to recieve data before it can be used");
	wrtln("INIT [drive_spec:]");
	wrtln("    Reinitializes FAT entries, erases data from a disk.");
	wrtln("MD [subdirectory name]");
	wrtln("    Creates a new subdirectory in current directory.");
	wrtln("MOVE source_file [destination_file]");
	wrtln("    Copies source to destination and deletes source.");
	wrtln("PAUSE");
	wrtln("    Writes 'CR to continue...' to standard output");
	if (stand_out)
	{
		wrtln("");
		cr2cont();
	}
	wrtln("    and waits for a carriage return from standard input.");
	wrtln("PRGERR [ON | OFF]");
	wrtln("    Turns command processing abort feature ON/OFF.");
	wrtln("    If PRGERR is ON and a .PRG file returns a non zero completion");
	wrtln("    code, all further processing will stop.  Usefull in .BAT files.");
	wrtln("    Default is ON.");
	wrtln("NOWRAP");
	wrtln("    Disables line wrap.");
	wrtln("PATH [;[pathnm]...]");
	wrtln("    With path name sets default path for batch and commands.");
	wrtln("    Without path name displays current path");
	wrtln("REM or ECHO [\"string\"]");
	wrtln("    Strips quotes and writes string to standard output.");
	wrtln("    \\r is replaced by 0x13, \\n by 0x10, \\0 by 0x0.");
	wrtln("    \\c by 0x13 0x10, \\anything is replaced by anything.");
	wrtln("REN source_file_nm [destination_file_nm]");
	wrtln("    Renames source to destination.");
	wrtln("RD [pathnm]");
	wrtln("    Removes named directory.");
	wrtln("RM or DEL or ERA filenm [[filemn]...] [-q]");
	wrtln("    Removes named file from directory.");
	wrtln("    IF the -q option is used, the CLI will display the question");
	wrtln("    Y/CR... and wait for a responce.");
	wrtln("SHOW [drive_spec:]");
	wrtln("    Displays disk status for default drive or drive specified.");
	if (stand_out)
	{
		wrtln("");
		cr2cont();
	}
	wrtln("STAY");
	wrtln("    Optionally stays in cli instead of exiting when");
	wrtln("    cli is invoked with parameters.");
	wrtln("TIME [hh:mm:ss]");
	wrtln("    Sets or displays the time that you enter into the system");
	wrtln("TREE [d:]");
	wrtln("    Displays all the directory paths on a given disk drive");
	wrtln("TYPE [d:][path]filename.ext");
	wrtln("    Displays the contents of a file on your screen");
	wrtln("VERSION");
	wrtln("    Displays current version of OS.");
	wrtln("WRAP");
	wrtln("    Enables line wrap.");
	wrtln(">filenm, >>filenm, <filenm");
	wrtln("    Provides for ouput, concatenation, and input redirection.");
	wrtln("cmd @filenm");
	wrtln("    Allows a list of command tails to be put in a file and fed to a command.");
	wrtln("");					/* for prn, otherwise, the previous line won't print */
}

/***************************************************************************/
/*
   dspMsg - display message
*/
/***************************************************************************/
VOID dspMsg(P(int) msg)
PP(int msg;)
{
	switch (msg)
	{
	case 0:
		wrtln("Wild cards not allowed/Cannot rename/type directories.");
		break;
	case 1:
		wrtln("File Not Found.");
		break;
	case 2:
		wrtln("Destination is not a valid wild card expresion.");
		break;
	case 3:
		wrtln("******* TEST  CLI *******");
		break;
	case 4:
		wrtln("Command v0.66 11/13/86 MLC");
		break;
	case 5:
		wrt("Done.");
		break;
	case 6:
		wrtln("Command is incompletely specified.");
		break;
	case 7:
		wrt(srcFlNm);
		break;
	case 8:
		wrt(dstFlNm);
		break;
	case 9:
		wrtln(".");
		break;
	case 10:
		wrt(" to ");
		break;
	case 11:
		break;
	case 12:
		wrtln("");
		break;
	case 13:
		wrtln("");
	case 14:
		wrt("{");
		drvch = (drv = Dgetdrv()) + 'a';
		Fwrite(1, 1L, &drvch);
		wrt("}");
		break;
	case 15:
		wrtln("Wild cards not allowed in destination.");
		break;
	case 16:
		drvch = (drv = Dgetdrv()) + 'a';
		Fwrite(1, 1L, &drvch);
		wrt(":");
		break;
	case 18:
		wrtln("Invalid drive/path/file name specified");
		break;
	}
}

/***************************************************************************/
/*
getYes 
*/
/***************************************************************************/
int getYes(NOTHING)
{
	char inpStr[30];

	inpStr[0] = Cnecin();
	inpStr[1] = 0;
	ucase(&inpStr[0]);
	if (inpStr[0] == 'Y')
		return -1;
	return 0;
}

/***************************************************************************/
/* 
   int copyCmd()

   copyCmd - copy file.
     
   returns 0 if copyied ok
          -1 if copy failed
*/
/***************************************************************************/
int copyCmd(P(char *) src, P(char *) dst, P(int) move)
PP(char *src;)
PP(char *dst;)
PP(int move;)
{
	register int i, srcEqDst, fds, fdd;
	long nr, nw;
	char srcSpc[PATH_MAX];
	char dstSpc[PATH_MAX];
	char mod;
	int buff[2], nofile;

	for (i = 0; (srcSpc[i] = src[i]) != 0; i++)
		;
	for (i = 0; (dstSpc[i] = dst[i]) != 0; i++)
		;

	WSrcReq = srcSpc;
	WAttCode = -1;
	nofile = TRUE;
	compl_code = -1;

	/* If not a valid file name... */
	switch (chkDir(srcSpc, srcDir, srcNmPat, 1))
	{
	case -2:
		goto error6;
	case -4:
		goto error7;
	default:
		{
			/* Check destination directory. */
			if (chkDir(dstSpc, dstDir, dstNmPat, 0) == -4)
				goto error7;
			if (chkDst())
				dspMsg(2);
			else
			{
				while (gtFlNm())
				{
					if (!(srchb[21] & 0x18))
					{
						nofile = FALSE;
						mkSrc();
						if (!(srcEqDst = mkDst()))
						{
							Fdelete(dstFlNm);
							dspMsg(12);
							dspMsg(7);
							dspMsg(10);
							dspMsg(8);
							if ((fds = Fopen(srcFlNm, O_RDONLY)) <= 0)
								goto error0;
							if ((fdd = Fcreate(dstFlNm, 0x20)) <= 0)
								goto error1;
							compl_code = 0;
							nr = nw = -1;
							while (nr && nw)
							{
								if ((nr = Fread(fds, (long) BUFSIZ, buf)) > 0)
								{
									if ((nw = Fwrite(fdd, nr, buf)) < nr)
										goto error4;
								} else if (nr < 0)
								{
									goto error3;
								}
							}
							Fdatime(buff, fds, 0);	/* get the time stamp from source */
							Fdatime(buff, fdd, 1);	/* set the time stamp for destination */
							mod = (char) Fattrib(srcFlNm, 0, 0);
							Fattrib(dstFlNm, 1, mod);
							Fclose(fds);
							if (move)
							{
								Fdelete(srcFlNm);
								wrt(" DELETING ");
								dspMsg(7);
							}
							Fclose(fdd);
						} else
						{
							goto error2;
						}
					}
				}
				if (nofile)
				{
					dspMsg(1);
				} else
				{
					dspMsg(12);
					dspMsg(5);
				}
			}
		}
	}
	return 0;

  error0:
	dspMsg(1);
	return -1;
  error1:
	wrtln("Error creating file.");
	return -1;
  error2:
	wrtln("Cannot copy ");
	dspMsg(7);
	wrt(" to itself.");
	return -1;
  error3:
	wrtln("Error reading source file.");
	goto eout;
  error4:
	wrtln("Write error -- copy failed.");
	goto eout;
  error6:
	dspMsg(1);
	return -1;
  error7:
	dspMsg(18);
	return -1;

  eout:
	Fdelete(dstFlNm);
	wrt(" DELETING ");
	wrt(dstFlNm);
	return -1;
}

/***************************************************************************/
/* 
   void renmCmd

   renmCmd - rename command

*/
/***************************************************************************/
VOID renmCmd(P(char *) src, P(char *) dst)
PP(char *src;)
PP(char *dst;)
{
	register int i, code;
	char srcSpc[PATH_MAX];
	char dstSpc[PATH_MAX];

	for (i = 0; (srcSpc[i] = src[i]) != 0; i++)
		;
	for (i = 0; (dstSpc[i] = dst[i]) != 0; i++)
		;

	WSrcReq = srcSpc;
	WAttCode = -1;

	/* Set up completion code to show failure */
	compl_code = -1;

	/* IF src not specified err out. */
	if (!(*src))
	{
		dspMsg(6);
	} else
	{
		/* If not a valid file name... */
		switch (i = chkDir(srcSpc, srcDir, srcNmPat, 1))
		{
		case -4:
			dspMsg(18);
			break;
		case -3:
			dspMsg(0);
			break;
		case -2:
			dspMsg(1);
			break;
		default:
			{
				/* Check destination directory. */
				if ((code = chkDir(dstSpc, dstDir, dstNmPat, 0)) == -3)
				{
					if (i == 1)
					{
						wrt("Rename ALL files matching ");
						wrt(srcDir);
						wrt(srcNmPat);
						wrt(" (Y/CR)? ");
						if (!getYes())
							return;
						wrtln("");
					}
				} else if (code == -4)
				{
					dspMsg(18);
					break;
				} else
				{
					if (chkDst())
					{
						dspMsg(2);
					} else
					{
						while (gtFlNm())
						{
							if (!(srchb[21] & 0x18))
							{
								mkSrc();
								if (!mkDst()) ; /* ??? runaway ';'? */
								{
									dspMsg(12);
									dspMsg(7);
									dspMsg(10);
									dspMsg(8);
									compl_code = Frename(0, srcFlNm, dstFlNm);
									if (compl_code < 0)
									{
										wrt("  Rename Unsucessfull!");
									}
								}
							}
						}
						dspMsg(12);
						dspMsg(5);
					}
				}
			}
		}
	}
}

/***************************************************************************/
/***************************************************************************/
long dirCmd(P(char **) argv)
PP(char **argv;)
{
	char srcSpc[PATH_MAX];
	register int i, j, k, n, att;
	short *dt;
	int filOnly, dirOnly, terse, wide, hidSys, tf;
	long compl_c;
	long *pl;
	long ts;

	wide = filOnly = dirOnly = terse = tf = ts = hidSys = 0;
	i = 1;
	while (*argv[i])
	{
		ucase(argv[i]);
		if (*argv[i] == '-')
		{
			switch (*(argv[i] + 1))
			{
			case 'F':
				filOnly = -1;
				dirOnly = 0;
				break;
			case 'D':
				dirOnly = -1;
				filOnly = 0;
				break;
			case 'W':
				wide = -1;
				/* fall through */
			case 'T':
				terse = -1;
				break;
			case 'H':
				hidSys = -1;
				break;					/* switch show the hidden, system files */
			default:
				break;
			}
			j = i;
			while (*(argv[j] = argv[j + 1]))
				j++;
		} else
			i++;
	}

	WAttCode = -1;
	for (i = 0; (srcSpc[i] = argv[1][i]) != 0; i++)
		;
	if (chkDir(srcSpc, srcDir, srcNmPat, 0) == -4)
	{
		dspMsg(18);
		return -1;
	}

	if (!terse)
	{
		wrt("Directory of ");
		wrt(srcDir);
		dspMsg(12);
	}

	WSrcReq = srcSpc;
	if (!gtFlNm())
	{
		compl_c = -1;
		if (!terse)
			dspMsg(1);
	} else
	{
		compl_c = 0;
		k = 0;
		do
		{
			n = xstrlen(&srchb[30]);
			if (dirOnly && (srchb[21] != 0x10))
				goto skip;
			if ((filOnly) && (srchb[21] == 0x10))
				goto skip;
			if (!hidSys && (srchb[21] & 0x06))
				goto skip;
			if ((terse) && ((xncmps(2, &srchb[30], ".")) || (xncmps(3, &srchb[30], ".."))))
				goto skip;

			if (wide)
			{
				wrt(&srchb[30]);
				if (k == 5)
				{
					wrtln("");
					k = 0;
				} else
				{
					for (i = n; i < 13; i++)
						wrt(" ");
					k++;
				}
			} else
			{
				wrtln(&srchb[30]);
				tf++;
			}

			if (!terse)
			{
				for (i = n; i < 15; i++)
					wrt(" ");

				dt = (short *)&srchb[24];
				j = *dt;
				prtDclFmt((long) ((j >> 5) & 0xF), 2, "0");
				wrt("/");
				prtDclFmt((long) (j & 0x1F), 2, "0");
				wrt("/");
				prtDclFmt((long) (((j >> 9) & 0x7F) + 80), 2, "0");
				wrt("  ");

				dt = (short *)&srchb[22];
				j = *dt;
				prtDclFmt((long) ((j >> 11) & 0x1F), 2, "0");
				wrt(":");
				prtDclFmt((long) ((j >> 5) & 0x3F), 2, "0");
				wrt(":");
				prtDclFmt((long) ((j & 0x1F) << 1), 2, "0");
				wrt("  ");

				att = srchb[21];
				if (att < 0x10)
					wrt("0");
				prthex(att);
				wrt("  ");

				pl = (long *) &srchb[26];
				ts += *pl;
				prtDclFmt((long) *pl, 6, " ");
			}
		  skip:
			;
		} while (gtFlNm());
		wrtln("");
		if (!terse)
		{
			wrt("       ");
			prtDclFmt((long) tf, 3, " ");
			wrt(" files using ");
			prtDclFmt(ts, 7, " ");
			wrt(" bytes of disk.");
			wrtln("");
		}
	}
	return compl_c;
}

/***************************************************************************/
/***************************************************************************/
long chmodCmd(P(char **) argv)
PP(char **argv;)
{
	char srcSpc[PATH_MAX];
	register int i, att, mode;
	register long compl_c;

	if (!*argv[2])
	{
		wrt("Mode specification needed for CHMOD command.");
		compl_c = -1;
	} else
	{
		mode = mknum(argv[2]);
		if (mode & ~0x7)
		{
			wrt("Invalid mode specification.");
			compl_c = -1;
		} else
		{
			for (i = 0; (srcSpc[i] = argv[1][i]) != 0; i++)
				;

			if (chkDir(srcSpc, srcDir, srcNmPat, 0) == -4)
			{
				dspMsg(18);
				return -1;
			}
			WSrcReq = srcSpc;
			WAttCode = -1;
			if (!gtFlNm())
			{
				compl_c = -1;
				dspMsg(1);
			} else
			{
				compl_c = 0;
				do
				{
					mkSrc();
					att = srchb[21];
					if (att & 0x18)
					{
						wrt("Unable to change mode on subdirectorys or volumes.");
						compl_c = -1;
					} else
					{
						compl_c = Fattrib(srcFlNm, 1, mode);
					}
				} while (gtFlNm());
				if (compl_c >= 0)
					dspMsg(5);
			}
		}
	}
	return compl_c;
}

/***************************************************************************/
/***************************************************************************/
long typeCmd(P(char **) argv)
PP(char **argv;)
{
	char srcSpc[PATH_MAX];
	register int i, j, fd;
	long compl_c;
	char *p;
	long n, count;

	compl_c = 0;
	if (!(*argv[1]))
	{
		dspMsg(6);
	} else
	{
		j = 1;
		while (*argv[j])
		{
			for (i = 0; (srcSpc[i] = argv[j][i]) != 0; i++)
				;
			j++;
			if (chkDir(srcSpc, srcDir, srcNmPat, 0) == -4)
			{
				dspMsg(18);
				return -1;
			}
			p = srcNmPat;
			while (*p)
			{
				if (*p == '*' || *p == '?')
				{
					dspMsg(0);
					return -1;
				}
				p++;
			}
			WSrcReq = srcSpc;
			WAttCode = -1;
			if (!gtFlNm())
			{
				dspMsg(1);
				compl_c = -1;
			} else
			{
				do
				{
					if (srchb[21] & 0x10)
					{
						dspMsg(0);
						return -1;
					}
					mkSrc();
					if ((fd = Fopen(srcFlNm, O_RDONLY)) <= 0)
					{
						wrtln("Cannot open file ");
						wrt(srcFlNm);
						return -1;
					}
					do
					{
						n = Fread(fd, (long)BUFSIZ, buf);
						if (n > 0)
						{
							count = Fwrite(1, (long) n, buf);
							if (count != n)
								wrerr();
						}
					} while (n > 0);
				} while (gtFlNm());
				Fclose(fd);
			}
		}
	}
	return compl_c;
}

/***************************************************************************/
/***************************************************************************/
long delCmd(P(char **) argv)
PP(char **argv;)
{
	char srcSpc[PATH_MAX];
	register int i, j, k, file_del, query;
	long compl_c;

	file_del = FALSE;
	compl_c = 0;
	query = 0;
	i = 1;
	while (*argv[i])
	{
		ucase(argv[i]);
		if (*argv[i] == '-')
		{
			switch (*(argv[i] + 1))
			{
			case 'Q':
				query = -1;
				break;
			default:
				break;
			}
			j = i;
			while (*(argv[j] = argv[j + 1]))
				j++;
		} else
		{
			i++;
		}
	}

	if (*argv[1])
	{
		k = 1;
		while (*argv[k])
		{
			for (i = 0; (srcSpc[i] = argv[k][i]) != 0; i++)
				;
			k++;
			WSrcReq = srcSpc;
			WAttCode = -1;
			if (((i = chkDir(srcSpc, srcDir, srcNmPat, 0)) == -3) || (i == 1))
			{
				wrt("Delete ALL files matching ");
				wrt(srcDir);
				wrt(srcNmPat);
				wrt(" (Y/CR)? ");
				if (!getYes())
					goto noera;
				wrtln("");
			} else if (i == -4)
			{
				dspMsg(18);
				return -1;
			}
			if (!gtFlNm())
			{
				compl_c = -1;
				dspMsg(1);
				wrtln("");
			} else
			{
				do
				{
					if (!(srchb[21] & 0x18))
					{
						mkSrc();
						dspMsg(7);
						if (srchb[21] & 0x01)
						{
							wrt(" IS READ-ONLY FILE, NOT DELETED");
							compl_c = -36;
							goto skipdel;
						} else if (query)
						{
							wrt("? ");
							i = getYes();
							wrt("\b\b ");
							if (i)
								wrt(" << DELETED");
							else
								goto skipdel;
						}
						compl_c = Fdelete(srcFlNm);
						file_del = TRUE;
					  skipdel:
						dspMsg(12);
					}
				} while (gtFlNm());
			}
		}
		if (file_del)
			dspMsg(5);
		else
			wrtln("No file deleted.");
	} else
		dspMsg(6);

  noera:
	return compl_c;
}

/***************************************************************************/
/*
   dspCL - display command line
*/
/***************************************************************************/
VOID dspCL(NOTHING)
{
	dspMsg(14);
	wrt(lin);
	wrtln("");
}

/***************************************************************************/
/*
   preCmd - Do pre command processing
*/
/***************************************************************************/
VOID preCmd(P(struct rdb *) rd, P(long) newsi, P(long) newso)
PP(struct rdb *rd;)
PP(long newsi;)
PP(long newso;)
{
	if (rd->echocmd)
		dspCL();
	if (rd->nso)
	{
		Fforce(1, (int) newso);
		Fclose((int) newso);
	}
	if (rd->nsi)
	{
		Fforce(0, (int) newsi);
		Fclose((int) newsi);
	}
}

/***************************************************************************/
/*
   setPath - set execution path
*/
/***************************************************************************/
long setPath(P(char *) p)
PP(char *p;)
{
	char pathbuf[PATH_MAX], c;
	register int i;
	register char *q, *s;

	compl_code = 0;
	if (!*p)
		wrt(path);
	else if (xncmps(2, p, ";"))
		path[0] = 0;
	else
	{
		s = p;
		while (*s)
		{
			q = pathbuf;
			while (*s != ';' && *s != '\0')
				*q++ = *s++;
			*q = '\0';
			if (*s == ';')
				s++;
			q = pathbuf + 3;
			if (*q-- == 0 && *q-- == '\\' && *q-- == ':')
			{
				i = Dgetdrv();
				c = *q;
				if (c >= 'a' && c <= 'z')
					c -= 'a';
				else if (c >= 'A' && c <= 'Z')
					c -= 'A';
				else
					c = 60;				/* just set it illegal */
				if (c < 0 || c > 15 || Dsetdrv((int) c) < 0)
				{
					Dsetdrv(i);
					return (compl_code = -1L);
				}
				Dsetdrv(i);				/*restore the old drive */
			} else
			{
				q = pathbuf;				/* scan pathbuf to see any * or ? embeded */
				while (*q && *q != '*' && *q != '?')
					q++;
				if (*q || Fsfirst(pathbuf, FA_SUBDIR) < 0)
					return (compl_code = -1L);
			}
		}
		q = path;
		s = p;
		while ((*q++ = *s++) != '\0')
			;
	}
	return compl_code;
}


/***************************************************************************

	execPrgm - execute program

***************************************************************************/
long execPrgm(P(char *) s, P(char *) cmdtl)
PP(char *s;)
PP(char *cmdtl;)
{
	char cmd[130];
	char ch;
	char *cmdptr;
	register int k, i, j, gtpath, envLen;
	int tlNtFnd = -1;
	long err;
	register char *envPtr;

	/* Loop thru environment strings looking for '00.  Don't count
	 * any PATH=... definitions in the environment because the user
	 * may have redefined the PATH, which is now in path[].
	 * The last string in the environment is followed by second NULL.
	 */

	envLen = 1;							/* one byte for last NULL   */
	envPtr = prntEnvPtr;
	while (*envPtr)						/* while not at last NULL   */
	{
		i = xstrlen(envPtr) + 1;			/* get length + 1 for NULL  */
		if (!xncmps(5, envPtr, pthSymb))	/* if not PATH=...      */
			envLen += i;				/* add length of this string    */
		envPtr += i;					/* move to next string      */
	}

	/* Add len of path definition */
	if ((i = xstrlen(path)) != 0)		/* is a new path defined?   */
		envLen += sizeof(pthSymb) + i + 1;	/* space for PATH=... and NULL  */

	/* Allocate envLen number of bytes for environment strings. */
	envPtr = (char *)Malloc((long) envLen);

	/* copy path string into env. */
	i = 0;
	if (path[0])
	{
		for (i = 0; pthSymb[i]; i++)
			envPtr[i] = pthSymb[i];
		j = 0;
		while ((envPtr[i] = path[j++]) != 0)
			i++;
		envPtr[i++] = 0;
	}

	/* Copy parents environment string into childs. */
	envLen = 0;
	while ((envPtr[i] = prntEnvPtr[envLen]) | prntEnvPtr[envLen + 1])
	{
		/* if a path has been defined, don't copy it. */
		if (xncmps(5, &prntEnvPtr[envLen], pthSymb))
			envLen += (1 + xstrlen(&prntEnvPtr[envLen]));
		else
		{
			i++;
			envLen++;
		}
	}

	/* inc index past 0. */
	i++;

	/* Null termintate. */
	envPtr[i] = 0;

	for (i = 0; (cmd[i] = *s) != 0 && (i < sizeof(cmd)); s++, i++)
		if (*s == '.')
			tlNtFnd = 0;

	if (tlNtFnd)
		for (j = 0; (cmd[i] = prgTail[j]) != 0 && (i < sizeof(cmd)); i++, j++)
			;

	i = 0;
	gtpath = -1;
	while ((ch = cmd[i++]) != 0 && (i <= sizeof(cmd)))
		if (ch == ':' || ch == '\\')
			gtpath = 0;

	exeflg = 1;

	super();
	rm_term();
	user();

	cmdptr = cmd;
	j = 0;
	while ((err = Pexec(0, cmdptr, cmdtl, envPtr)) == -33 && gtpath)
	{
		k = j;
		if (path[j])
		{
			while ((path[j]) && (path[j] != ';'))
				j++;
			for (i = 0; k < j; k++, i++)
				buf[i] = path[k];
			if (buf[i - 1] != '\\')
				buf[i++] = '\\';
			k = 0;
			while (cmd[k])
				buf[i++] = cmd[k++];
			buf[i] = 0;
			cmdptr = buf;
			if (!(path[j]))
				gtpath = 0;
			else
				j++;
		} else
			gtpath = 0;
	}

	super();
	in_term();
	user();

	exeflg = 0;
	Mfree(envPtr);

	return err;
}

/**************************************************************************/
/*
   execBat - execute batch file
*/
/***************************************************************************/
int execBat(P(char *) s, P(char **) parms)
PP(char *s;)
PP(char **parms;)
{
	long flHnd;
	int i;
	register int j, k;
	int gtpath;
	int tlNtFnd = -1;
	register char ch;
	char cmd[130];
	char *cmdptr;

	for (i = 0; (cmd[i] = *s) != 0 && (i < sizeof(cmd)); s++, i++)
		if (*s == '.')
			tlNtFnd = 0;

	if (tlNtFnd)
	{
		for (j = 0; (cmd[i] = batTail[j]) != 0 && (i < sizeof(cmd)); i++, j++)
			;
	}
	
	if (xncmps(3, &cmd[i - 3], "BAT"))
	{
		i = 0;
		gtpath = -1;
		while ((ch = cmd[i++]) != 0 && (i <= sizeof(cmd)))
			if (ch == ':' || ch == '\\')
				gtpath = 0;

		cmdptr = cmd;
		j = 0;
		while (((flHnd = Fopen(cmdptr, O_RDONLY)) <= 0) && (gtpath))
		{
			k = j;
			if (path[j])
			{
				while (path[j] && path[j] != ';')
					j++;
				for (i = 0; k < j; k++, i++)
					buf[i] = path[k];
				if (buf[i - 1] != '\\')
					buf[i++] = '\\';
				k = 0;
				while (cmd[k])
					buf[i++] = cmd[k++];
				buf[i] = 0;
				cmdptr = buf;
				if (!(path[j]))
					gtpath = 0;
				else
					j++;
			} else
				gtpath = 0;
		}

		if (flHnd >= 0)
		{
			i = 0;
			if (++batlev > 4)
			{
				wrtln("too many levels of batch files nested.");
				compl_code = -1;
				batlev = 0;
				if (prgerr)
					errout();
				return -1;
			}
			xCmdLn(parms, &i, (int)flHnd, NULL);
			Fclose((int) flHnd);
			--batlev;
			compl_code = 0;
			return -1;
		}
	}

	compl_code = -1;
	return 0;
}

/***************************************************************************/
/***************************************************************************/
int mknum(P(char *) str)
PP(char *str;)
{
	register int num, hex;
	register char ch;

	hex = 0;
	ucase(str);
	if (*str == 'X')
	{
		hex = 1;
		str++;
	}

	num = 0;
	while ((ch = *str++) != 0)
	{
		if (hex)
		{
			num *= 16;
			if (ch > 9)
				num += (ch - 'A' + 10);
			else
				num += ch - '0';
		} else
		{
			num *= 10;
			num += ch - '0';
		}
	}
	return num;
}

/***************************************************************************/
/***************************************************************************/
int chk_sub(P(char *) tl, P(char **) parm)
PP(char *tl;)
PP(char **parm;)
{
	char ch;
	char tmptl[167];
	char *tmptl_ptr;
	char *tmp_front;
	char *tl_front;
	char *parm_ptr;

	tmptl_ptr = tmptl;
	tmp_front = tmptl_ptr;
	tl_front = tl;

	while ((ch = *tl++) != 0)
	{
		switch (ch)
		{
		case '\\':
			if (*tl == '%')
			{
				*tmptl_ptr++ = ch;
				*tmptl_ptr++ = *tl++;
			} else
			{
				*tmptl_ptr++ = ch;
			}
			break;

		case '"':
			*tmptl_ptr++ = ch;
			while (*tl && (*tmptl_ptr++ = *tl++) != '"')
				;
			break;

		case '%':
			if (*tl > '9' || *tl < '0')
				return -1;
			if (*(parm_ptr = parm[*tl++ - '0']))
			{
				while ((*tmptl_ptr = *parm_ptr++) != 0)
					tmptl_ptr++;
			}
			break;

		default:
			*tmptl_ptr++ = ch;
			break;
		}
	}
	*tmptl_ptr = 0;

	while ((*tl_front = *tmp_front++) != 0)
		tl_front++;
	return 0;
}

/***************************************************************************/
/***************************************************************************/
VOID chk_str(P(char **) parm)
PP(char **parm;)
{
	register int i;
	register char *parm_ptr;
	register char *tmp_ptr;
	register char ch;

	i = 0;
	while (*parm[i])
	{
		if (*parm[i] == '"')
		{
			parm[i]++;
			if (*(parm[i] + xstrlen(parm[i]) - 1) == '"')
				*(parm[i] + xstrlen(parm[i]) - 1) = 0;

			parm_ptr = parm[i];
			while ((ch = *parm_ptr) != 0)
			{
				if (ch == '\\')
				{
					switch (ch = *(parm_ptr + 1))
					{
					case 'c':
						*parm_ptr++ = 13;
						*parm_ptr = 10;
						goto skip;
					case 'r':
						*parm_ptr = 13;
						break;
					case 'n':
						*parm_ptr = 10;
						break;
					case '0':
						*parm_ptr = '\0';
						break;
					default:
						*parm_ptr = ch;
						break;
					}
					parm_ptr++;
					tmp_ptr = parm_ptr;
					while ((*tmp_ptr = *(tmp_ptr + 1)) != 0)
						tmp_ptr++;
				  skip:
				  	;
				}

				else
				{
					parm_ptr++;
				}
			}
		}
		i++;
	}
}

/***************************************************************************/
/*
   readSi - read standard input
*/
/***************************************************************************/
long readSi(P(char *) llin)
PP(register char *llin;)
{
	register int i, j;

	dspMsg(13);
	for (i = 1; i <= 125; llin[i++] = 0)
		;
	i = j = 0;

	llin[0] = 126;
	Cconrs(llin);

	llin[llin[1] + 2] = 0;

	i = 2;
	while (llin[i])
	{
		if ((llin[i] >= ' ') && (llin[i] <= '~'))
			llin[j++] = llin[i];
		i++;
	}

	llin[j] = 0;
	llin[j + 1] = 0;
	return 1;
}

/***************************************************************************/
/*
   readDsk - read from disk file
*/
/***************************************************************************/
int readDsk(P(char *) llin, P(int) flHnd)
PP(register char *llin;)
PP(int flHnd;)
{
	register int i, j;
	long chrFnd;
	char ch;

	j = 0;
	while ((chrFnd = Fread(flHnd, 1L, &ch)) > 0)
	{
		if (ch == 0x0a)
			break;
		if (ch == 0x0d)
		{
			if (Fread(flHnd, 1L, &ch) > 0)
			{
				if (ch != 0x0a)
					Fseek(-1L, flHnd, SEEK_CUR);
			}
			break;
		}
		if (j < 126)
			llin[j++] = ch;
	}
	llin[j] = '\0';
	
	j = 0;
	i = 0;
	while (llin[i])
	{
		if (llin[i] >= ' ' && llin[i] <= '~')
			llin[j++] = llin[i];
		i++;
	}

	llin[j] = 0;

	return j ? j : (chrFnd ? -1 : 0);
}


static VOID initCmd(P(char *) p)
PP(register char *p;)
{
	register int i, j;
	register BPB *b;
	register char *d;
	register int f1;
	register int f2;
	register int fs;
	register int nd;
	register int rec;
	
	for (i = 0; i < BUFSIZ; i++)
		buf[i] = 0;
	i = Dgetdrv();			/* default drive */
	if (!*p)
	{
		drv = i;
	} else
	{
		ucase(p);
		drv = *p - 'A';
		if (drv < 0 || drv > 31 || Dsetdrv(drv) < 0)
		{
			dspMsg(18);
			wrt(p);
			Dsetdrv(i);		/* restore the default drive */
			return;
		}
		Dsetdrv(i);
	}
	buf[0] = 0xf7;
	buf[1] = 0xff;
	buf[2] = 0xff;
	super();
	b = (BPB *)Getbpb(drv);
	if (b->b_flags & 1)
		buf[3] = 0xFF;
	f1 = b->fatrec - b->fsiz;
	f2 = b->fatrec;
	fs = b->fsiz;
	Rwabs(1, buf, fs, f1, drv);
	Rwabs(1, buf, fs, f2, drv);
	nd = b->recsiz / 32;
	d = buf;
	for (i = 0; i < nd; i++)
	{
		*d++ = 0;
		for (j = 0; j < 31; j++)
			*d++ = 0;		/* formerly f6 */
	}
	rec = f2 + fs;
	for (i = 0; i < b->rdlen; i++, rec++)
		Rwabs(1, buf, 1, rec, drv);
	user();
	dspMsg(5);
}

/***************************************************************************/
/*
   xCmdLn - execute command line.
*/
/***************************************************************************/
VOID xCmdLn(P(char **) parm, P(int *) pipeflg, P(int) nonStdIn, P(char *) outsd_tl)
PP(char **parm;)
PP(int *pipeflg;)
PP(int nonStdIn;)
PP(char *outsd_tl;)
{
	int pipe, bdChrs;
	int fd, n;
	register int i, j, k, argc;
	int concat;
	long newso, newsi;
	long sbuf[4];
	long dskFlHnd;
	char *d;
	char ch, *cmdtl, *s;
	char *argv[MAXARGS];
	register char *tl, *tl0, *tl1;
	char *p, ltail[130];
	long count, bytes;
	struct rdb rd;

	rd.nso = FALSE;
	rd.nsi = FALSE;
	rd_ptr = &rd;
	rd.nonstdin = nonStdIn;
	rd.echocmd = nonStdIn != 0;
	newsi = 0;
	newso = 0;

	/* while there is input from the disk or standard input */
	while ((bdChrs = outsd_tl ? 1 : nonStdIn ? readDsk(lin, rd.nonstdin) : readSi(lin)) != 0)
	{
		/* Garbage chars in disk file. */
		if (bdChrs == -1 && rd.nonstdin)
			goto again;

		exeflg = 0;						/* not in an exec */
		wrtln("");
		d = ltail;
		argv[0] = d;
		argc = 0;
		concat = 0;
		pipe = 0;
		dskFlHnd = 0;

		/* Set up for input redirection. */
		if (*pipeflg)
		{
			argv[0] = parm[0];
			argv[1] = d;
			argc = 1;
		}

		/* find command tail */
		if (outsd_tl)
		{
			tl = outsd_tl + 1;
			tl[(__uint8_t)outsd_tl[0]] = 0;
		} else
		{
			tl = lin;
		}
		
		while (*tl == ' ')
			tl++;
		if (chk_sub(tl, parm))
		{
			wrt(" Illegal usage of %.");
			goto again;
		}

		/* allow remarks in batch files. */
		if (rd.nonstdin && *tl == '#')
			goto again;

		while ((ch = *tl++) != 0)
		{
			switch (ch)
			{
			case ' ':
				*d++ = 0;
				while (*tl == ' ')
					tl++;
				argv[++argc] = d;
				break;

			case '"':
				*d++ = ch;
				while ((*tl) && ((*d++ = *tl++) != '"'))
					;
				break;

			case '@':
				for (tl0 = tl; (ch = *tl) != 0; tl++)
				{
					if (ch == ' ')
						break;
				}
				*tl++ = 0;
				if (!(*tl0))
				{
					wrt("File name not specified after @.");
					goto redrc;
				}
				if ((dskFlHnd = Fopen(tl0, O_RDONLY)) >= 0)
				{
					pipe = -1;
				} else
				{
					wrtln(tl0);
					wrt(" not found.");
					goto redrc;
				}
				break;

			case '<':
				for (tl0 = tl; (ch = *tl) != 0; tl++)
				{
					if (ch == ' ')
						break;
				}
				*tl++ = 0;

				if ((newsi = Fopen(tl0, O_RDONLY)) >= 0)
				{
					rd.oldsi = Fdup(0);
					rd.nsi = TRUE;
				} else
				{
					wrtln(tl0);
					wrt(" not found.");
					goto redrc;
				}
				break;

			case '>':
				for (tl1 = tl; (ch = *tl) != 0; tl++)
				{
					if (ch == '>')
					{
						if (concat)
						{
							wrtln(" Illegal output file name : ");
							wrt(tl1);	/* takes care the case: >>> was in the command line */
							goto redrc;
						} else
						{
							concat = -1;
							tl1++;
						}
					}
					if (ch == ' ')
						break;
				}
				*tl++ = 0;
				if (*tl1 == '@' || *tl1 == '<')
				{
					wrtln("Illegal outputfile name:");
					wrt(tl1);
					goto redrc;
				}
				if (concat)
				{
					if ((newso = Fopen(tl1, O_WRONLY)) < 0)
					{
						if ((newso = Fcreate(tl1, 0)) < 0)
						{
							wrtln(" Error of opening/creating output:");
							wrt(tl1);
							goto redrc;
						}
					}
				} else
				{
					Fdelete(tl1);
					if ((newso = Fcreate(tl1, 0)) < 0)
					{
						wrtln(" Error of creating output:");
						wrt(tl1);
						goto redrc;
					}
				}
				rd.oldso = Fdup(1);
				rd.nso = TRUE;
				if (concat)
					Fseek(0L, (int) newso, 2);
				break;
			default:
				*d++ = ch;
			}
		}
		
		/* If pipe tack on remaining parms if any. */
		if (*pipeflg)
		{
			i = 1;
			while (*parm[i])
			{
				argv[++argc] = parm[i++];
			}
		}

		*d++ = 0;
		*d = 0;
		i = argc;
		argv[++i] = d;

		s = argv[0];
		p = argv[1];
		ucase(s);

		if (pipe)
		{
			preCmd(&rd, newsi, newso);
			xCmdLn(argv, &pipe, (int)dskFlHnd, NULL);
			Fclose((int) dskFlHnd);
		} else
		{
			if (xstrlen(s) == 2 && s[1] == ':')
			{
				preCmd(&rd, newsi, newso);
				drv = *s - 'A';
				if (drv < 0 || drv > 31 || Dsetdrv(drv) < 0)
				{
					dspMsg(18);
					wrt(s);
				}
			} else if (xncmps(3, s, "LS") || xncmps(4, s, "DIR"))
			{
				preCmd(&rd, newsi, newso);
				compl_code = dirCmd(argv);
			} else if (xncmps(6, s, "CHMOD"))
			{
				preCmd(&rd, newsi, newso);
				compl_code = chmodCmd(argv);
			} else if (xncmps(4, s, "ERR"))
			{
				preCmd(&rd, newsi, newso);
				wrt("Completion code for previous command = ");
				prthex((int) compl_code);
			} else if (xncmps(5, s, "PATH"))
			{
				preCmd(&rd, newsi, newso);
				ucase(p);
				if (setPath(p))
				{
					dspMsg(18);
					wrt(p);
				}
			} else if (xncmps(4, s, "ENV"))
			{
				preCmd(&rd, newsi, newso);
				i = 0;
				while ((prntEnvPtr[i] + prntEnvPtr[i + 1]) != 0)
				{
					/* if a path has been defined, don't count it. */
					if (!xncmps(5, &prntEnvPtr[i], pthSymb))
					{
						wrtln(&prntEnvPtr[i]);
					}
					i += xstrlen(&prntEnvPtr[i]);
					if (prntEnvPtr[i] + prntEnvPtr[i + 1] == 0)
						break;
					i += 1;
				}
				if (path[0])
				{
					wrtln(pthSymb);
					wrt(path);
				}
			} else if (xncmps(4, s, "CAT") || xncmps(4, s, "TYPE"))
			{
				preCmd(&rd, newsi, newso);
				compl_code = typeCmd(argv);
			} else if ((xncmps(4, s, "REM")) || (xncmps(4, s, "ECHO")))
			{
				rd.echocmd = FALSE;	/* don't want to print the command */
				preCmd(&rd, newsi, newso);
				rd.echocmd = rd.nonstdin != 0;
				chk_str(&argv[1]);
				i = 1;
				while (*argv[i])
				{
					wrt(argv[i++]);
					wrt(" ");
				}
				dspMsg(12);
			} else if (xncmps(3, s, "CD"))
			{
				preCmd(&rd, newsi, newso);
				if (argc == 0)
				{
					Dgetpath(buf, drv + 1);
					if (!buf[0])
					{
						buf[0] = '\\';
						buf[1] = 0;
					}
					bytes = xstrlen(buf);
					count = Fwrite(1, bytes, buf);
					if (count != bytes)
						wrerr();
				} else
				{
					if (chk_wild(p))
						if ((compl_code = Dsetpath(p)) != 0)
							wrt("Directory not found.");
				}
			} else if (xncmps(7, s, "CMDERR"))
			{
				preCmd(&rd, newsi, newso);
				if (argc == 0)
				{
					if (cmderr)
						wrt("ON");
					else
						wrt("OFF");
				} else
				{
					ucase(p);
					if (xncmps(3, p, "ON"))
						cmderr = -1;
					else if (xncmps(4, p, "OFF"))
						cmderr = 0;
					else
						wrt("Arg must be ON or OFF.");
				}
			} else if (xncmps(7, s, "PRGERR"))
			{
				preCmd(&rd, newsi, newso);
				if (argc == 0)
				{
					if (prgerr)
						wrt("ON");
					else
						wrt("OFF");
				} else
				{
					ucase(p);
					if (xncmps(3, p, "ON"))
						prgerr = -1;
					else if (xncmps(4, p, "OFF"))
						prgerr = 0;
					else
						wrt("Arg must be ON or OFF.");
				}
			} else if (xncmps(3, s, "MD"))
			{
				preCmd(&rd, newsi, newso);
				if (chk_wild(p))
				{
					if ((compl_code = Dcreate(p)) != 0)
						wrt("Unable to make directory");
				}
			} else if (xncmps(3, s, "RD"))
			{
				preCmd(&rd, newsi, newso);
				if (!(*p))
				{
					dspMsg(6);
				} else
				{
					if (chk_wild(p))
					{
						if ((*p == '\\' && *(p + 1) == 0) ||
							(*(p + 1) == ':' && (*(p + 2) == 0 ||
							 (*(p + 2) == '\\' && *(p + 3) == 0))))
						{
							wrt("Cannot remove root directory\n");
							compl_code = -1L;
						} else if ((compl_code = Ddelete(p)) != 0)
						{
							wrt("Unable to remove directory");
						}
					}
				}
			} else if (xncmps(3, s, "RM") || xncmps(4, s, "DEL") || xncmps(4, s, "ERA"))
			{
				preCmd(&rd, newsi, newso);
				compl_code = delCmd(argv);
			} else if (xncmps(4, s, "REN"))
			{
				preCmd(&rd, newsi, newso);
				if (argc < 2)
				{
					dspMsg(6);
				} else
				{
					renmCmd(argv[1], argv[2]);
				}
			} else if (xncmps(5, s, "SHOW"))
			{
				preCmd(&rd, newsi, newso);
				ucase(p);
				if (*p)
					drv = *p - 'A' + 1;
				else
					drv = 0;
				if (drv < 0 || drv > 32 || (drv != 0 && p[1] != ':' && p[1] != '\0'))
				{
					dspMsg(18);
					wrt(p);
				} else if (Dfree(sbuf, drv) < 0)
				{
					wrt("Unable to get drive information: ");
					wrt(p);
				} else
				{
					wrt("Allocation Information: Drive ");
					if (!*p)
						dspMsg(16);
					else
						wrt(p);
					dspMsg(12);
					wrtln("Drive size in BYTES    ");
					prtDclFmt((long) (sbuf[1] * sbuf[3] * sbuf[2]), 8, " ");
					wrtln("BYTES used on drive    ");
					prtDclFmt((long) ((sbuf[1] - sbuf[0]) * sbuf[3] * sbuf[2]), 8, " ");
					wrtln("BYTES left on drive    ");
					prtDclFmt((long) (sbuf[0] * sbuf[3] * sbuf[2]), 8, " ");
					wrtln("Total Units on Drive   ");
					prtDclFmt((long) sbuf[1], 8, " ");
					wrtln("Free Units on Drive    ");
					prtDclFmt((long) sbuf[0], 8, " ");
					wrtln("Sectors per Unit       ");
					prtDclFmt((long) sbuf[3], 8, " ");
					wrtln("Bytes per Sector       ");
					prtDclFmt((long) sbuf[2], 8, " ");
				}
			} else if (xncmps(5, s, "INIT"))
			{
				preCmd(&rd, newsi, newso);
				initCmd(p);
			} else if (xncmps(8, s, "PUTBOOT"))
			{
				preCmd(&rd, newsi, newso);
				ucase(p);
				drv = *p - 'A';
				fd = Fopen(argv[2], O_RDONLY);
				Fread(fd, 540L, buf);
				Fclose(fd);
				super();
				Rwabs(1, &buf[28], 1, 0, drv);
				user();
				dspMsg(5);
			} else if ((xncmps(5, s, "COPY")) || (xncmps(5, s, "MOVE")))
			{
				preCmd(&rd, newsi, newso);
				if (argc >= 1)
					compl_code = copyCmd(p, argv[2], xncmps(5, s, "MOVE") ? 1 : 0);
				else
					dspMsg(6);
			} else if (xncmps(4, s, "GET"))
			{
				preCmd(&rd, newsi, newso);
				ucase(p);
				cpmopen(p);
				fd = Fcreate(argv[2], 0x20);

				do
				{
					n = cpmread(buf);
					if (!n)
						Fwrite(fd, 128L, buf);
				} while (!n);

				Fclose(fd);
				dspMsg(5);
			} else if (xncmps(6, s, "PAUSE"))
			{
				preCmd(&rd, newsi, newso);
				cr2cont();
			} else if (xncmps(4, s, "PUT"))
			{
				preCmd(&rd, newsi, newso);
				cpmcreate(argv[2]);
				fd = Fopen(p, O_RDONLY);

				do
				{
					n = Fread(fd, 128L, buf);
					if (n > 0)
						compl_code = cpmwrite(buf);
				} while (n > 0);

				cpmclose();
				dspMsg(5);
			} else if (xncmps(5, s, "HELP"))
			{
				preCmd(&rd, newsi, newso);
				dspHelp(rd.nso ? 0 : 1);
			} else if (xncmps(6, s, "BREAK"))
			{
				xbrkpt();
			} else if (xncmps(5, s, "EXIT"))
			{
			  exit:
				preCmd(&rd, newsi, newso);
				Fclose(rd.oldsi);
				Fclose(rd.oldso);
				devector();				/* remove vectors */
				Pterm0();
			} else if (xncmps(8, s, "VERSION"))
			{
				preCmd(&rd, newsi, newso);
				i = Sversion();
				prtdecl((long) (i & 0xFF));
				wrt(".");
				prtdecl((long) ((i >> 8) & 0xFF));
			} else if (xncmps(5, s, "WRAP"))
			{
				preCmd(&rd, newsi, newso);
				wrt("\033v");
				dspMsg(5);
			} else if (xncmps(7, s, "NOWRAP"))
			{
				preCmd(&rd, newsi, newso);
				wrt("\033w");
				dspMsg(5);
			} else if (xncmps(4, s, "CLS"))
			{
				rd.echocmd = FALSE;
				preCmd(&rd, newsi, newso);
				wrt("\033H\033J");
				rd.echocmd = rd.nonstdin != 0;
			} else if (xncmps(5, s, "STAY"))
			{
				xlongjmp(&jb, -2);
			} else
			{
				preCmd(&rd, newsi, newso);
				if (*s && chk_wild(s))
				{
					if (!(execBat(s, argv)))
					{
						/* Build command tail.  So lin won't be modified before the command */
						/* was outputed by preCmd which calls dspCL to actually do it */
						cmdtl = lin;
						j = 1;
						i = 1;
						while (*argv[i])
						{
							k = 0;
							while ((cmdtl[++j] = *(argv[i] + k++)) != 0)
								;
							cmdtl[j] = ' ';
							i++;
						}
						cmdtl[j] = 0;
						cmdtl[0] = --j;
						cmdtl[1] = ' ';

						if ((compl_code = execPrgm(s, cmdtl)) == -32)
							errout();
						else if (compl_code > 0 && prgerr)
							errout();
						else if (compl_code < 0)
						{
							wrt("Command '");
							wrt(s);
							wrt("' not found.");
							if (prgerr)
								errout();
						}
					}
				}
			}
		}
	  redrc:
		chk_redirect(&rd);

	  again:
		/* if command coming from outside the command int exit */
		if (outsd_tl)
			goto exit;
	}
}

/***************************************************************************/
/***************************************************************************/

VOID cmain(P(char *) bp)
PP(char *bp;)								/* Base page address */
{
	char *parm[MAXARGS];
	register char *tl;
	int i, k, cmd;
	int err;

	basePage = bp;
	prntEnvPtr = *((char **) (basePage + 0x2C));
	tl = basePage + 0x80;
	cmd = tl[0];
	if (cmd < 0 || cmd >= 127)
		cmd = 0;

	Fsetdta(srchb);
	path[0] = 0;
	compl_code = 0;
	prgerr = -1;
	cmderr = 0;
	if (!cmd)
		dspMsg(4);

	i = 0;
	while ((prntEnvPtr[i] + prntEnvPtr[i + 1]) != 0)
	{
		/* if a path has been defined, don't count it. */
		if (xncmps(5, &prntEnvPtr[i], pthSymb))
		{
			if (setPath(&prntEnvPtr[i + 5]))
			{
				dspMsg(18);
				wrt(&prntEnvPtr[i + 5]);
			}
			break;
		}
		i++;
	}

	if (!cmd)
		execBat(autoBat, parm);

	if ((err = xsetjmp(&jb)) != 0)
	{
		if (err != -2)
		{
			for (i = 6; i <= 20; i++)
				Fclose(i);
			if (cmd)
			{
				wrtln("Stay in COMMAND.PRG (Y/CR)? ");
				if (getYes())
				{
					cmd = 0;
				} else
				{
					tl[0] = 4;
					tl[1] = 'e';
					tl[2] = 'x';
					tl[3] = 'i';
					tl[4] = 't';
					tl[5] = 0;
				}
			}
		}
	}

	for (;;)
	{
		k = 0;
		batlev = 0;
		xCmdLn(parm, &k, 0, cmd ? tl : NULL);
	}
}
