/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
 */

char *SCCSID = "@(#) ar68 - July 1, 1983";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <ar68.h>
#include "util.h"

#define FALSE 0
#define TRUE  1

#define _(x) x

#ifndef NO_CONST
#  ifdef __GNUC__
#	 define NO_CONST(p) __extension__({ union { const void *cs; void *s; } x; x.cs = p; x.s; })
#  else
#    ifdef __ALCYON__ /* Alcyon parses the cast as function call??? */
#      define NO_CONST(p) p
#    else
#      define NO_CONST(p) ((void *)(p))
#    endif
#  endif
#endif

#ifdef __ALCYON__
#define fopenbr(f) fopenb(f, "r")
#define fopenbw(f) fopenb(f, "w")
#else
#define fopenbr(f) fopen(f, "rb")
#define fopenbw(f) fopen(f, "wb")
#define efseek fseek
#define creatb creat
#define openb open
#endif

#define USAGE		"usage: %s [rdxtpvabi] [pos] archive file [file ...]\n"
#define DEFMODE  	0666

#define SYMDEF "._SYMDEF"

char buff[BUFSIZ];

#if 0
struct stat astat, tstat;
#endif

/* flags for cp1file */

#define IEVEN	1
#define OEVEN	2
#define WHDR	4
#define WCOUTHD 8

struct libhdr libhd;
struct libhdr *lp = &libhd;

unsigned short libmagic = LIBMAGIC;

int rflg, dflg, xflg, tflg,vflg;
int uflg;
int pflg;
int areof;
FILE *arfp;
FILE *tempfd;

int aflg, bflg;
int psflg;
int matchflg;
char *psname;
char *arname, *tempname;

char *myname;
struct hdr2 couthd;
int exitstat;

FILE *openar PROTO((const char *arp, int crfl));
VOID xtell PROTO((const char * ap));
VOID pfname PROTO((NOTHING));
VOID replace PROTO((const char *name));
VOID delete PROTO((const char *name));
VOID extract PROTO((const char *name));
VOID print PROTO((const char *name));
VOID endit PROTO((NOTHING));
VOID cleanup PROTO((NOTHING));
VOID listall PROTO((NOTHING));
int nextar PROTO((NOTHING));
VOID skcopy PROTO((int cpflag));
char *mktemp PROTO((char *ap));
int streq PROTO((const char *s1, const char *s2));
VOID pmode PROTO((int aflg1));
VOID select PROTO((int *pairp, int flg));
VOID inform PROTO((char cc));
VOID copystr PROTO((const char *ap1, char *ap2, int alen));
VOID cprest PROTO((NOTHING));
VOID cp1file PROTO((FILE *ifd, FILE *ofd, int aflags, const char *iname, const char *oname));
int ckafile PROTO((const char *ap));
VOID exall PROTO((NOTHING));
VOID tmp2ar PROTO((NOTHING));
const char *fnonly PROTO((const char *s));
int copy PROTO((const char *from, const char *to));


VOID (*docom) PROTO((const char *name));

char fname[15];

