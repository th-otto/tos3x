include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = lo68$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES
include optimize.mak

LIB1OBJS = kputc.o kgetc.o kopen.o kcreat.o xmain.o trap.o xwmain.o open.o xopen.o

LIBOBJS = \
		$(LIBSRC)kwritef.o \
		$(LIBSRC)nofloat.o \
		$(LIBSRC)optoff.o \
		$(LIBSRC)printf.o \
		$(LIBSRC)fprintf.o \
		$(LIBSRC)read.o \
		$(LIBSRC)readasc.o \
		$(LIBSRC)readbin.o \
		$(LIBSRC)unlink.o \
		$(LIBSRC)creat.o \
		$(LIBSRC)exit.o \
		$(LIBSRC)cleanup.o \
		$(LIBSRC)fclose.o \
		$(LIBSRC)close.o \
		$(LIBSRC)fdecls.o \
		$(LIBSRC)fflush.o \
		$(LIBSRC)lseek.o \
		$(LIBSRC)doprt.o \
		$(LIBSRC)fputn.o \
		$(LIBSRC)fputc.o \
		$(LIBSRC)flsbuf.o \
		$(LIBSRC)isatty.o \
		$(LIBSRC)malloc.o \
		$(LIBSRC)mallocnd.o \
		$(LIBSRC)prtint.o \
		$(LIBSRC)prtld.o \
		$(LIBSRC)prtshort.o \
		$(LIBSRC)sbrk.o \
		$(LIBSRC)write.o \
		$(LIBSRC)allocc.o \
		$(LIBSRC)chinit.o \
		$(LIBSRC)chkc.o \
		$(LIBSRC)writeasc.o \
		$(LIBSRC)writebin.o \
		$(LIBSRC)errno.o \
		$(LIBSRC)strcat.o \
		$(LIBSRC)strncpy.o \
		$(LIBSRC)strcpy.o \
		$(LIBSRC)strlen.o \
		$(LIBSRC)ctype.o \
		$(LIBSRC)stricmp.o \
		$(LIBSRC)ldiv.o \
		$(LIBSRC)salloc.o \
		$(LIBSRC)uldiv.o \
		$(LIBSRC)blivot.o \
		$(LIBSRC)xread.o \
		$(LIBSRC)xwrite.o \
		$(LIBSRC)lmul.o \
		$(empty)

#lo68$(EXEEXT): crt0.o $(LO68_OBJS) $(LIBOBJS) ${MAKEFILE}
#	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(CRT0) $(LO68_OBJS) $(LIBOBJS)

lo68.rel: crt0.o $(LO68_OBJS) $(LIB1OBJS) lo68.lnk ${MAKEFILE}
	$(CP) $(LIBOBJS) .
	$(RM) $@
	$(LINK68) [co[lo68.lnk]]

lo68$(EXEEXT): lo68.rel
	$(RELMOD) $< $@
	$(PRGFLAGS) 7 7 $@


crt0.o: crt0.S
myflush.o: myflush.c
kgetc.o: kgetc.c
kputc.o: kputc.c
kopen.o: kopen.c
kcreat.o: kcreat.c
xmain.o: xmain.c
xwmain.o: xwmain.c

check::
	cmp lo68$(EXEEXT) ${BIN}lo68$(EXEEXT)

clean::
	$(RM) *.o *.a $(PROGRAMS:.ttp=.s) *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
