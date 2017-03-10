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
                     
NAMES

	JSL	Jason S. Loveman
	LGT	Lou T. Garavaglia
	SCC	Steven C. Cavender

*/

#include <string.h>

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#ifdef __ALCYON__
#  define BINEXACT 1
#else
#  define BINEXACT 0
#endif

#define _(x) x

struct xjmpbuf {
	long fp;
	long sp;
	long pc;
#if BINEXACT
	int drv;
#define zdrv jb.drv
#endif
};

/*
 * from coma.s
 */
long oscall PROTO((short code, ...));
long bios PROTO((short code, ...));
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
long div10 PROTO((long x));
int mod10 PROTO((long x)); /* BUG: could actually be declared as long */

#define FALSE 0
#define TRUE -1 /* WTF */
#define MAXARGS 20

#define xrdchne() oscall (0x08)
#define xecho(a) oscall (0x02,a)
#define xread(a,b,c) oscall(0x3f,a,b,c)
#define xwrite(a,b,c) oscall(0x40,a,b,c)
#define xopen(a,b) oscall(0x3d,a,b)
#define xclose(a) oscall(0x3e,a)
#define xcreat(a,b) oscall(0x3c,a,b)
#define xforce(a,b) oscall(0x46,a,b)
#define xexec(a,b,c,d) oscall(0x4b,a,b,c,d)
#define dup(a) oscall(0x45,a)
#define xgetdrv() oscall(0x19)
#define xsetdrv(a) oscall(0x0e,a)
#define xsetdta(a) oscall(0x1a,a)
#define xsfirst(a,b) oscall(0x4e,a,b)
#define xsnext() oscall(0x4f)
#define xgetdir(a,b) oscall(0x47,a,b)
#define xmkdir(a) oscall(0x39,a)
#define xrmdir(a) oscall(0x3a,a)
#define xchdir(a) oscall(0x3b,a)
#define xunlink(a) oscall(0x41,a)
#define xrename(a,b,c) oscall(0x56,a,b,c)
#define xgetfree(a,b) oscall(0x36,a,b)
#define xterm(a) oscall(0x4c,a)
#define xf_seek(a,b,c) oscall(0x42,a,b,c)
#define xmalloc(a) oscall(0x48,a);
#define xmfree(a) oscall(0x49,a);
#define xattrib(a,b,c) oscall(0x43,a,b,c)
#define getbpb(a) bios(7,a)
#define rwabs(a,b,c,d,e) bios(4,a,b,c,d,e)

#define BPB struct _bpb
BPB										/* bios parameter block */
{
	short recsiz;
	short clsiz;
	short clsizb;
	short rdlen;							/* root directory length in records */
	short fsiz;							/* fat size in records */
	short fatrec;							/* first fat record (of last fat) */
	short datrec;							/* first data record */
	short numcl;							/* number of data clusters available */
	short b_flags;
};

char zero = { '0' };

