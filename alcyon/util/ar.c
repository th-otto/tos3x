/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
 */

#include "../include/compiler.h"
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

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif
#ifndef O_BINARY
#define O_BINARY 0
#endif

#define _(x) x

#define DEFMODE  	0666

#define SYMDEF "._SYMDEF"

static char buff[BUFSIZ];

#define HAVE_STAT 0

#if HAVE_STAT
static struct stat astat, tstat;
#endif

/* flags for cp1file */

#define IEVEN	1
#define OEVEN	2
#define WHDR	4
#define WCOUTHD 8

static struct libhdr libhd;
static struct libhdr *lp = &libhd;

static unsigned short libmagic = LIBMAGIC;

static int rflg;
static int dflg;
static int xflg;
static int tflg;
static int vflg;
static int uflg;
static int pflg;
static int areof;
static FILE *arfp;
static FILE *tempfd;

static int aflg;
static int bflg;
static int psflg;
static int matchflg;
static char *psname;
static char *arname;
static char *tempname;
static char tempnbuf[10];

static char const program_name[] = "ar68";

static struct hdr2 couthd;
static int exitstat;

static VOID (*docom) PROTO((const char *name));


static short const m1[] = { 1, S_IRUSR, 'r', '-' };
static short const m2[] = { 1, S_IWUSR, 'w', '-' };
static short const m3[] = { 1, S_IXUSR, 'x', '-' };
static short const m4[] = { 1, S_IRGRP, 'r', '-' };
static short const m5[] = { 1, S_IWGRP, 'w', '-' };
static short const m6[] = { 1, S_IXGRP, 'x', '-' };
static short const m7[] = { 1, S_IROTH, 'r', '-' };
static short const m8[] = { 1, S_IWOTH, 'w', '-' };
static short const m9[] = { 1, S_IXOTH, 'x', '-' };

static const short *const m[] = { m1, m2, m3, m4, m5, m6, m7, m8, m9 };



static VOID cleanup(NOTHING)
{
	if (tempname)
		unlink(tempname);
	exit(exitstat);
}


static VOID endit(P(int) sig)
PP(int sig;)
{
	UNUSED(sig);
	exitstat = EXIT_FAILURE;
	cleanup();
}


static FILE *openar(P(const char *) arp, P(int) crfl)
PP(const char *arp;)
PP(int crfl;)
{
	register FILE *i;
	unsigned short ib;

	if ((i = fopen(arp, "rb")) == NULL)
	{									/* does not exist */
		areof = 1;
		return NULL;
	}
	if (lgetw(&ib, i) != 0 || (ib != LIBMAGIC && ib != LIBRMAGIC))
	{
		fprintf(stderr, _("%s: not archive format: %s %o\n"), program_name, arp, ib);
		endit(0);
	}
	libmagic = ib;						/* use the same magic number */
	return i;
}




static VOID selectp(P(const short *) pairp, P(int) flg)
PP(const short *pairp;)
PP(int flg;)
{
	register int n;
	register const short *ap;
	register int f;

	ap = pairp;
	f = flg;
	n = *ap++;
	while (--n >= 0 && (f & *ap++) == 0)
		ap++;
	putchar(*ap);
}


static VOID pmode(P(int) aflg1)
PP(int aflg1;)
{
	register const short *const *mp;

	for (mp = &m[0]; mp < &m[9];)
		selectp(*mp++, aflg1);
}


static VOID pfname(NOTHING)
{
	register char *p;
	register int i;

	p = lp->lfname;
	i = LIBNSIZE;
	while (i && *p)
	{
		putchar(*p++);
		i--;
	}
	putchar('\n');
}


static VOID inform(P(char) cc)
PP(char cc;)
{
	if (vflg)
	{
		putchar(cc);
		putchar(' ');
		pfname();
	}
}