int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register char *p1, *p2;
	register char **ap;
	register int i;
	int j, docopy;

	UNUSED(p2);
	printf(_("AR68 Archiver (c) Alcyon Corporation\n"));
	close(2); /* WTF? */
	close(0); /* WTF? */
	myname = *argv;
	exitstat = 0;
	if (argc < 3)
	{
	  usage:
		printf(USAGE, myname);
		endit();
	}
	signal(SIGHUP, (sighandler_t)endit);
	signal(SIGINT, (sighandler_t)endit);
	ap = &argv[1];
	p1 = *ap++;
	i = argc - 3;
	while (*p1)
	{
		switch (*p1++)
		{
		case 'r':
			rflg++;
			docom = replace;
			break;
		case 'a':
			aflg++;
			psname = *ap++;
			i--;
			break;
		case 'b':
		case 'i':
			bflg++;
			i--;
			psname = *ap++;
			break;
		case 'd':
			dflg++;
			docom = delete;
			break;
		case 'x':
			xflg++;
			docom = extract;
			break;
		case 't':
			tflg++;
			docom = xtell;
			break;
		case 'p':
			pflg++;
			docom = print;
			break;
		case 'v':
			vflg++;
			break;
		case '-':
			break;
		default:
			printf(_("%s: invalid option flag: %c\n"), myname, *--p1);
			endit();
		}
	}
	uflg = rflg + dflg;
	if ((uflg + xflg + tflg + pflg) != 1)
	{
		printf(_("%s: one and only one of 'rdxt' flags required\n"), myname);
		endit();
	}
	psflg = aflg + bflg;
	if (psflg > 1)
	{
		printf(_("%s: only one of 'abi' flags allowed\n"), myname);
		endit();
	}
	if (psflg && (rflg != 1))
	{
		printf(_("%s: 'abi' flags can only be used with 'r' flag\n"), myname);
		endit();
	}
	arname = *ap++;
	if ((arfp = openar(arname, rflg)) == NULL)
	{
		if ((tflg + xflg + bflg + aflg + dflg) != 0)
		{
			printf("%s: no such file '%s'\n", myname, arname);
			endit();
		}
	}
	if (i == 0 && tflg)
	{
		listall();
		cleanup();
	}
	if (i == 0 && xflg)
	{
		exall();
		cleanup();
	}
	if (i <= 0)
		goto usage;
	if (libmagic == (uflg == 0 ? 0 : 1))
		printf(_("%s: warning, changing a random access library\n"), myname);
	if (!pflg && !tflg)
	{
		tempname = mktemp("art?????");
		if ((tempfd = fopenbw(tempname)) == NULL)
		{
			printf(_("%s: can't create %s\n"), myname, tempname);
			endit();
		}

		if (lputw(&libmagic, tempfd) != 0)
			printf(_("%s: write error on magic number"), myname); /* BUG: no newline; should use perror */
	}
	
	/* read archive, executing appropriate commands */
	while (matchflg == 0 && nextar())
	{
		docopy = 1;
		if (!psflg)
		{
			for (j = 0; j < i; j++)
			{
				if (streq(fnonly(ap[j]), &lp->lfname[0]))
				{
					docopy = 0;
					(*docom) (ap[j]);
					i--;
					while (j < i)
					{
						ap[j] = ap[j + 1];
						j++;
					}
				}
			}
		} else if (streq(psname, &lp->lfname[0]))
		{
			docopy = 0;
			for (j = 0; j < i; j++)
				(*docom) (ap[j]);
			i = 0;
			psflg = 0;
		}
		if (docopy)
			skcopy(uflg);
	}

	/* deal with the leftovers */
	if (i > 0)
	{
		for (j = 0; j < i; j++)
		{
			if (rflg)
			{
				(*docom) (ap[j]);
			} else
			{
				printf(_("%s not found\n"), ap[j]);
				exitstat = EXIT_FAILURE;
			}
		}
	}
	(*docom) (NULL);

#if 0
	/* make temp file the archive file */
	if (stat(tempname, &tstat) < 0)
	{
		printf(_("can't find %s\n"), tempname);
		endit();
	}
	if (arfp != NULL)
	{
		if (stat(arname, &astat) < 0)
		{
			printf(_("can't find %s\n"), arname);
			endit();
		}
		if ((astat.st_nlink != 1) || (astat.st_dev != tstat.st_dev))
		{
			fclose(arfp);
			tmp2ar();
		} else if ((unlink(arname) == -1))
		{
			printf(_("can't unlink old archive\nnew archive left in %s\n"), tempname);
			tempname = 0;				/* keeps endit from removing the archive */
		} else if (link(tempname, arname) < 0)
		{
			if (copy(tempname, arname) < 0)
			{
				printf(_("can't link to %s\nnew archive left in %s\n"), arname, tempname);
				tempname = 0;
			}
		}
	} else
	{
		if ((arfp = fopenbw(arname)) == NULL)
		{
			printf(_("can't create %s\narchive left in %s\n"), arname, tempname);
			tempname = 0;				/* keeps endit from removing the archive */
			endit();
		}
		if (stat(arname, &astat) < 0)
		{
			printf(_("can't find %s\n"), arname);
			endit();
		}
		fclose(arfp);
		if (astat.st_dev != tstat.st_dev)
		{
			tmp2ar();
			exitstat = EXIT_SUCCESS;
		} else if ((unlink(arname) < 0) || (link(tempname, arname) < 0))
		{
			printf(_("can't link to %s\n"), arname);
			printf(_("new archive left in %s\n"), tempname);
			tempname = 0;				/* keeps endit from removing the archive */
		}
	}
	endit();
#else
	if (tflg || pflg)
		cleanup();
	tmp2ar();
	exitstat = EXIT_SUCCESS;
	cleanup();
#endif

#ifndef __ALYCYON__
	return EXIT_SUCCESS;
#endif
}


