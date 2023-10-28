include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = lo68$(EXEEXT) reloc$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

LIB1OBJS = kputc.o kgetc.o kopen.o kcreat.o loxmain.o bdtrap.o xwmain.o loopen.o loxopen.o

LIBOBJS = \
		$(LIBSRC)crt0.o \
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
		$(LIBSRC)fopen.o \
		$(LIBSRC)open.o \
		$(LIBSRC)xopen.o \
		$(LIBSRC)fread.o \
		$(LIBSRC)fseek.o \
		$(LIBSRC)ftell.o \
		$(LIBSRC)fwrite.o \
		$(LIBSRC)getw.o \
		$(LIBSRC)putw.o \
		$(LIBSRC)filbuf.o \
		$(LIBSRC)trap.o \
		$(LIBSRC)fgetc.o \
		$(LIBSRC)xnwmain.o \
		$(LIBSRC)xmain.o \
		$(empty)

#lo68$(EXEEXT): crt0.o $(LO68_OBJS) $(LIBOBJS) ${MAKEFILE}
#	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(CRT0) $(LO68_OBJS) $(LIBOBJS)

lo68.rel: locrt0.o $(LO68_OBJS) $(LIB1OBJS) lo68.lnk ${MAKEFILE}
	$(CP) $(LIBOBJS) .
	$(RM) $@
	$(LINK68) [co[lo68.lnk]]

lo68$(EXEEXT): lo68.rel
	$(RELMOD) $< $@
	$(PRGFLAGS) 7 7 $@


reloc.rel: crt0.o $(RELOC_OBJS) $(LIB1OBJS) reloc.lnk ${MAKEFILE}
	$(CP) $(LIBOBJS) .
	$(RM) $@
	$(LINK68) [co[reloc.lnk]]

reloc$(EXEEXT): reloc.rel
	$(RELMOD) $< $@
	$(PRGFLAGS) 7 7 $@


locrt0.o: locrt0.S
bdtrap.o: bdtrap.S
myflush.o: myflush.c
kgetc.o: kgetc.c
kputc.o: kputc.c
kopen.o: kopen.c
kcreat.o: kcreat.c
loxmain.o: loxmain.c
xwmain.o: xwmain.c
loopen.o: loopen.c
loxopen.o: loxopen.c

check::
	cmp lo68$(EXEEXT) ${BIN}lo68$(EXEEXT)
	cmp reloc$(EXEEXT) ${BIN}reloc$(EXEEXT)

clean::
	$(RM) *.o *.a $(PROGRAMS:.ttp=.s) *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
