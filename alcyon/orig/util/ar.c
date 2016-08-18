/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
 */

char *SCCSID = "@(#) ar68 - July 1, 1983";

#include <stdio.h>

#ifndef PDP11
#	define SEEKTYPE long
#	include <sys/types.h>
#	include <stat.h>
#	ifdef VAX11
#		include <c68/ar68.h>
#	else
#		include <ar68.h>
#	endif
#else
#	include <stat11.h>
#	define SEEKTYPE int

long mklong();
#endif

#ifdef VAX11
#	define int short
#endif

#define USAGE		"usage: %s [rdxtpvabi] [pos] archive file [file ...]\n"
#define DEFMODE  	0666

char buff[BUFSIZ];

struct stat astat,
 tstat;									/* 11 apr 83, struct stat */

#define READ	0
#define WRITE	1

/* flags for cp1file */

#define IEVEN	1
#define OEVEN	2
#define WHDR	4

#define	ROWN	0400					/* file protection flags */
#define	WOWN	0200
#define	XOWN	0100
#define	RGRP	040
#define	WGRP	020
#define	XGRP	010
#define	ROTH	04
#define	WOTH	02
#define	XOTH	01

struct libhdr libhd;
struct libhdr *lp = { &libhd };

int libmagic = LIBMAGIC;

int rflg,
 dflg,
 xflg,
 tflg,
 vflg;

int uflg;

int pflg;

int areof;

FILE *arfp;

FILE *tempfd;

int aflg,
 bflg;

int psflg;

int matchflg;

char *psname;

char *arname,
*tempname;

FILE *openar();

char *mktemp();

char *ctime();

char *fnonly();

int (*docom) ();

int endit();

int replace();

int delete();

int extract();

int tell();

int print();

#ifdef VAX11
struct
{
	short loword;
	short hiword;
};
#else
struct
{
	int hiword;
	int loword;
};
#endif

char fname[15];

main(argc, argv)
char **argv;
{
	register char *p1,
	*p2;

	register char **ap;

	register int i;

	int j,
	 docopy;

	char *myname;

	myname = *argv;
	if (argc < 3)
	{
	  usage:
		printf(USAGE, myname);
		endit();
	}
	signal(1, endit);
	signal(2, endit);
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
			docom = tell;
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
			printf("invalid option flag: %c\n", *--p1);
			endit();
		}
	}
	uflg = rflg + dflg;
	if ((uflg + xflg + tflg + pflg) != 1)
	{
		printf("At least one and only one of 'rdxt' flags required\n");
		endit();
	}
	psflg = aflg + bflg;
	if (psflg > 1)
	{
		printf("only one of 'abi' flags allowed\n");
		endit();
	}
	if (psflg && (rflg != 1))
	{
		printf("'abi' flags can only be used with 'r' flag\n");
		endit();
	}
	arname = *ap++;
	arfp = openar(arname, rflg);
	if (i == 0 && tflg)
	{
		listall();
		endit();
	}
	if (i == 0 && xflg)
	{
		exall();
		endit();
	}
	if (i <= 0)
		goto usage;
	tempname = mktemp("/tmp/_~ar?????");
	if ((tempfd = fopen(tempname, "w")) == NULL)
	{
		printf("can't create %s\n", tempname);
		endit();
	}

	if (lputw(&libmagic, tempfd) != 0)
		perror("ar: write error on magic number");

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
				(*docom) (ap[j]);
			else
				printf("%s not found\n", ap[j]);
		}
	}
	(*docom) (0L);

/* make temp file the archive file */
	if (stat(tempname, &tstat) < 0)
	{
		printf("can't find %s\n", tempname);
		endit();
	}
	if (arfp != NULL)
	{
		if (stat(arname, &astat) < 0)
		{
			printf("can't find %s\n", arname);
			endit();
		}
		if ((astat.st_nlink != 1) || (astat.st_dev != tstat.st_dev))
		{
			fclose(arfp);
			tmp2ar();
		} else if ((unlink(arname) == -1))
		{
			printf("can't unlink old archive\nnew archive left in %s\n", tempname);
			tempname = 0;				/* keeps endit from removing the archive */
		} else if (link(tempname, arname) < 0)
		{
			if (copy(tempname, arname) < 0)
			{
				printf("can't link to %s\nnew archive left in %s\n", arname, tempname);
				tempname = 0;
			}
		}
	} else
	{
		if ((arfp = fopen(arname, "w")) == NULL)
		{
			printf("can't create %s\narchive left in %s\n", arname, tempname);
			tempname = 0;				/* keeps endit from removing the archive */
			endit();
		}
		if (stat(arname, &astat) < 0)
		{
			printf("can't find %s\n", arname);
			endit();
		}
		fclose(arfp);
		if (astat.st_dev != tstat.st_dev)
			tmp2ar();
		else if ((unlink(arname) < 0) || (link(tempname, arname) < 0))
		{
			printf("can't link to %s\n", arname);
			printf("new archive left in %s\n", tempname);
			tempname = 0;				/* keeps endit from removing the archive */
		}
	}
	endit();
}