FILE *openar(P(const char *) arp, P(int) crfl)
PP(const char *arp;)
PP(int crfl;)
{
	register FILE *i;
	unsigned short ib;

	if ((i = fopenbr(arp)) == NULL)
	{									/* does not exist */
		areof = 1;
		return NULL;
	}
	if (lgetw(&ib, i) != 0 || ib != LIBMAGIC)
	{
	/*  notar: */
		printf(_("%s: not archive format: %s %x\n"), myname, arp, ib);
		endit();
	}
	return i;
}


/* execute one command -- call with filename or 0 */

VOID xtell(P(const char *) ap)
PP(const char *ap;)
{
	register char *p;
	register int i;
	register char *p1;

	UNUSED(i);
	UNUSED(p);
	if (ap != (char *)-1 && ckafile(ap))
		return;
	if (vflg)
	{									/* long list */
		pmode(lp->lfimode);
		printf(" %d/%d ", lp->luserid, lp->lgid);
		printf("%6ld", lp->lfsize);
		p1 = ctime(&lp->lmodti);
		p1[24] = '\0';
		p1 += 4;
		printf(" %s  ", p1);
	}
	pfname();
	skcopy(0);
}


VOID pfname(NOTHING)
{
	register char *p;
	register int i;

	p = &lp->lfname[0];
	i = LIBNSIZE;
	while (*p && i)
	{
		putchar(*p++);
		i--;
	}
	putchar('\n');
}


VOID replace(P(const char *) name)
PP(const char *name;)
{
	register int i;
	register FILE *ifd;
	char namebuf[32];
	
	UNUSED(i);
	if (name == NULL)
	{
		if (bflg && areof == 0)
		{
			if (efseek(arfp, -(off_t) LIBHDSIZE, SEEK_CUR) == -1)
			{
#ifdef __ALCYON__
				/* BUG: should print arname instead */
				printf(_("%s: fseek error\n"), myname);
#else
				fprintf(stderr, _("%s: %s: fseek error\n"), myname, arname);
#endif
			}
		}
		cprest();
		return;
	}
	copystr(name, namebuf, sizeof(namebuf));
	
	if ((ifd = fopenbr(namebuf)) == NULL)
	{
		/* BUG: should use namebuf */
		printf(_("%s: can't open %s\n"), myname, name);
		endit();
	}
	/* BUG: ar should not depend on members being object files */
	if (getchd(ifd, &couthd) < 0)
	{
		printf(_("%s: can't read %s\n"), myname, name);
		endit();
	}
	if (areof && psflg)
	{
		printf(_("%s: %s not in library\n"), myname, psname);
		endit();
	}
	if ((bflg | aflg) && matchflg == 0)
	{									/* copy archive before appending */
		if (aflg)
			skcopy(1);
		matchflg++;
	}
	copystr(name, &lp->lfname[0], LIBNSIZE);
	if (areof | aflg)
	{
		inform('a');
	} else if (bflg)
	{
		inform('i');
	} else
	{
		inform('r');
		skcopy(0);						/* skip old copy */
	}
#if 0
	lp->luserid = stp->st_uid;
	lp->lgid = stp->st_gid;
	lp->lfimode = stp->st_mode;
	lp->lmodti = stp->st_mtime;
	lp->lfsize = stp->st_size;
#else
	lp->lfsize = couthd.ch_tsize + couthd.ch_dsize + couthd.ch_ssize + HDSIZE; /* BUG: should check magic first for headersize */
	if (couthd.ch_rlbflg == 0)
		lp->lfsize += couthd.ch_tsize + couthd.ch_dsize;
#endif
	cp1file(ifd, tempfd, WHDR + OEVEN, name, tempname);
	fclose(ifd);
}


VOID delete(P(const char *) ap)
PP(const char *ap;)
{
	if (ap == NULL)
	{
		cprest();
		efseek(tempfd, 0L, SEEK_SET);
		lputw(&libmagic, tempfd);
		return;
	}
	if (strcmp(ap, SYMDEF) == 0)
		libmagic = LIBMAGIC;
	inform('d');
	skcopy(0);
}


VOID extract(P(const char *) ap)
PP(const char *ap;)
{
	register FILE *ofd;
	register int i;

	if (ckafile(ap))
		return;
	if ((i = creatb(ap, lp->lfimode)) < 0)
	{
		printf(_("%s: can't create %s\n"), myname, ap);
		endit();
	}
	ofd = fdopen(i, "w");
	inform('x');
	cp1file(arfp, ofd, IEVEN, arname, ap);
	fclose(ofd);
}