static VOID cp1file(P(FILE *) ifd, P(FILE *) ofd, P(int) aflags, P(const char *) iname, P(const char *) oname)
PP(FILE *ifd;)
PP(FILE *ofd;)
PP(int aflags;)
PP(const char *iname;)
PP(const char *oname;)
{
	register int i;
	register long l;
	register int flags, sz;

	flags = aflags;
	if (flags & WHDR)
	{
		if (putarhd(ofd, &libhd) != 0)
		{
		  iwrerr:
			fprintf(stderr, _("%s: write error on %s: %s\n"), program_name, oname, strerror(errno));
			endit(0);
		}
	}
	if (flags & WCOUTHD)
	{
		if (putchd(ofd, &couthd) != 0)
		{
			goto iwrerr;
		}
		l = lp->lfsize;
		if (couthd.ch_magic == EX_ABMAGIC)
			l -= HDSIZ2;
		else
			l -= HDSIZE;
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
			fprintf(stderr, _("%s: read error: %s\n"), program_name, strerror(errno));
			endit(0);
		}
		if (fwrite(buff, sizeof(char), i, ofd) != i)
			goto iwrerr;
		l -= i;
	}
	if (flags & OEVEN)
	{
		if (lp->lfsize & 1)
			fputc(0, ofd);
	}
	if (flags & IEVEN)
	{
		if (lp->lfsize & 1)
			fgetc(ifd);
	}
}


/* call with cpflag = 0 for skip, cpflag = 1 for copy */
static VOID skcopy(P(int) cpflag)
PP(int cpflag;)
{
	register off_t l;

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
		if (fseek(arfp, l, SEEK_CUR) == -1)
		{
			fprintf(stderr, _("%s: seek error on library\n"), program_name);
			endit(0);
		}
	}
}


static VOID copystr(P(const char *) ap1, P(char *) ap2, P(int) alen)
PP(const char *ap1;)
PP(char *ap2;)
PP(int alen;)
{
	register const char *p1;
	register const char *slash;
	register char *p2;
	register int len;
	register char c;
	
	slash = NULL;
	p1 = ap1;
	while (*p1)
	{
		c = *p1++;
		if (c == '/' || c == '\\' || c == ':')
			slash = p1;					/* point to char after last '/' in name */
	}
	if (slash)
		p1 = slash;
	else
		p1 = ap1;
	p2 = ap2;
	len = alen;
	while (len)
	{
		if ((*p2++ = *p1++) == '\0')
			break;
		len--;
	}
	while (--len > 0)
		*p2++ = '\0';
}


static int ckafile(P(const char *) ap)
PP(const char *ap;)
{
	if (ap == NULL || *ap == '\0')
		cleanup();
	if (areof)
	{
		fprintf(stderr, _("%s: %s not in archive file\n"), program_name, ap);
		return TRUE;
	}
	return FALSE;
}


/* execute one command -- call with filename or 0 */

static VOID tellar68(P(const char *) ap)
PP(const char *ap;)
{
	register char *p1;

	if (ap != (const char *)-1 && ckafile(ap))
		return;
	if (vflg)
	{									/* long list */
		time_t t;
		
		pmode(lp->lfimode);
		printf(" %d/%d ", lp->luserid, lp->lgid);
		printf("%6ld", (long)lp->lfsize);
		t = lp->lmodti;
		p1 = ctime(&t);
		p1[24] = '\0';
		p1 += 4;
		printf(" %s  ", p1);
	}
	pfname();
	skcopy(0);
}


/* read next ar file header into libhd */

static int nextar(NOTHING)
{
	if (areof || getarhd(arfp, &libhd) == EOF || feof(arfp) || *libhd.lfname == '\0')
	{
		areof++;
		return FALSE;
	}
	return TRUE;
}


static VOID cprest(NOTHING)
{
	while (nextar())
		skcopy(1);						/* copy rest of library */
}


