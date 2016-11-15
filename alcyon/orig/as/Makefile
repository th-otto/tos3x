include ../Makefile.common
include ../Makefile.silent

PROGRAMS = as68$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

LIB1OBJS = kputc.o kopen.o kcreat.o

LIBOBJS = \
		$(LIBSRC)kwritef.o \
		$(LIBSRC)xnwmain.o \
		$(LIBSRC)nofloat.o \
		$(LIBSRC)xmain.o \
		$(LIBSRC)mktemp.o \
		$(LIBSRC)getpid.o \
		$(LIBSRC)optoff.o \
		$(LIBSRC)printf.o \
		$(LIBSRC)fprintf.o \
		$(LIBSRC)qsort.o \
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
		$(LIBSRC)open.o \
		$(LIBSRC)lseek.o \
		$(LIBSRC)sprintf.o \
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
		$(LIBSRC)xopen.o \
		$(LIBSRC)errno.o \
		$(LIBSRC)strcat.o \
		$(LIBSRC)strncmp.o \
		$(LIBSRC)strcpy.o \
		$(LIBSRC)strlen.o \
		$(LIBSRC)ctype.o \
		$(LIBSRC)stricmp.o \
		$(LIBSRC)ldiv.o \
		$(LIBSRC)lmul.o \
		$(LIBSRC)uldiv.o \
		$(LIBSRC)signal.o \
		$(LIBSRC)xsignal.o \
		$(LIBSRC)blivot.o \
		$(LIBSRC)xread.o \
		$(LIBSRC)xwrite.o \
		$(LIBSRC)trap.o \
		$(empty)

#as68$(EXEEXT): $(AS68_OBJS) $(LIB1OBJS) ${MAKEFILE}
#	$(AM_V_LD)$(LD) $(LDFLAGS) -s -h 7 -o $@ $(CRT0) $(AS68_OBJS) $(LIB1OBJS) $(LIBOBJS)

as68.rel: $(AS68_OBJS) $(LIB1OBJS) as68.lnk ${MAKEFILE}
	$(CP) $(LIBSRC)crt0.o $(LIBSRC)$(LIBC) $(LIBOBJS) .
	$(RM) $@
	$(LINK68) [co[as68.lnk]]
	$(RM) crt0.o $(LIBC)

as68$(EXEEXT): as68.rel
	$(RELMOD) $< $@
	$(PRGFLAGS) 7 7 $@


check::
	cmp as68$(EXEEXT) ${BIN}as68$(EXEEXT)

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