FILE *openar(arp, crfl)
char *arp;
{
	register FILE *i;

	unsigned int ib;

	if ((i = fopen(arp, "r")) == NULL)
	{									/* does not exist */
		areof = 1;
		return (NULL);
	}
	if (lgetw(&ib, i) != 0 || ib != LIBMAGIC)
	{
	  notar:
		printf("not archive format: %s %o\n", arp, ib);
		endit();
	}
	return (i);
}

/* execute one command -- call with filename or 0 */

int tell(ap)
char *ap;
{
	register char *p;

	register i;

	register char *p1;

	if (ckafile(ap))
		return;
	if (vflg)
	{									/* long list */
		pmode(lp->lfimode);
		printf(" %d/%d ", lp->luserid, lp->lgid);
#ifdef PDP11
		plong("%6ld", lp->lfsize);
#else
		printf("%6ld", lp->lfsize);
#endif
		p1 = ctime(&lp->lmodti);
		p1[24] = '\0';
		p1 += 4;
		printf(" %s  ", p1);
	}
	pfname();
	skcopy(0);
}

pfname()
{
	register char *p;

	register i;

	p = &lp->lfname[0];
	i = LIBNSIZE;
	while (*p && i)
	{
		putchar(*p++);
		i--;
	}
	putchar('\n');
}

int replace(name)
char *name;
{
	register int i;

	register FILE *ifd;

	register struct stat *stp;			/* 11 apr 83, struct stat */

	struct stat stbuf;

#ifdef PDP11
	long l;
#endif

	stp = &stbuf;
	if (name == 0)
	{
		if (bflg && areof == 0)
		{
			if (fseek(arfp, -(SEEKTYPE) LIBHDSIZE, 1) == -1)
				printf("fseek error\n");
		}
		cprest();
		return;
	}
	if (stat(name, stp) < 0)
	{
		printf("can't find %s\n", name);
		endit();
	}
	if ((ifd = fopen(name, "r")) == NULL)
	{
		printf("can't open %s\n", name);
		endit();
	}
	if (areof && psflg)
	{
		printf("%s not in library\n", psname);
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
		inform('a');
	else if (bflg)
		inform('i');
	else
	{
		inform('r');
		skcopy(0);						/* skip old copy */
	}
	lp->luserid = stp->st_uid;
	lp->lgid = stp->st_gid;
	lp->lfimode = stp->st_mode;
#ifdef PDP11
	l = mklong(stp->st_mtime[0], stp->st_mtime[1]);
	lp->lmodti = l;
	l = mklong(stp->st_size0, stp->st_size1);
	lp->lfsize = l;
#else
	lp->lmodti = stp->st_mtime;
	lp->lfsize = stp->st_size;
#endif
	cp1file(ifd, tempfd, WHDR + OEVEN, name, tempname);
	fclose(ifd);
}

int delete(ap)
char *ap;
{
	if (ap == 0)
	{
		cprest();
		return;
	}
	inform('d');
	skcopy(0);
}

int extract(ap)
char *ap;
{
	register FILE *ofd;

	register i;

	if (ckafile(ap))
		return;
	if ((i = creat(ap, lp->lfimode)) < 0)
	{
		printf("can't create %s\n", ap);
		endit();
	}
	ofd = fdopen(i, "w");
	inform('x');
	cp1file(arfp, ofd, IEVEN, arname, ap);
	fclose(ofd);
}

int print(ap)
char *ap;
{

	if (ckafile(ap))
		return;
	cp1file(arfp, 1, IEVEN, arname, "std output");
}

int endit()
{

	if (tempname)
		unlink(tempname);
	exit(0);
}

/* list all file in the library */

listall()
{

	while (nextar())
		tell((char *) -1);
}

/* read next ar file header into libhd */

nextar()
{

	if (areof || getarhd(arfp, &libhd) == EOF || libhd.lfname[0] == 0)
	{
		areof++;
		return (0);
	}
	return (1);
}

/* call with cpflag = 0 for skip, cpflag = 1 for copy */
skcopy(cpflag)
int cpflag;
{
	register SEEKTYPE l;

	register int i;

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
		if (fseek(arfp, l, 1) == -1)
		{
			printf("seek error on library\n");
			endit();
		}
	}
}

char *mktemp(ap)
char *ap;
{
	register char *p;

	register i,
	 j;

	i = getpid();						/* process id */
	p = ap;
	while (*p)
		p++;
	for (j = 0; j < 5; j++)
	{
		*--p = ((i & 7) + '0');
		i >>= 3;
	}
	return (ap);
}