static VOID replace(P(const char *) name)
PP(const char *name;)
{
	register FILE *ifd;

#if HAVE_STAT
	register struct stat *stp;
	struct stat stbuf;
	stp = &stbuf;
#endif

	if (name == NULL)
	{
		if (bflg && areof == 0)
		{
			if (fseek(arfp, -(long) LIBHDSIZE, 1) == -1)
				fprintf(stderr, _("%s: %s: fseek error\n"), program_name, arname);
		}
		cprest();
		return;
	}
#if HAVE_STAT
	if (stat(name, stp) < 0)
	{
		fprintf(stderr, _("%s: can't find %s: %s\n"), program_name, name, strerror(errno));
		endit(0);
	}
#endif
	if ((ifd = fopen(name, "rb")) == NULL)
	{
		fprintf(stderr, _("%s: can't open %s: %s\n"), program_name, name, strerror(errno));
		endit(0);
	}
	/* BUG: ar should not depend on members being object files */
	if (getchd(ifd, &couthd) < 0)
	{
		fprintf(stderr, _("%s: can't read %s: %s\n"), program_name, name, strerror(errno));
		endit(0);
	}
	if (areof && psflg)
	{
		fprintf(stderr, _("%s: %s not in library\n"), program_name, psname);
		endit(0);
	}
	if ((bflg | aflg) && matchflg == 0)
	{									/* copy archive before appending */
		if (aflg)
			skcopy(1);
		matchflg++;
	}
	copystr(name, lp->lfname, LIBNSIZE);
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
#if HAVE_STAT
	lp->luserid = stp->st_uid;
	lp->lgid = stp->st_gid;
	lp->lfimode = stp->st_mode;
	lp->lmodti = stp->st_mtime;
	lp->lfsize = stp->st_size;
#else
	lp->lfsize = couthd.ch_tsize + couthd.ch_dsize + couthd.ch_ssize;
	if (couthd.ch_magic == EX_ABMAGIC)
		lp->lfsize += HDSIZ2;
	else
		lp->lfsize += HDSIZE;
	lp->lfimode = 0644;
	if (couthd.ch_rlbflg == 0)
		lp->lfsize += couthd.ch_tsize + couthd.ch_dsize;
#endif
	fseek(ifd, 0L, SEEK_SET);
	cp1file(ifd, tempfd, WHDR + OEVEN, name, tempname);
	fclose(ifd);
}


static VOID ddelete(P(const char *) ap)
PP(const char *ap;)
{
	if (ap == NULL)
	{
		cprest();
		fseek(tempfd, 0L, SEEK_SET);
		lputw(&libmagic, tempfd);
		return;
	}
	if (strcmp(ap, SYMDEF) == 0)
		libmagic = LIBMAGIC;
	inform('d');
	skcopy(0);
}


static VOID extract(P(const char *) ap)
PP(const char *ap;)
{
	register FILE *ofd;
	register int i;
	mode_t mode;
	
	if (ckafile(ap))
		return;
	mode = lp->lfimode & 0777;
	if (!(mode & S_IWUSR))
		mode = DEFMODE;
	if ((i = open(ap, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, mode)) < 0)
	{
		fprintf(stderr, _("%s: can't create %s: %s\n"), program_name, ap, strerror(errno));
		endit(0);
	}
	ofd = fdopen(i, "wb");
	inform('x');
	cp1file(arfp, ofd, IEVEN, arname, ap);
	fclose(ofd);
}


static VOID prshort(P(const char *) ap)
PP(const char *ap;)
{
	if (ckafile(ap))
		return;
	cp1file(arfp, stdout, IEVEN, arname, "std output");
}


/* list all file in the library */

static VOID listall(NOTHING)
{
	while (nextar())
		tellar68((const char *) -1);
}


static VOID exall(NOTHING)
{
	if (nextar())
	{
		if (strcmp(lp->lfname, SYMDEF) == 0)
		{
			fseek(arfp, lp->lfsize, SEEK_CUR);
			if (nextar() == FALSE)
				return;
		}
	}
	do {
		extract(lp->lfname);
	} while (nextar());
}