VOID print(P(const char *) ap)
PP(const char *ap;)
{
	if (ckafile(ap))
		return;
	/* WTF; 2nd argument to cp1file is a FILE *, not '1' */
#ifdef __ALCYON___
	cp1file(arfp, 1, IEVEN, arname, "std output");
#else
	cp1file(arfp, (FILE *)1, IEVEN, arname, "std output");
#endif
}


VOID endit(NOTHING)
{
	exitstat = EXIT_FAILURE;
	cleanup();
}


VOID cleanup(NOTHING)
{
	if (tempname)
		unlink(tempname);
	exit(exitstat);
}


/* list all file in the library */

VOID listall(NOTHING)
{
	while (nextar())
		xtell((char *) -1);
}

/* read next ar file header into libhd */

int nextar(NOTHING)
{
	if (areof || getarhd(arfp, &libhd) == EOF || libhd.lfname[0] == 0)
	{
		areof++;
		return FALSE;
	}
	return TRUE;
}

/* call with cpflag = 0 for skip, cpflag = 1 for copy */
VOID skcopy(P(int) cpflag)
PP(int cpflag;)
{
	register off_t l;
	register int i;

	UNUSED(i);
	if (areof)
		return;
	l = lp->lfsize;
	if (l & 1)
		l++;
	if (cpflag)
	{
		inform('c');
		cp1file(arfp, tempfd, WHDR + OEVEN + IEVEN, arname, tempname);
	} else
	{
		if (efseek(arfp, l, SEEK_CUR) == -1)
		{
			printf(_("%s; seek error on library\n"), myname);
			endit();
		}
	}
}


char *mktemp(P(char *) ap)
PP(char *ap;)
{
	register char *p;
	register int i, j;

	i = getpid();						/* process id */
	p = ap;
	while (*p)
		p++;
	for (j = 0; j < 5; j++)
	{
		*--p = ((i & 7) + '0');
		i >>= 3;
	}
	return ap;
}


int streq(P(const char *) s1, P(const char *) s2)
PP(const char *s1;)
PP(const char *s2;)
{
	register const char *p1;
	register const char *p2;

	p1 = s1;
	p2 = s2;
	while (*p1++ == *p2)
		if (*p2++ == 0)
			return TRUE;
	return FALSE;
}


int m1[] = { 1, S_IRUSR, 'r', '-' };
int m2[] = { 1, S_IWUSR, 'w', '-' };
int m3[] = { 1, S_IXUSR, 'x', '-' };
int m4[] = { 1, S_IRGRP, 'r', '-' };
int m5[] = { 1, S_IWGRP, 'w', '-' };
int m6[] = { 1, S_IXGRP, 'x', '-' };
int m7[] = { 1, S_IROTH, 'r', '-' };
int m8[] = { 1, S_IWOTH, 'w', '-' };
int m9[] = { 1, S_IXOTH, 'x', '-' };

int *m[] = { m1, m2, m3, m4, m5, m6, m7, m8, m9 };

VOID pmode(P(int) aflg1)
PP(int aflg1;)
{
	register int **mp;

	for (mp = &m[0]; mp < &m[9];)
		select(*mp++, aflg1);
}


VOID select(P(int *) pairp, P(int) flg)
PP(int *pairp;)
PP(int flg;)
{
	register int n;
	register int *ap;
	register int f;

	ap = pairp;
	f = flg;
	n = *ap++;
	while (--n >= 0 && (f & *ap++) == 0)
		ap++;
	putchar(*ap);
}


VOID inform(P(char) cc)
PP(char cc;)
{
	if (vflg)
	{
		putchar(cc);
		putchar(' ');
		pfname();
	}
}


VOID copystr(P(const char *) ap1, P(char *) ap2, P(int) alen)
PP(const char *ap1;)
PP(char *ap2;)
PP(int alen;)
{
	register const char *p1;
	register char *p2;
	register int len;

	p2 = 0;
	p1 = ap1;
	while (*p1)
		if (*p1++ == '/')
			p2 = NO_CONST(p1);					/* point to char after last '/' in name */
	if (p2)
		p1 = p2;
	else
		p1 = ap1;
	p2 = ap2;
	len = alen;
	while (len)
	{
		if (!(*p2++ = *p1++))
			break;
		len--;
	}
	while (--len > 0)
		*p2++ = '\0';
}


VOID cprest(NOTHING)
{
	while (nextar())
		skcopy(1);						/* copy rest of library */
}


