include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = cp68$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

LIB1OBJS = kputc.o kgetc.o kopen.o

LIBOBJS = \
		$(LIBSRC)kcreat.o \
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
		$(LIBSRC)uldiv.o \
		$(LIBSRC)blivot.o \
		$(LIBSRC)xread.o \
		$(LIBSRC)xwrite.o \
		$(LIBSRC)trap.o \
		$(LIBSRC)lmul.o \
		$(empty)

#cp68$(EXEEXT): $(CP_OBJS) $(LIB1OBJS) ${MAKEFILE}
#	$(AM_V_LD)$(LD) $(LDFLAGS) -s -h 7 -o $@ crtg.o $(CP_OBJS) $(LIB1OBJS) $(LIBOBJS)


cp68.rel: $(CP_OBJS) $(LIB1OBJS) cp68.lnk ${MAKEFILE}
	$(CP) $(LIBSRC)crtg.o $(LIBSRC)$(LIBC) $(LIBOBJS) .
	$(RM) $@
	$(LINK68) [un,co[cp68.lnk]]
	$(RM) crtg.o kcreat.o kwritef.o $(LIBC)

cp68$(EXEEXT): cp68.rel
	$(RELMOD) $< $@
	$(PRGFLAGS) 7 7 $@

check::
	cmp cp68$(EXEEXT) ${BIN}cp68$(EXEEXT)

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