streq(s1, s2)
char *s1,
*s2;
{
	register char *p1,
	*p2;

	p1 = s1;
	p2 = s2;
	while (*p1++ == *p2)
		if (*p2++ == 0)
			return (1);
	return (0);
}

int m1[] = { 1, ROWN, 'r', '-' };
int m2[] = { 1, WOWN, 'w', '-' };
int m3[] = { 1, XOWN, 'x', '-' };
int m4[] = { 1, RGRP, 'r', '-' };
int m5[] = { 1, WGRP, 'w', '-' };
int m6[] = { 1, XGRP, 'x', '-' };
int m7[] = { 1, ROTH, 'r', '-' };
int m8[] = { 1, WOTH, 'w', '-' };
int m9[] = { 1, XOTH, 'x', '-' };

int *m[] = { m1, m2, m3, m4, m5, m6, m7, m8, m9 };

pmode(aflg1)
{
	register int **mp;

	for (mp = &m[0]; mp < &m[9];)
		select(*mp++, aflg1);
}

select(pairp, flg)
int *pairp;

int flg;
{
	register int n,
	*ap,
	 f;

	ap = pairp;
	f = flg;
	n = *ap++;
	while (--n >= 0 && (f & *ap++) == 0)
		ap++;
	putchar(*ap);
}

inform(cc)
char cc;
{

	if (vflg)
	{
		putchar(cc);
		putchar(' ');
		pfname();
	}
}

copystr(ap1, ap2, alen)
char *ap1,
*ap2;
{
	register char *p1,
	*p2;

	register len;

	p2 = 0;
	p1 = ap1;
	while (*p1)
		if (*p1++ == '/')
			p2 = p1;					/* point to char after last '/' in name */
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

#ifdef PDP11
long mklong(ai1, ai2)
{
	long l;

	l.hiword = ai1;
	l.loword = ai2;
	return (l);
}

plong(num)
long num;
{
}
#endif

cprest()
{

	while (nextar())
		skcopy(1);						/* copy rest of library */
}

cp1file(ifd, ofd, aflags, iname, oname)
FILE *ifd,
*ofd;

int aflags;

char *iname,
*oname;
{
	register i;

	register long l;

	register int flags,
	 sz;

	char str[50];

	flags = aflags;
	if (flags & WHDR)
	{
		if (putarhd(ofd, &libhd) != 0)
		{
		  iwrerr:
			sprintf(str, "ar: write error on %s", oname);
			perror(str);
			endit();
		}
	}
	l = lp->lfsize;
	while (l)
	{
		if (l < BUFSIZ)
			sz = l;
		else
			sz = BUFSIZ;
		if ((i = fread(buff, sizeof(char), sz, ifd)) == NULL)
		{
			perror("ar: read error");
			endit();
		}
		if (fwrite(buff, sizeof(char), i, ofd) == NULL)
			goto iwrerr;
		l -= i;
	}
	if (flags & OEVEN)
	{
		if (lp->lfsize & 1)
			fwrite("", sizeof(char), 1, ofd);
	}
	if (flags & IEVEN)
		if (lp->lfsize & 1)
			fread(buff, sizeof(char), 1, ifd);
}

ckafile(ap)
char *ap;
{

	if (ap == 0)
		endit();
	if (areof)
	{
		printf("%s not in archive file\n", ap);
		return (1);
	}
	return (0);
}

exall()
{
	while (nextar())
		extract(lp->lfname);
}

tmp2ar()
{
	register int n,
	 ifd,
	 ofd;

	if ((ofd = creat(arname, DEFMODE)) < 0)
	{
		printf("can't create %s\n", arname);
		printf("archive left in %s\n", tempname);
		tempname = 0;					/* keeps endit from removing the archive */
		return;
	}
	fclose(tempfd);
	if ((ifd = open(tempname, 0)) < 0)
	{
		printf("failed to open %s\n", tempname);
		return;
	}
	while ((n = read(ifd, buff, BUFSIZ)) > 0)
		write(ofd, buff, n);
	tempfd = fdopen(ifd, "r");
}

char *fnonly(s)
char *s;
{
	register char *p,
	*r;

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

	return (p);
}

/* this is a copy routine for the cross device archive creation */
copy(from, to)
char *from,
*to;
{
	register int ifd,
	 ofd,
	 len;

	if ((ofd = open(to, WRITE)) == -1)
	{
		if ((ofd = creat(to, DEFMODE)) == -1)
			return (-1);
	}
	if ((ifd = open(from, READ)) == -1)
	{
		close(ofd);
		unlink(to);
		return (-1);
	}

	while ((len = read(ifd, buff, sizeof buff)) > 0)
		write(ofd, buff, len);

	close(ifd);
	close(ofd);
}
