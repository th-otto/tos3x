include ../Makefile.common
include ../Makefile.silent

PROGRAMS = c068$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

LIB1OBJS = kputc.o kgetc.o kopen.o kcreat.o

LIBOBJS = \
		$(LIBSRC)kwritef.o \
		$(LIBSRC)xnwmain.o \
		$(LIBSRC)nofloat.o \
		$(LIBSRC)xmain.o \
		$(LIBSRC)optoff.o \
		$(LIBSRC)printf.o \
		$(LIBSRC)fprintf.o \
		$(LIBSRC)read.o \
		$(LIBSRC)readasc.o \
		$(LIBSRC)readbin.o \
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
		$(LIBSRC)strcpy.o \
		$(LIBSRC)strlen.o \
		$(LIBSRC)ctype.o \
		$(LIBSRC)stricmp.o \
		$(LIBSRC)ldiv.o \
		$(LIBSRC)lmul.o \
		$(LIBSRC)lrem.o \
		$(LIBSRC)uldiv.o \
		$(LIBSRC)blivot.o \
		$(LIBSRC)xread.o \
		$(LIBSRC)xwrite.o \
		$(LIBSRC)trap.o \
		\
		$(LIBSRC)unlink.o \
		$(LIBSRC)signal.o \
		$(LIBSRC)xsignal.o \
		$(LIBSRC)getpid.o \
		$(LIBSRC)fpsub.o \
		$(LIBSRC)fpadd.o \
		$(LIBSRC)fpdiv.o \
		$(LIBSRC)fpmul.o \
		$(LIBSRC)fpcmp.o \
		$(LIBSRC)fpneg.o \
		$(LIBSRC)ftol.o \
		$(LIBSRC)ltof.o \
		$(LIBSRC)ffpmul2.o \
		$(LIBSRC)ffpadd.o \
		$(LIBSRC)ffpdiv.o \
		$(LIBSRC)ffpcmp.o \
		$(LIBSRC)ffpabs.o \
		$(LIBSRC)ffpcpyrt.o \
		\
		$(empty)

c068$(EXEEXT): $(C068_OBJS) $(LIB1OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ crtg.o $(C068_OBJS) $(LIB1OBJS) $(LIBOBJS)

c068.rel: $(C068_OBJS) $(LIB1OBJS) c068.lnk ${MAKEFILE}
	$(CP) $(LIBSRC)crtg.o $(LIBSRC)$(LIBC) $(LIBOBJS) .
	$(RM) $@
	$(LINK68) [un,co[c068.lnk]]
	$(RM) crtg.o $(LIBC)

#c068$(EXEEXT): c068.rel
#	$(RELMOD) $< $@
#	$(PRGFLAGS) 7 7 $@


check::
	for i in $(PROGRAMS); do cmp $$i ${BIN}$$i; done

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