char hexch[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
int exeflg;
int rtrnFrmBat;

struct xjmpbuf jb;
#if !BINEXACT
int zdrv;
#endif

long compl_code;

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

char srcFlNm[67];						/* src file name */
char dstFlNm[67];						/* destination file name */
char srcDir[67];						/* src dir path */
char dstDir[67];						/* dst dir path */
char srcNmPat[13];						/* src file name specified in path */
char dstNmPat[13];						/* dst file name specified in path */
char path[67];							/* lst of default path names       */

/* Forward declarations. */
int xncmps PROTO((int n, const char *s, const char *d));
VOID prthex PROTO((unsigned int h));
size_t strlen PROTO((const char *s));
VOID prtdecl PROTO((long d));
VOID pdl PROTO((long d));
VOID prtDclFmt PROTO((long d, int cnt, char *ch));
VOID ucase PROTO((char * s));
int gtFlNm PROTO((NOTHING));
int chkDir PROTO((char *pathExp, char *dirExp, char *filExp));
int chkDst PROTO((NOTHING));
int mkDst PROTO((NOTHING));
VOID mkSrc PROTO((NOTHING));
VOID wrt PROTO((const char *msg));
VOID wrtln PROTO((const char *msg));
VOID dspDir PROTO((char *p, char *dir));
VOID dspMsg PROTO((int msg));
int copyCmd PROTO((char *src, char *dst, int move));
int renmCmd PROTO((char *src, char *dst));
VOID dspCL PROTO((char **argv));
VOID setPath PROTO((char *p));
long execPrgm PROTO((char *s, char *cmdtl));
int execBat PROTO((char *s, char **parms));
int mknum PROTO((char *str));
VOID chk_sub PROTO((char *tl, char **parm));
VOID chk_str PROTO((char **parm));
int readSi PROTO((char *lin));
int readDsk PROTO((char *lin, long *flHnd));
VOID xCmdLn PROTO((char **parm, int *pipeflg, long *nonStdIn));

/* entry point called from coma.S */
VOID cmain PROTO((char *bp));


/***************************************************************************/
/***************************************************************************/
int xncmps(P(int) n, P(const char *) s, P(const char *) d)
PP(int n;)
PP(const char *s;)
PP(const char *d;)
{
	while (n--)
		if (*s++ != *d++)
			return 0;
	return 1;
}

/***************************************************************************/
/***************************************************************************/
VOID prthex(P(unsigned int) h)
PP(unsigned int h;)
{
	unsigned int h2;

	if ((h2 = (h >> 4)) /* != 0 */)
		prthex(h2);
	else
		xwrite(1, 1L, "0");
	xwrite(1, 1L, &hexch[h & 0x0f]);
}

/***************************************************************************/
/***************************************************************************/
size_t strlen(P(const char *) s)
PP(const char *s;)
{
	int n;

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
		xwrite(1, 1L, "0");
}

/***************************************************************************/
/***************************************************************************/
VOID pdl(P(long) d)
PP(long d;)
{
	long d2;

	if ((d2 = div10(d)) /* != 0 */)
		pdl(d2);
	xwrite(1, 1L, &hexch[mod10(d)]);
}

/***************************************************************************/
/***************************************************************************/
VOID prtDclFmt(P(long) d, P(int) cnt, P(char *) ch)
PP(long d;)
PP(int cnt;)
PP(char *ch;)
{
	int i;
	long k, j;

	k = (d ? d : 1);
	j = 1;
	for (i = 1; i < cnt; i++)
		j *= 10;
	while (k < j)
	{
		xwrite(1, 1L, ch);
		k *= 10;
	}
	prtdecl(d);
}

/***************************************************************************/
/***************************************************************************/
VOID ucase(P(char *) s)
PP(char *s;)
{
	for (; *s; s++)
		if (*s >= 'a' && *s <= 'z')
			*s &= ~0x20;
}

/***************************************************************************/
/*
   gtFlNm - get file name of the next file in the directory match a 
      path\wildcat specification. The first invocation makes a call to
      xsfirst.  Each subsequent invocation uses xsnext().  To invoke
      the routine, the wildcarded path name is put into WSrcReq and the
      routine called.  For this and each subseqent call the descriptor block
      block for the found file (if one was found) is pointed to by WThisSrc.


   returns 0 if no match
   returns -1 if file found

*/
/***************************************************************************/

int gtFlNm(NOTHING)
{
	/* First file request? */
	if (WSrcReq != NULL)
	{
		if (xsfirst(WSrcReq, WAttCode))
			return FALSE;

		WSrcReq = NULL;
	}

	/* Subsequent file request	*/
	else
	{
		if (xsnext())
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

  int chkDir (pathExp, dirExp, filExp)

  returns -3 if wild cards specified in path name
          -2 if pathExp does NOT specify a file, directory, or device
          -1 if pathExp evaluates to a subdirectory, or a volume label
           0 if pathExp evaluates to a normal, read-only, or system file
           1 if pathExp evaluates to a normal, read-only, or system file but
             was specified with wild cards

  pathExp - ptr to path expression, w/wo wild cards to be evaluated
  dirExp  - ptr to dir part of pathExp
  filExp  - ptr to file.ext part of pathExp
*/
/***************************************************************************/

int chkDir(P(char *) pathExp, P(char *) dirExp, P(char *) filExp)
PP(char *pathExp;)
PP(char *dirExp;)
PP(char *filExp;)
{
	int pathLen = strlen(pathExp);
	int dirLen;
	int wildLen = strlen(wildExp);
	int wildFnd = FALSE;
	int i = 0;
	int flExsists;
	char c;

	/* directory length = path length to start with. */
	dirLen = pathLen;

	/* Loop thru path expresion from end looking for delimeters and the 1st char.*/
	do
	{
		if ((c = pathExp[dirLen]) == '*')
			wildFnd = TRUE;
	} while ((c != ':') && (c != '\\') && (dirLen--));
	dirLen++;

	/* IF nothing specified defalult to wild expresion.*/
	if (pathLen == 0 || pathExp[pathLen - 1] == '\\' || pathExp[pathLen - 1] == ':')
	{
		while ((pathExp[pathLen] = wildExp[i]) /* != 0 */)
		{
			pathLen++;
			i++;
		}
		wildFnd = TRUE;
	}

	/* return file not found if not found.*/
	flExsists = xsfirst(pathExp, WAttCode) ? -2 : 0;

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
	/* copy path exp into directory expresion.*/
	for (pathLen = 0; pathLen < dirLen; pathLen++)
		if ((dirExp[pathLen] = pathExp[pathLen]) == '*')
			return -3;

	/* chop off file nm from dirExp.*/
	dirExp[dirLen] = '\0';

	/* copy file name into return var.*/
	i = 0;
	while ((filExp[i++] = pathExp[pathLen++]) /* != 0 */)
		;

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
	while ((c = srcNmPat[i++]) /* != 0 */)
	{
		/* Look for wild card chars. */
		switch (c)
		{
		case '*':
		case '?':

		  doDstChk:
			/* If dst file name longer than wild exp, must be error. */
			if ((int)strlen(dstNmPat) > (int)strlen(wildExp))
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
	int i, k, ndx;
	int srcEqDst;
	i = ndx = 0;

	/* determine If dst dir path = src dir path.*/
	while ((srcEqDst = (srcDir[i] == dstDir[i])) && srcDir[i] && (dstFlNm[i] = dstDir[i]))
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

	/* if the entire name matches create a dst file name with '.&&&' as ext*/
	if (srcEqDst)
	{
		i = 0;
		while ((dstFlNm[ndx] = dstDir[ndx]) /* != 0 */)
			ndx++;

		while ((dstFlNm[ndx + i] = srchb[30 + i]) && (dstFlNm[ndx + i] != '.'))
			i++;
		i++;

		for (k = 0; k <= 2; dstFlNm[i + (k++)] = '&')
			;
	}

	/* else file names do not match.*/
	else
	{
		i = 0;
		ndx = 0;

		/* copy dst dir path into dst file name. */
		while ((dstFlNm[ndx] = dstDir[ndx]) /* != 0 */)
			ndx++;

		/* if dst file pat is wild card, copy src file name into dst file name. */
		if (dstNmPat[0] == '*')
			while ((dstFlNm[ndx + i] = srchb[30 + i]) /* != 0 */)
				i++;

		else
			/* copy dst name pat directly into dst file name. */
			while ((dstFlNm[ndx + i] = dstNmPat[i]) /* != 0 */)
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
	int i;
	int j = 0;

	/* copy src directroy into src directory file name.*/
	for (i = 0; (srcFlNm[i] = srcDir[i]); i++)
		;

	/* copy source file name from search first/next into src file name.*/
	while ((srcFlNm[i + j] = srchb[30 + j]) /* != 0 */)
		j++;
}

/***************************************************************************/
/*
   wrt - write to standard output
*/
/***************************************************************************/
VOID wrt(P(const char *) msg)
PP(const char *msg;)
{
	xwrite(1, (long)(int)strlen(msg), msg);
}

VOID wrtln(P(const char *) msg)
PP(const char *msg;)
{
	wrt("\r\n");
	wrt(msg);
}

/***************************************************************************/
/*
   dspDir - display directory
*/
/***************************************************************************/
VOID dspDir(P(char *) p, P(char *) dir)
PP(char *p;)
PP(char *dir;)
{
	int i, j;
	char ch;
	char tmpbuf[100];

	if (*p && p[1] == ':')
	{
		ch = *p;
		i = p[0] - 'A';
		j = 2;
	} else
	{
		ch = (i = xgetdrv()) + 'A';
		j = 0;
	}
	xwrite(1, 1L, &ch);
	wrt(":");
	if (!dir[j])
	{
		xgetdir(&tmpbuf, i + 1);
		if (tmpbuf[0] == 0)
		{
			tmpbuf[0] = '\\';
			tmpbuf[1] = 0;
		}
		wrt(tmpbuf);
	} else
		wrt(dir + j);
}

/***************************************************************************/
#if 0
VOID wrtEol(NOTHING)
{
	wrt("\r\n");
}

VOID to(P(int) n)
PP(int n;)
{
	char ch;
	ch = n+'0';
	wrt(" OUT<< ");
	xwrite (1, 1L, &ch);
	wrt ("\r\n");
}

VOID ti(P(int) n)
PP(int n;)
{
	char ch;
	
	ch = n + '0';
	wrt(" IN>> ");
	xwrite(1, 1L, &ch);
	wrtln("");
}
#endif

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
		wrtln("Wild cards not allowed in path name.");
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
		wrtln("Command v0.33 6/26/85 LTG");
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
		drvch = (zdrv = xgetdrv()) + 'a';
		xwrite(1, 1L, &drvch);
		wrt("}");
		break;
	case 15:
		wrtln("Wild cards not allowed in destination.");
		break;
	case 16:
		drvch = (zdrv = xgetdrv()) + 'a';
		xwrite(1, 1L, &drvch);
		wrt(":");
		break;
	case 17:
		wrtln("DIR or LS [filenm.ext] [/f] [/d] [/t]");
		wrtln("    /f - anything but directoryies.");
		wrtln("    /d - directories only.");
		wrtln("    /t - terse: names only.");
		wrtln("PATH [;[pathnm]...]");
		wrtln("    With path name sets default path for batch and commands.");
		wrtln("    Without path name displays current path");
		wrtln("CAT or TYPE filenm.ext");
		wrtln("    Writes filenm.ext to standard output.");
		wrtln("REM or ECHO [\"string\"]");
		wrtln("    Strips quotes and writes string to standard output.");
		wrtln("    \r is replaced by 0x13, \n by 0x10, \0 by 0x0.");
		wrtln("CD [pathnm]");
		wrtln("    With pathnm it sets default for working directory.");
		wrtln("    Without pathnm displays current working directory.");
		wrtln("MD [subdirectory name]");
		wrtln("    Creates a new subdirectory in current directory.");
		wrtln("RD [pathnm]");
		wrtln("    Removes named directory.");
		wrtln("RM or DEL or ERA filenm");
		wrtln("    Removes named file from directory.");
		wrtln("REN source_file_nm [destination_file_nm]");
		wrtln("    Renames source to destination.");
		wrtln("SHOW [drive_spec:]");
		wrtln("    Displays disk status for default drive or drive specified.");
		wrtln("INIT [drive_spec:]");
		wrtln("    Reinitializes FAT entries this wiping disk.");
		wrtln("COPY source_file [destination_file]");
		wrtln("    Copies source to destination.");
		wrtln("MOVE source_file [destination_file]");
		wrtln("    Copies source to destination and deletes source.");
		wrtln("PAUSE");
		wrtln("    Writes 'CR to continue...' to standard output");
		wrtln("    and waits for a carriage return from standard input.");
		wrtln("EXIT");
		wrtln("    Exits CLI to invoking program.");
		wrtln("VERSION");
		wrtln("    Displays current version of OS.");
		wrtln("WRAP");
		wrtln("    Enbles line wrap.");
		wrtln("NOWRAP");
		wrtln("    Disables line wrap.");
		wrtln("CLS");
		wrtln("    Clears the screen.");
		break;
	}
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
	int i, j, srcEqDst, fds, fdd;
	long nr, nw;
	char unused[67];
	
	UNUSED(i);
	UNUSED(j);
	UNUSED(unused);

	WSrcReq = src;
	WAttCode = -1;

	compl_code = 0xFFFFFFFF;

	/* If not a valid file name... */
	switch (chkDir(src, srcDir, srcNmPat))
	{
	case -3:
		dspMsg(0);
		break;
	case -2:
		dspMsg(1);
		break;
	default:
		{
			/* Check destination directory. */
			if (chkDir(dst, dstDir, dstNmPat) == -3)
				dspMsg(0);
			else
			{
				if (chkDst())
					dspMsg(2);

				else
				{
					while (gtFlNm())
					{
						if (!(srchb[21] & 0x18))
						{
							mkSrc();
							if (!(srcEqDst = mkDst()))
							{
								xunlink(dstFlNm);
								dspMsg(7);
								dspMsg(10);
								dspMsg(8);
								dspMsg(12);
								if ((fds = xopen(srcFlNm, 0)) <= 0)
									goto error0;
								if ((fdd = xcreat(dstFlNm, 0x20)) <= 0)
									goto error1;
								compl_code = 0;
								nr = nw = -1;
								while (nr && nw)
								{
									if ((nr = xread(fds, (long) BUFSIZ, buf)) > 0)
									{
										if ((nw = xwrite(fdd, nr, buf)) < nr)
											return -1;
									}
								}
								xclose(fds);
								if (move)
								{
									xunlink(srcFlNm);
								}
								xclose(fdd);
							} else
							{
								wrtln("Cannot copy ");
								dspMsg(7);
								wrt(" to itself.");
								return -2;
							}
						}
					}
					dspMsg(5);
				}
			}
		}
	}
	return 0;

  error0:
	dspMsg(1);
	return -3;
  error1:
	wrtln("Error creating file.");
	xlongjmp(&jb, -1);
#if !BINEXACT
	return -1;
#endif
}

/***************************************************************************/
/* 
   int renmCmd

   renmCmd - rename command

*/
/***************************************************************************/
int renmCmd(P(char *) src, P(char *) dst)
PP(char *src;)
PP(char *dst;)
{
	int i, j, fds, fdd;
	long nl;
	char unused[67];

	UNUSED(nl);
	UNUSED(fdd);
	UNUSED(fds);
	UNUSED(i);
	UNUSED(j);
	UNUSED(unused);

	WSrcReq = src;
	WAttCode = -1;

	/* Set up completion code to show failure */
	compl_code = 0xFFFFFFFF;

	/* IF src not specified err out. */
	if (!(*src))
		dspMsg(6);

	/* If not a valid file name...*/
	else
		switch (chkDir(src, srcDir, srcNmPat))
		{
		case -3:
			dspMsg(0);
			break;
		case -2:
			dspMsg(1);
			break;
		default:
			{
				/* Check destination directory. */
				if (chkDir(dst, dstDir, dstNmPat) == -3)
				{
					dspMsg(15);
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
								if (!mkDst()) ; /* ??? runaway ';'? */
								{
									mkSrc();

									dspMsg(7);
									dspMsg(10);
									dspMsg(8);
									dspMsg(9);
									compl_code = xrename(0, srcFlNm, dstFlNm);
								}
							}
						}
						dspMsg(5);
					}
				}
			}
		}

#if !BINEXACT
	return 0;
#endif
}

/***************************************************************************/
/*
   dspCL - display command line
*/
/***************************************************************************/
VOID dspCL(P(char **) argv)
PP(char **argv;)
{
	int i;

	dspMsg(14);
	i = 0;
	while (*argv[i])
	{
		wrt(argv[i++]);
		wrt(" ");
	}
	dspMsg(12);
}

/***************************************************************************/
/*
   setPath - set execution path
*/
/***************************************************************************/
VOID setPath(P(char *) p)
PP(char *p;)
{
	int i = 0;

	if (!*p)
		wrt(path);
	else if (xncmps(2, p, ";"))
		path[i] = 0;
	else
#if BINEXACT
		while ((path[i] = p[i++]) /* != 0 */) /* BUG: operation on i is undefined */
			;
#else
		while ((path[i] = p[i]) != 0)
			i++;
#endif
}


/***************************************************************************

	execPrgm - execute program

***************************************************************************/
long execPrgm(P(char *) s, P(char *) cmdtl)
PP(char *s;)
PP(char *cmdtl;)
{
	char cmd[100]; /* WTF? why 100? */
	char ch, *cmdptr;
	int k, i, j, gtpath, envLen;
	int tlNtFnd = -1;
	long err;
	char *envPtr;

	/* Add len of path definition + 2 for 00 terminator */
#if BINEXACT
	envLen = ((i = strlen(path)) + (i ? 5 : 0) + 2); /* BUG: operation on i is undefined */
#else
	i = strlen(path);
	envLen = (i + (i ? 5 : 0) + 2);
#endif

	/*Loop thru enviorment strings looking for '00' */
	i = 0;
	while ((prntEnvPtr[i] + prntEnvPtr[i + 1]) != 0)
	{
		/* if a path has been defined, don't count it. */
		if (xncmps(5, &prntEnvPtr[i], "PATH="))
		{
			envLen--;
			i += strlen(&prntEnvPtr[i]);
		} else
		{
			envLen++;
			i++;
		}
	}

	/* Allocate envLen number of bytes for environment strings. */
	envPtr = (char *)xmalloc((long) envLen);

	/* copy path string into env. */
	i = 0;
	if (path[0])
	{
		for (i = 0; pthSymb[i]; i++)
			envPtr[i] = pthSymb[i];
		j = 0;
		while ((envPtr[i] = path[j++]) /* != 0 */)
			i++;
		envPtr[i++] = 0;
	}

	/* Copy parents environment string into childs. */
	envLen = 0;
	while ((envPtr[i] = prntEnvPtr[envLen]) | prntEnvPtr[envLen + 1])
	{
		/* if a path has been defined, don't copy it. */
		if (xncmps(5, &prntEnvPtr[envLen], "PATH="))
			envLen += (1 + strlen(&prntEnvPtr[envLen]));
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

	for (i = 0; (cmd[i] = *s); s++, i++)
		if (*s == '.')
			tlNtFnd = 0;

	if (tlNtFnd)
		for (j = 0; (cmd[i] = prgTail[j]) /* != 0 */; i++, j++)
			;

	i = 0;
	gtpath = -1;
	while ((ch = cmd[i++]) /* != 0 */)
		if (ch == ':' || ch == '\\')
			gtpath = 0;

	exeflg = 1;

	super();
	rm_term();
	user();

	cmdptr = cmd;
	j = 0;
	while ((((err = xexec(0, cmdptr, cmdtl, envPtr)) & 0xFFFFFFFF) == -33) && (gtpath))
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
			cmdptr = &buf[0];
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
	xmfree(envPtr);

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
	int i, j, k, oldsi, gtpath;
	int tlNtFnd = -1;
	char ch;
	char cmd[100];
	char *cmdptr;

	UNUSED(oldsi);
	for (i = 0; (cmd[i] = *s) /* != 0 */; s++, i++)
		if (*s == '.')
			tlNtFnd = 0;

	if (tlNtFnd)
	{
		for (j = 0; (cmd[i] = batTail[j]) /* != 0 */; i++, j++)
			;
	}
	
	if (xncmps(3, &cmd[i - 3], "BAT"))
	{
		i = 0;
		gtpath = -1;
		while ((ch = cmd[i++]) /* != 0 */)
			if (ch == ':' || ch == '\\')
				gtpath = 0;

		cmdptr = cmd;
		j = 0;
		while (((flHnd = xopen(cmdptr, 0)) <= 0) && (gtpath))
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
				cmdptr = &buf[0];
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
			xCmdLn(parms, &i, &flHnd);
			xclose((int) flHnd);
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
	int num, hex = 0;
	char ch;

	ucase(str);
	if (*str == 'X')
	{
		hex = 1;
		str++;
	}

	num = 0;
	while ((ch = *str++) /* != 0 */)
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
VOID chk_sub(P(char *) tl, P(char **) parm)
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

	while ((ch = *tl++) /* != 0 */)
	{
		switch (ch)
		{
		case '\\':
			*tmptl_ptr++ = ch;
			*tmptl_ptr++ = *tl++;
			break;

		case '%':
			if (*(parm[*tl - '0']))
			{
				parm_ptr = parm[*tl++ - '0'];
				while ((*tmptl_ptr = *parm_ptr++) /* != 0 */)
					tmptl_ptr++;
			}
			break;

		default:
			*tmptl_ptr++ = ch;
			break;
		}
	}
	*tmptl_ptr = 0;

	while ((*tl_front = *tmp_front++) /* != 0 */)
		tl_front++;
}

/***************************************************************************/
/***************************************************************************/
VOID chk_str(P(char **) parm)
PP(char **parm;)
{
	int i = 0;
	char *parm_ptr;
	char *tmp_ptr;
	char ch;

	while (*parm[i])
	{
		if (*parm[i] == '"')
			parm[i]++;
		if (*(parm[i] + (int)strlen(parm[i]) - 1) == '"')
			*(parm[i] + (int)strlen(parm[i]) - 1) = 0;

		parm_ptr = parm[i++];
		while ((ch = *parm_ptr) /* != 0 */)
		{
			if (ch == '\\')
			{
				switch (ch = *(parm_ptr + 1))
				{
				case 'r':
					*parm_ptr = 13;
					break;
				case 'n':
					*parm_ptr = 10;
					break;
				case '0':
					*parm_ptr = 0;
					break;
				default:
					*parm_ptr = ch;
					break;
				}
				parm_ptr++;
				tmp_ptr = parm_ptr;
				while ((*tmp_ptr = *(tmp_ptr + 1)) /* != 0 */)
					tmp_ptr++;
			} else
			{
				parm_ptr++;
			}
		}
	}
}

/***************************************************************************/
/*
   readSi - read standard input
*/
/***************************************************************************/
int readSi(P(char *) llin)
PP(char *llin;)
{
	int i, j;

	j = 0;
	dspMsg(13);

	llin[0] = 126;
	oscall(10, &llin[0]);

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
	return j;
}

/***************************************************************************/
/*
   readDsk - read from disk file
*/
/***************************************************************************/
int readDsk(P(char *) llin, P(long *) flHnd)
PP(char *llin;)
PP(long *flHnd;)
{
	int i, j;
	int chrFnd;
	char ch;

	j = 0;

	while ((chrFnd = xread((int) *flHnd, 1L, &ch) > 0) && (ch != '\r'))
		llin[j++] = ch;
	llin[j] = '\0';
	
	j = 0;
	i = 0;

	while (llin[i])
	{
		if ((llin[i] >= ' ') && (llin[i] <= '~'))
			llin[j++] = llin[i];
		i++;
	}

	llin[j] = 0;

	return j ? j : (chrFnd ? -1 : 0);
}

/***************************************************************************/
/*
   xCmdLn - execute command line.
*/
/***************************************************************************/
VOID xCmdLn(P(char **) parm, P(int *) pipeflg, P(long *) nonStdIn)
PP(char **parm;)
PP(int *pipeflg;)
PP(long *nonStdIn;)
{
	int pipe;
	int nsi;
	int nso;
	int bdChrs;
	int n2;
	int fs;
	int fd;
	int n;
	int fds;
	int fdd;
	int rwflg;
	int dummy1;
	int i;
	int j;
	int k;
	int att;
	int argc;
	int f1;
	int f2;
	int nd;
	int rec;
	int oldsi;
	int oldso;
	int concat;
	short *dt;
	int filOnly;
	int dirOnly;
	int terse;
	int nch;
	short dummy2;
	short dummy3;
	char unused[10];
	long *pl;
	long nl;
	long newso;
	long newsi;
	long sbuf[4];
	long dskFlHnd;
	int next;
	char ch;
	char *cmdtl;
	char *tl0;
	char *tl1;
	char *tl;
	char *d;
	char *s;
	char *argv[MAXARGS];
	char *p;
	char *lastp;
	char ltail[130];
	BPB *b;
	
	UNUSED(lastp);
	UNUSED(nl);
	UNUSED(fdd);
	UNUSED(fds);
	UNUSED(next);
	UNUSED(nch);
	UNUSED(n2);
	UNUSED(rwflg);
	UNUSED(dummy1);
	UNUSED(dummy2);
	UNUSED(dummy3);
	UNUSED(unused);
	
	nsi = nso = 0;
	oldsi = dup(0);
	oldso = dup(1);
	
	/* while there is input from the disk or standard input */
#if !BINEXACT
	bdChrs = 0;
#endif
	while ((*nonStdIn ? (bdChrs = readDsk(lin, nonStdIn)) : readSi(lin)))
	{
		/* Garbage chars in disk file. */
		if (bdChrs == -1 && *nonStdIn)
			goto again;

		exeflg = 0;						/* not in an exec */
		wrtln("");
		d = &ltail[0];
		argv[0] = d;
		argc = 0;
		concat = 0;
		pipe = 0;
		dskFlHnd = 0;

		/*Set up for input redirection. */
		if (*pipeflg)
		{
			argv[0] = parm[0];
			argv[1] = d;
			argc = 1;
		}

		/* find command tail */
		tl = &lin[0];

		while (*tl == ' ')
			tl++;
		chk_sub(tl, &parm[0]);

		while ((ch = *tl++) /* != 0 */)
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
				for (tl0 = tl; (ch = *tl) /* != 0 */; tl++)
				{
					if (ch == ' ')
						break;
				}
				*tl++ = 0;

				if ((dskFlHnd = xopen(tl0, 0)) >= 0)
				{
					pipe = -1;
				} else
				{
					wrtln(tl0);
					wrt(" not found.");
				}
				break;

			case '<':
				for (tl0 = tl; (ch = *tl) /* != 0 */; tl++)
				{
					if (ch == ' ')
						break;
				}
				*tl++ = 0;

				if ((newsi = xopen(tl0, 0)) >= 0)
				{
					xforce(0, (int) newsi);
					xclose((int) newsi);
					nsi = -1;
				} else
				{
					wrtln(tl0);
					wrt(" not found.");
				}
				break;

			case '>':
				for (tl1 = tl; (ch = *tl) /* != 0 */; tl++)
				{
					if (ch == '>')
					{
						concat = -1;
						tl1++;
					}
					if (ch == ' ')
						break;
				}
				*tl++ = 0;
				if (concat)
				{
					if ((newso = xopen(tl1, 1)) < 0)
					{
						newso = xcreat(tl1, 0);
					}
				} else
				{
					xunlink(tl1);
					newso = xcreat(tl1, 0);
					nso = -1;
				}
				xforce(1, (int) newso);
				xclose((int) newso);
				if (concat)
					xf_seek(0L, 1, 2);
				nso = -1;
				break;
			default:
				*d++ = ch;
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
		argv[argc + 1] = d;

		s = argv[0];
		p = argv[1];
		ucase(s);

		/*Build command tail. */
		cmdtl = lin;
		j = 1;
		i = 1;
		while (*argv[i])
		{
			k = 0;
			while ((cmdtl[++j] = *(argv[i] + k++)) /* != 0 */)
				;
			cmdtl[j] = ' ';
			i++;
		}
		cmdtl[j] = 0xd;
		cmdtl[j + 1] = 0;
		cmdtl[0] = --j;
		cmdtl[1] = ' ';

		if (pipe)
		{
			xCmdLn(argv, &pipe, &dskFlHnd);
			xclose((int) dskFlHnd);
		} else
		{
			if ((strlen(s) == 2) && (s[1] == ':'))
			{
				xsetdrv(zdrv = (*s - 'A'));
			} else if (xncmps(3, s, "LS") || xncmps(4, s, "DIR"))
			{
				filOnly = dirOnly = terse = 0;
				i = 1;
				while (*argv[i])
				{
					ucase(argv[i]);
					if (*argv[i] == '/')
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
						case 'T':
							terse = -1;
							break;
						default:
							break;
						}
						j = i;
						while (*(argv[j] = argv[j + 1]))
							j++;
					} else
						i++;
				}
			
				if (*nonStdIn)
					dspCL(&argv[0]);
				chkDir(argv[1], srcDir, srcNmPat);
			
				if (!terse)
				{
					wrt("Directory of ");
					dspDir(argv[1], srcDir);
					dspMsg(12);
				}
			
				WSrcReq = argv[1];
				WAttCode = -1;
				if (!gtFlNm())
				{
					compl_code = 0xFFFFFFFF;
					if (!terse)
						dspMsg(1);
				} else
				{
					compl_code = 0;
					do
					{
						n = strlen(&srchb[30]);
						if (dirOnly && (srchb[21] != 0x10))
							goto skip;
						if ((filOnly) && (srchb[21] == 0x10))
							goto skip;
						if ((terse) && ((xncmps(2, &srchb[30], ".")) || (xncmps(3, &srchb[30], ".."))))
							goto skip;
			
						wrtln("");
						xwrite(1, (long)n, &srchb[30]);
			
						if (!terse)
						{
							for (i = n; i < 15; i++)
								xwrite(1, 1L, " ");
			
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
							xwrite(1, 2L, "  ");
			
							pl = (long *) &srchb[26];
							prtDclFmt((long) *pl, 6, " ");
						}
					  skip:
					  	;
					} while (gtFlNm());
					wrtln("");
					if (!terse)
						dspMsg(5);
				}
			} else if (xncmps(6, s, "CHMOD"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				chkDir(argv[1], srcDir, srcNmPat);
				WSrcReq = argv[1];
				WAttCode = -1;
				if (!gtFlNm())
				{
					compl_code = 0xFFFFFFFF;
					dspMsg(1);
				} else
				{
					mkSrc();
					att = srchb[21];
					if (att & 0x18)
					{
						wrt("Unable to change mode on subdirectorys or volumes.");
						compl_code = 0xFFFFFFFF;
					} else
					{
						compl_code = 0;
						do
						{
							if (!*argv[2])
							{
								wrt("Invalid mode specification.");
								compl_code = 0xFFFFFFFF;
							} else
							{
								i = mknum(argv[2]);
								if (i & ~0x7)
								{
									wrt("Invalid mode specification.");
									compl_code = 0xFFFFFFFF;
								} else
									compl_code = xattrib(srcFlNm, 1, i);
							}
						} while (gtFlNm());
					}
					if (!compl_code)
					{
						dspMsg(5);
					}
				}
			} else if (xncmps(4, s, "ERR"))
			{
				wrt("Competion code for previous command = "); /* typo */
				prthex((int) compl_code);
			} else if (xncmps(5, s, "PATH"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				ucase(p);
				setPath(p);
			} else if (xncmps(4, s, "ENV"))
			{
				i = 0;
				while ((prntEnvPtr[i] + prntEnvPtr[i + 1]) != 0)
				{
					/* if a path has been defined, don't count it. */
					if (!(xncmps(5, &prntEnvPtr[i], "PATH=")))
					{
						wrtln(&prntEnvPtr[i]);
					}
					i += strlen(&prntEnvPtr[i]);
					if (prntEnvPtr[i] + prntEnvPtr[i + 1] == 0)
						break;
					i += 1;
				}
				if (path[0])
				{
					wrtln("PATH=");
					wrt(path);
				}
			} else if (xncmps(4, s, "CAT") || xncmps(4, s, "TYPE"))
			{
				if (!*p)
					dspMsg(6);
				else
				{
					if (*nonStdIn)
						dspCL(&argv[0]);
					chkDir(p, srcDir, srcNmPat);
					WSrcReq = argv[1];
					WAttCode = -1;
					if (!gtFlNm())
					{
						dspMsg(1);
						compl_code = 0xFFFFFFFF;
					} else
					{
						compl_code = 0;
						do
						{
							mkSrc();
							fd = xopen(srcFlNm, 0);
							do
							{
								n = xread(fd, 1000L, buf);
								if (n > 0)
									xwrite(1, (long) n, buf);
							} while (n > 0);
						} while (gtFlNm());
						xclose(fd);
					}
				}
			} else if ((xncmps(4, s, "REM")) || (xncmps(4, s, "ECHO")))
			{
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
				if (*nonStdIn)
					dspCL(&argv[0]);
				if (argc == 0)
				{
					xgetdir(buf, zdrv + 1);
					if (!buf[0])
					{
						buf[0] = '\\';
						buf[1] = 0;
					}
					xwrite(1, (long)(int)strlen(buf), buf);
				} else
				{
					if ((compl_code = xchdir(p)) != 0)
						wrt("Directory not found.");
				}
			} else if (xncmps(3, s, "MD"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				compl_code = xmkdir(p);
			} else if (xncmps(3, s, "RD"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				compl_code = xrmdir(p);
			} else if (xncmps(3, s, "RM") || xncmps(4, s, "DEL") || xncmps(4, s, "ERA"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				if (*p)
				{
					chkDir(p, srcDir, srcNmPat);
					WSrcReq = argv[1];
					WAttCode = -1;
					if (!gtFlNm())
					{
						compl_code = 0xFFFFFFFF;
						dspMsg(1);
					} else
					{
						do
						{
							if (!(srchb[21] & 0x18))
							{
								mkSrc();
								dspMsg(7);
								dspMsg(12);
								compl_code = xunlink(srcFlNm);
							}
						} while (gtFlNm());
						dspMsg(5);
					}
				} else
					dspMsg(6);
			} else if (xncmps(4, s, "REN"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				compl_code = renmCmd(argv[1], argv[2]); /* BUG: renmCmd does not return anything */
			} else if (xncmps(5, s, "SHOW"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				ucase(p);
				xgetfree(sbuf, (*p ? *p - 64 : 0));
				wrt("Allocation Information: Drive ");
				if (!*p)
					dspMsg(16);
				else
					wrt(p);
				dspMsg(12);
				dspMsg(12);
				wrt("Total Units on Drive   ");
				prtDclFmt((long) sbuf[1], 6, " ");
				dspMsg(12);
				wrt("Free Units on Drive    ");
				prtDclFmt((long) sbuf[0], 6, " ");
				dspMsg(12);
				wrt("Sectors per Unit       ");
				prtDclFmt((long) sbuf[3], 6, " ");
				dspMsg(12);
				wrt("Bytes per Sector       ");
				prtDclFmt((long) sbuf[2], 6, " ");
			} else if (xncmps(5, s, "INIT"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				for (i = 0; i < BUFSIZ; i++)
					buf[i] = 0;
				/*
				 * A confirmation would be a good idea...
				 */
				ucase(p);
				zdrv = *p - 'A';
				buf[0] = 0xf7;
				buf[1] = 0xff;
				buf[2] = 0xff;
				/* why execute this in supervisor mode??? */
				super();
				b = (BPB *)getbpb(zdrv);
				if (b->b_flags & 1)
					buf[3] = 0xFF;
				f1 = b->fatrec - b->fsiz;
				f2 = b->fatrec;
				fs = b->fsiz;
				rwabs(1, buf, fs, f1, zdrv);
				rwabs(1, buf, fs, f2, zdrv);
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
					rwabs(1, buf, 1, rec, zdrv);
				user();
				dspMsg(5);
			} else if (xncmps(8, s, "PUTBOOT"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				ucase(p);
				zdrv = *p - 'A';
				fd = xopen(argv[2], 0);
				xread(fd, 540L, buf);
				xclose(fd);
				super();
				rwabs(1, &buf[28], 1, 0, zdrv);
				user();
				dspMsg(5);
			} else if ((xncmps(5, s, "COPY")) || (xncmps(5, s, "MOVE")))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				if (argc >= 1)
				{
					if (copyCmd(p, argv[2], xncmps(5, s, "MOVE") ? 1 : 0) == -1)
					{
						wrtln("Disk full - copy failed.");
						compl_code = 0xFFFFFFFF;
					}
				} else
					dspMsg(6);
			} else if (xncmps(4, s, "GET"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				ucase(p);
				cpmopen(p);
				fd = xcreat(argv[2], 0x20);

				do
				{
					n = cpmread(buf);
					if (!n)
						xwrite(fd, 128L, buf);
				} while (!n);

				xclose(fd);
				dspMsg(5);
			} else if (xncmps(6, s, "PAUSE"))
			{
				wrt("CR to continue...");
				lin[0] = 126;
				oscall(10, &lin[0]);
			} else if (xncmps(4, s, "PUT"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				cpmcreate(argv[2]);
				fd = xopen(p, 0);

				do
				{
					n = xread(fd, 128L, buf);
					if (n > 0)
						compl_code = cpmwrite(buf);
				} while (n > 0);

				cpmclose();
				dspMsg(5);
			} else if (xncmps(5, s, "HELP"))
			{
				dspMsg(17);
			} else if (xncmps(6, s, "BREAK"))
			{
				xbrkpt();
			} else if (xncmps(5, s, "EXIT"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				xclose(oldsi);
				xclose(oldso);
				devector();				/* remove vectors */
				xterm(0);
			} else if (xncmps(8, s, "VERSION"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				i = oscall(0x30);
				prtdecl((long) (i & 0xFF));
				xwrite(1, 1L, ".");
				prtdecl((long) ((i >> 8) & 0xFF));
			} else if (xncmps(5, s, "WRAP"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				xwrite(1, 2L, "\033v");
				dspMsg(5);
			} else if (xncmps(7, s, "NOWRAP"))
			{
				if (*nonStdIn)
					dspCL(&argv[0]);
				xwrite(1, 2L, "\033w");
				dspMsg(5);
			} else if (xncmps(4, s, "CLS"))
			{
				xwrite(1, 2L, "\033E");
			} else
			{
				if (*nonStdIn)
					dspCL(argv);
				if (!(execBat(s, argv)))
				{
					if ((compl_code = execPrgm(s, cmdtl)) == -32)
						xlongjmp(&jb, -1);
					else if (compl_code < 0)
					{
						wrt("Command not found.");
						dspMsg(12);
					}
				}
			}
		}
		if (nsi == -1)
		{
			xclose(0);
			xforce(0, oldsi);
		}
		if (nso == -1)
		{
			xclose(1);
			xforce(1, oldso);
		}
	  again:
	  	;
		/* if command coming from outside the command int exit */
	}
}

/***************************************************************************/
/***************************************************************************/

VOID cmain(P(char *) bp)
PP(char *bp;)								/* Base page address */
{
	char *parm[8];
	int i, k;
	long j;

	basePage = bp;
	prntEnvPtr = *((char **) (basePage + 0x2C));

	xsetdta(srchb);
	path[0] = 0;
	compl_code = 0;
	dspMsg(4);

	i = 0;
	while ((prntEnvPtr[i] + prntEnvPtr[i + 1]) != 0)
	{
		/* if a path has been defined, don't count it. */
		if (xncmps(5, &prntEnvPtr[i], "PATH="))
		{
			setPath(&prntEnvPtr[i + 5]);
			break;
		}
		i++;
	}

	execBat(autoBat, &parm[0]);

	if (xsetjmp(&jb))
	{
		for (i = 6; i <= 20; i++)
			xclose(i);
	}

	while (1)
	{
		k = 0;
		j = 0;
		xCmdLn(&parm[0], &k, &j);
	}
}
