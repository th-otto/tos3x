include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = doindex$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES
include optimize.mak

doindex$(EXEEXT): $(DOINDEX_OBJS) ldiv.o ${MAKEFILE}
	# expecting unresolved symbols etoa & ftoa
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -h 7 -o $@ $(DOINDEX_OBJS) \
		$(LIBSRC)xbios.o \
		$(LIBSRC)bios.o \
		$(LIBSRC)gemdos.o \
		ldiv.o \
		$(LIBSRC)lmul.o \
		$(LIBSRC)trap.o \
		$(LIBSRC)xread.o \
		$(LIBSRC)xwrite.o \
		$(LIBSRC)uldiv.o \
		$(LIBSRC)salloc.o \
		$(LIBSRC)stricmp.o \
		$(LIBSRC)fdecls.o \
		$(LIBSRC)strlen.o \
		$(LIBSRC)strcpy.o \
		$(LIBSRC)strncpy.o \
		$(LIBSRC)strcmp.o \
		$(LIBSRC)strcat.o \
		$(LIBSRC)fixxopen.o \
		$(LIBSRC)ttyout.o \
		$(LIBSRC)fixlst.o \
		$(LIBSRC)fixwrtc.o \
		$(LIBSRC)writebin.o \
		$(LIBSRC)writeasc.o \
		$(LIBSRC)allocc.o \
		$(LIBSRC)chinit.o \
		$(LIBSRC)chkc.o \
		$(LIBSRC)fixwrite.o \
		$(LIBSRC)sbrk.o \
		$(LIBSRC)prtshort.o \
		$(LIBSRC)prtld.o \
		$(LIBSRC)prtint.o \
		$(LIBSRC)mallocnd.o \
		$(LIBSRC)malloc.o \
		$(LIBSRC)isatty.o \
		$(LIBSRC)flsbuf.o \
		$(LIBSRC)fputc.o \
		$(LIBSRC)fputn.o \
		$(LIBSRC)xdoprtfp.o \
		$(LIBSRC)doprt.o \
		$(LIBSRC)lseek.o \
		$(LIBSRC)open.o \
		$(LIBSRC)fflush.o \
		$(LIBSRC)ctype.o \
		$(LIBSRC)close.o \
		$(LIBSRC)fclose.o \
		$(LIBSRC)cleanup.o \
		$(LIBSRC)exit.o \
		$(LIBSRC)qsort.o \
		$(LIBSRC)puts.o \
		$(LIBSRC)printf.o \
		$(LIBSRC)fprintf.o \
		$(LIBSRC)errno.o \
		$(empty)

check::
	for i in $(PROGRAMS); do cmp $$i ${BIN}$$i; done

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
