include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = command.tos find.ttp

all: $(PROGRAMS)

include SRCFILES
include optimize.mak

command.rel: $(COMA_OBJS) $(LIB1OBJS) ${MAKEFILE}
	$(RM) $@
	$(LINK68) $@=coma.o,command.o,lmul.o

command.tos: command.rel
	$(RELMOD) $< $@
	$(PRGFLAGS) 7 7 $@

find.ttp: $(FIND_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(CRT0) $(FIND_OBJS) \
		$(LIBSRC)nofloat.o \
		$(LIBSRC)fgets.o \
		$(LIBSRC)fopen.o \
		$(LIBSRC)fputs.o \
		$(LIBSRC)xmain.o \
		$(LIBSRC)optoff.o \
		$(LIBSRC)printf.o \
		$(LIBSRC)fprintf.o \
		$(LIBSRC)fgetc.o \
		$(LIBSRC)filbuf.o \
		$(LIBSRC)read.o \
		$(LIBSRC)readasc.o \
		$(LIBSRC)readbin.o \
		$(LIBSRC)xwmain.o \
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
		$(LIBSRC)salloc.o \
		$(LIBSRC)uldiv.o \
		$(LIBSRC)blivot.o \
		$(LIBSRC)xread.o \
		$(LIBSRC)xwrite.o \
		$(LIBSRC)trap.o \
		$(LIBSRC)lmul.o \
		$(empty)

lmul.o:	../libsrc/lmul.o
	$(CP) $< $@

lrem.o:	../libsrc/lrem.o
	$(CP) $< $@

ldiv.o:	../libsrc/ldiv.o
	$(CP) $< $@

check:: $(PROGRAMS)
	cmp command.tos ${BIN}command.tos
	cmp find.ttp ${BIN}find.ttp

clean:
	$(RM) *.o *.a command.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