static VOID tmp2ar(NOTHING)
{
	register int n, ifd, ofd;

	if ((ofd = open(arname, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, DEFMODE)) < 0)
	{
		fprintf(stderr, _("%s: can't create %s: %s\narchive left in %s\n"), program_name, arname, strerror(errno), tempname);
		tempname = NULL;					/* keeps endit from removing the archive */
		return;
	}
	fclose(tempfd);
	if ((ifd = open(tempname, O_RDONLY|O_BINARY)) < 0)
	{
		fprintf(stderr, _("%s: failed to open %s\n"), program_name, tempname);
		return;
	}
	while ((n = read(ifd, buff, BUFSIZ)) > 0)
		write(ofd, buff, n);
	buff[0] = buff[1] = buff[2] = buff[3] = 0;
	write(ofd, buff, 4);
	close(ofd);
	tempfd = fdopen(ifd, "rb");
}


static const char *fnonly(P(const char *) s)
PP(const char *s;)
{
	register const char *p;
	register const char *r;

	r = s;
	p = NULL;
	while (*r)
	{
		if (*r == '/' || *r == '\\')
			p = r;
		r++;
	}
	if (p)
	{
		++p;
		if (*p == 0)
			p = s;
	} else
	{
		p = s;
	}
	
	return p;
}


#if HAVE_STAT
/* this is a copy routine for the cross device archive creation */
static int copy(P(const char *) from, P(const char *) to)
PP(const char *from;)
PP(const char *to;)
{
	register int ifd, ofd, len;

	if ((ofd = open(to, O_WRONLY|O_BINARY)) == -1)
	{
		if ((ofd = open(to, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, DEFMODE)) == -1)
			return -1;
	}
	if ((ifd = open(from, O_RDONLY|O_BINARY)) == -1)
	{
		close(ofd);
		unlink(to);
		return -1;
	}

	while ((len = read(ifd, buff, sizeof buff)) > 0)
		write(ofd, buff, len);

	close(ifd);
	close(ofd);
	return 0;
}
#endif


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register char *p1;
	register char **ap;
	register int i;
	int j, docopy;

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	/* printf(_("AR68 Archiver (c) 1985 Alcyon Corporation\n")); */
	exitstat = EXIT_SUCCESS;
	if (argc < 3)
	{
	  usage:
		fprintf(stderr, _("usage: %s [rdxtpvabi] [pos] archive file [file ...]\n"), program_name);
		endit(0);
	}
	signal(SIGHUP, endit);
	signal(SIGINT, endit);
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
			docom = ddelete;
			break;
		case 'x':
			xflg++;
			docom = extract;
			break;
		case 't':
			tflg++;
			docom = tellar68;
			break;
		case 'p':
			pflg++;
			docom = prshort;
			break;
		case 'v':
			vflg++;
			break;
		case '-':
			break;
		default:
			fprintf(stderr, _("%s: invalid option flag: %c\n"), program_name, *--p1);
			endit(0);
			break;
		}
	}
	uflg = rflg + dflg;
	if ((uflg + xflg + tflg + pflg) != 1)
	{
		fprintf(stderr, _("%s: one and only one of 'rdxt' flags required\n"), program_name);
		endit(0);
	}
	psflg = aflg + bflg;
	if (psflg > 1)
	{
		fprintf(stderr, _("%s: only one of 'abi' flags allowed\n"), program_name);
		endit(0);
	}
	if (psflg && (rflg != 1))
	{
		fprintf(stderr, _("%s: 'abi' flags can only be used with 'r' flag\n"), program_name);
		endit(0);
	}
	arname = *ap++;
	arfp = openar(arname, rflg);
	if (arfp == NULL && (tflg + xflg + bflg + aflg + dflg))
	{
		fprintf(stderr, _("%s: no such file '%s'\n"), program_name, arname);
		endit(0);
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
	if (libmagic == LIBRMAGIC && uflg)
		fprintf(stderr, _("%s: warning, changing a random access library\n"), program_name);
	if (!pflg && !tflg)
	{
		strcpy(tempnbuf, "arXXXXXX");
		tempname = mktemp(tempnbuf);
		if ((tempfd = fopen(tempname, "wb")) == NULL)
		{
			fprintf(stderr, _("%s: can't create %s: %s\n"), program_name, tempname, strerror(errno));
			endit(0);
		}

		if (lputw(&libmagic, tempfd) != 0)
			fprintf(stderr, _("%s: write error on magic number: %s\n"), program_name, strerror(errno));
	}

	/* read archive, executing appropriate commands */
	while (matchflg == 0 && nextar())
	{
		docopy = 1;
		if (!psflg)
		{
			for (j = 0; j < i; j++)
			{
				if (strcmp(fnonly(ap[j]), lp->lfname) == 0)
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
		} else if (strcmp(psname, lp->lfname) == 0)
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
				fprintf(stderr, "%s not found\n", ap[j]);
				exitstat = EXIT_FAILURE;
			}
		}
	}
	(*docom) (NULL);

	if (tflg || pflg)					/* 20 sep 83, don't need to do this stuff... */
		cleanup();