VOID cp1file(P(FILE *) ifd, P(FILE *) ofd, P(int) aflags, P(const char *) iname, P(const char *) oname)
PP(FILE *ifd;)
PP(FILE *ofd;)
PP(int aflags;)
PP(const char *iname;)
PP(const char *oname;)
{
	register int i;
	register long l;
	register int flags, sz;
	char str[50];

	flags = aflags;
	UNUSED(str);
	if (flags & WHDR)
	{
		if (putarhd(ofd, &libhd) != 0)
		{
		  iwrerr:
#ifdef __ALCYON__
			sprintf(str, _("%s: write error on %s"), myname, oname);
			printf("%s\n", str);
#else
			fprintf(stderr, "%s: ", myname);
			perror(oname);
#endif
			endit();
		}
	}
	if (flags & WCOUTHD)
	{
		if (putchd(ofd, &couthd) != 0)
		{
#ifdef __ALCYON__
			sprintf(str, _("%s: write error on CHDR %s"), myname, oname);
			printf("%s\n", str);
#else
			goto iwrerr;
#endif
		}
		l = lp->lfsize - HDSIZE;
	} else
	{
		l = lp->lfsize;
	}
	while (l)
	{
		if (l < BUFSIZ)
			sz = l;
		else
			sz = BUFSIZ;
		if ((i = fread(buff, sizeof(char), sz, ifd)) == 0)
		{
#ifdef __ALCYON__
			printf(_("%s: read error\n"), myname);
#else
			fprintf(stderr, "%s: ", myname);
			perror(iname);
#endif
			endit();
		}
		if (fwrite(buff, sizeof(char), i, ofd) == 0)
			goto iwrerr;
		l -= i;
	}
	if (flags & OEVEN)
	{
		if (lp->lfsize & 1)
			fwrite("", sizeof(char), 1, ofd);
	}
	if (flags & IEVEN)
	{
		if (lp->lfsize & 1)
			fread(buff, sizeof(char), 1, ifd);
	}
}


int ckafile(P(const char *) ap)
PP(const char *ap;)
{
	if (ap == NULL || *ap == '\0')
		cleanup();
	if (areof)
	{
		printf(_("%s: %s not in archive file\n"), myname, ap);
		return TRUE;
	}
	return FALSE;
}


VOID exall(NOTHING)
{
	if (nextar())
	{
		if (strcmp(libhd.lfname, SYMDEF) == 0)
		{
			efseek(arfp, lp->lfsize, SEEK_CUR);
			if (nextar() == FALSE)
				return;
		}
	}
	do {
		extract(lp->lfname);
	} while (nextar());
}


VOID tmp2ar(NOTHING)
{
	register int n, ifd, ofd;

	if ((ofd = creatb(arname, DEFMODE)) < 0)
	{
		printf(_("%s: can't create %s\narchive left in %s\n"), myname, arname, tempname);
		tempname = NULL;					/* keeps endit from removing the archive */
		return;
	}
	fflush(tempfd); /* BUG: superfluous */
	fclose(tempfd);
	if ((ifd = openb(tempname, O_RDONLY)) < 0)
	{
		printf(_("%s: failed to open %s\n"), myname, tempname);
		return;
	}
	while ((n = read(ifd, buff, BUFSIZ)) > 0)
		write(ofd, buff, n);
	buff[0] = buff[1] = buff[2] = buff[3] = 0;
	write(ofd, buff, 4);
	/* BUG: ofd never closed */
	tempfd = fdopen(ifd, "r");
}


const char *fnonly(P(const char *) s)
PP(const char *s;)
{
	register const char *p;
	register const char *r;

	r = s;
	strcpy(fname, r);
	p = 0;
	while (*r)
	{
		if (*r == '/')
			p = r;
		r++;
	}
	if (p)
	{
		++p;
		if (*p == 0)
			p = fname;
	} else
		p = fname;

	return p;
}


/* this is a copy routine for the cross device archive creation */
int copy(P(const char *) from, P(const char *) to)
PP(const char *from;)
PP(const char *to;)
{
	register int ifd, ofd, len;

	if ((ofd = open(to, O_WRONLY)) == -1) /* BUG: open() opens in ASCII mode by default */
	{
		if ((ofd = creat(to, DEFMODE)) == -1) /* BUG: creat() opens in ASCII mode by default */
			return -1;
	}
	if ((ifd = open(from, O_RDONLY)) == -1) /* BUG: open() opens in ASCII mode by default */
	{
		close(ofd);
		unlink(to);
		return -1;
	}

	while ((len = read(ifd, buff, sizeof buff)) > 0)
		write(ofd, buff, len);

	close(ifd);
	close(ofd);
	/* BUG: no return value */
#ifndef __ALCYON__
	return 0;
#endif
}
