top_srcdir=..

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CPPFLAGS = -I$(INC)
RUN_OPTIMIZE_1 = 

LIBC_OBJS = \
	channel5.o \
	xmainnw.o \
	mallocdb.o \
	noascii.o \
	nobinary.o \
	nodisk.o \
	nofilesz.o \
	nofloat.o \
	nolong.o \
	nottyin.o \
	access.o \
	chmod.o \
	chown.o \
	atoi.o \
	atol.o \
	calloc.o \
	execl.o \
	fdopen.o \
	fgets.o \
	fopen.o \
	fputs.o \
	fread.o \
	freopen.o \
	fseek.o \
	ftell.o \
	fwrite.o \
	getl.o \
	getpass.o \
	gets.o \
	getw.o \
	main.o \
	mktemp.o \
	getpid.o \
	optoff.o \
	perror.o \
	printf.o \
	fprintf.o \
	putl.o \
	puts.o \
	putw.o \
	qsort.o \
	rand.o \
	readl.o \
	rename.o \
	strrchr.o \
	scanf.o \
	setbuf.o \
	sgtty.o \
	sscanf.o \
	doscan.o \
	fgetc.o \
	filbuf.o \
	read.o \
	readasc.o \
	readbin.o \
	swab.o \
	ttyin.o \
	ttyinraw.o \
	ungetc.o \
	unlink.o \
	writel.o \
	xmain.o \
	creat.o \
	exit.o \
	cleanup.o \
	fclose.o \
	close.o \
	fdecls.o \
	fflush.o \
	open.o \
	lseek.o \
	filesz.o \
	sprintf.o \
	doprt.o \
	doprtfp.o \
	fputn.o \
	fputc.o \
	flsbuf.o \
	isatty.o \
	malloc.o \
	mallocnd.o \
	prtint.o \
	prtld.o \
	prtshort.o \
	sbrk.o \
	write.o \
	channels.o \
	writeasc.o \
	writebin.o \
	blkio.o \
	osattr.o \
	wrtchr.o \
	lstout.o \
	ttyout.o \
	xopen.o \
	chkuser.o \
	errno.o \
	strchr.o \
	parsefn.o \
	blkfill.o \
	blkmove.o \
	strcat.o \
	strcmp.o \
	strncat.o \
	strncmp.o \
	strncpy.o \
	strcpy.o \
	strlen.o \
	ctype.o \
	xstrcmp.o \
	yesfloat.o \
	yesstart.o \
	xatof.o \
	abort.o \
	aldiv.o \
	almul.o \
	alrem.o \
	ldiv.o \
	lmul.o \
	lrem.o \
	salloc.o \
	setjmp.o \
	uldiv.o \
	signal.o \
	xsignal.o \
	blivot.o \
	$(empty)

all: crt0.o libc.a

libc.a: $(LIBC_OBJS)
	$(AM_V_at)$(RM) $@
	$(AR) $(ARFLAGS) $@ ${LIBC_OBJS}


clean distclean::
	$(RM) *.o *.a