#if HAVE_STAT
	/* make temp file the archive file */
	if (stat(tempname, &tstat) < 0)
	{
		fprintf(stderr, _("%s: can't find %s: %s\n", program_name, tempname, strerror(errno));
		endit(0);
	}
	if (arfp != NULL)
	{
		if (stat(arname, &astat) < 0)
		{
			fprintf(stderr, _("%s: can't find %s: %s\n"), program_name, arname, strerror(errno));
			endit(0);
		}
		if (astat.st_nlink != 1 || astat.st_dev != tstat.st_dev)
		{
			fclose(arfp);
			tmp2ar();
		} else if (unlink(arname) == -1)
		{
			fprintf(stderr, _("%s: can't unlink old archive: %s\nnew archive left in %s\n"), program_name, strerror(errno), tempname);
			tempname = NULL;				/* keeps endit from removing the archive */
		} else if (rename(tempname, arname) < 0)
		{
			if (copy(tempname, arname) < 0)
			{
				fprintf(stderr, _("%s: can't link to %s: %s\nnew archive left in %s\n"), program_name, arname, strerror(errno), tempname);
			}
			tempname = NULL;
		} else
		{
			tempname = NULL;
		}
	} else
	{
		if ((arfp = fopen(arname, "wb")) == NULL)
		{
			fprintf(stderr, _("%s: can't create %s: %s\narchive left in %s\n"), program_name, arname, strerror(errno), tempname);
			tempname = NULL;				/* keeps endit from removing the archive */
			endit();
		}
		if (stat(arname, &astat) < 0)
		{
			fprintf(stderr, _("%s: can't find %s: %s\n"), program_name, arname, strerror(errno));
			endit(0);
		}
		fclose(arfp);
		if (astat.st_dev != tstat.st_dev)
		{
			tmp2ar();
			exitstat = EXIT_SUCCESS;
		} else if (unlink(arname) < 0 || rename(tempname, arname) < 0)
		{
			fprintf(stderr, _("%s: can't link to %s: %s\nnew archive left in %s\n"), program_name, arname, strerror(errno), tempname);
			tempname = NULL;				/* keeps endit from removing the archive */
		} else
		{
			tempname = NULL;
		}
	}
	cleanup();
#else
	if (arfp != NULL)
	{
		fclose(arfp);
		arfp = NULL;
	}
	if (tflg || pflg)
		cleanup();
	tmp2ar();
	exitstat = EXIT_SUCCESS;
	cleanup();
#endif

	return EXIT_SUCCESS;
}
