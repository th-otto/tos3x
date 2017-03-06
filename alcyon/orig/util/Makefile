include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = size68$(EXEEXT) nm68$(EXEEXT) ar68$(EXEEXT)
LIBS = ${LIBC}

all: $(PROGRAMS)

include SRCFILES

size68$(EXEEXT): $(SIZE_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -o $@ $(CRT0) $(SIZE_OBJS) \
		$(LIBSRC)nofloat.o \
		$(LIBSRC)fopen.o \
		$(LIBSRC)getl.o \
		$(LIBSRC)getw.o \
		$(LIBSRC)xmain.o \
		$(LIBSRC)optoff.o \
		$(LIBSRC)printf.o \
		$(LIBSRC)fprintf.o \
		$(LIBSRC)puts.o \
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

nm68$(EXEEXT): $(NM_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -o $@ $(CRT0) $(NM_OBJS) \
		$(LIBSRC)kputchar.o \
		$(LIBSRC)kgetc.o \
		$(LIBSRC)kopen.o \
		$(LIBSRC)kdup.o \
		$(LIBSRC)kwritef.o \
		$(LIBSRC)channel5.o \
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


ar68$(EXEEXT): $(AR_OBJS) arcrt.o ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -s -u -h 7 -o $@ arcrt.o $(AR_OBJS) \
		$(LIBSRC)lseek.o \
		$(LIBSRC)channel5.o \
		$(LIBSRC)fdopen.o \
		$(LIBSRC)fopen.o \
		$(LIBSRC)fread.o \
		$(LIBSRC)fwrite.o \
		$(LIBSRC)xmain.o \
		$(LIBSRC)getpid.o \
		$(LIBSRC)printf.o \
		$(LIBSRC)fprintf.o \
		$(LIBSRC)fgetc.o \
		$(LIBSRC)filbuf.o \
		$(LIBSRC)read.o \
		$(LIBSRC)readasc.o \
		$(LIBSRC)readbin.o \
		$(LIBSRC)unlink.o \
		$(LIBSRC)xwmain.o \
		$(LIBSRC)creat.o \
		$(LIBSRC)exit.o \
		$(LIBSRC)cleanup.o \
		$(LIBSRC)fclose.o \
		$(LIBSRC)close.o \
		$(LIBSRC)fdecls.o \
		$(LIBSRC)fflush.o \
		$(LIBSRC)open.o \
		$(LIBSRC)sprintf.o \
		$(LIBSRC)doprt.o \
		$(LIBSRC)xdoprtfp.o \
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
		$(LIBSRC)writeasc.o \
		$(LIBSRC)writebin.o \
		$(LIBSRC)xopen.o \
		$(LIBSRC)errno.o \
		$(LIBSRC)strcat.o \
		$(LIBSRC)strcmp.o \
		$(LIBSRC)strcpy.o \
		$(LIBSRC)strlen.o \
		$(LIBSRC)ctype.o \
		$(LIBSRC)stricmp.o \
		$(LIBSRC)salloc.o \
		$(LIBSRC)uldiv.o \
		$(LIBSRC)signal.o \
		$(LIBSRC)xsignal.o \
		$(LIBSRC)blivot.o \
		$(LIBSRC)xread.o \
		$(LIBSRC)xwrite.o \
		$(LIBSRC)trap.o \
		$(LIBSRC)lmul.o \
		\
		$(LIBSRC)ftoa.o \
		$(LIBSRC)etoa.o \
		$(LIBSRC)fpadd.o \
		$(LIBSRC)fpcmp.o \
		$(LIBSRC)fpdiv.o \
		$(LIBSRC)ltof.o \
		$(LIBSRC)ftol.o \
		$(LIBSRC)fpmul.o \
		$(LIBSRC)fpneg.o \
		$(LIBSRC)fpsub.o \
		$(LIBSRC)ffpabs.o \
		$(LIBSRC)ffpadd.o \
		$(LIBSRC)ffpcmp.o \
		$(LIBSRC)ffpdiv.o \
		$(LIBSRC)ffpmul2.o \
		$(LIBSRC)ffpcpyrt.o \
		$(empty)


check::
	cmp size68$(EXEEXT) ${BIN}size68$(EXEEXT)
	cmp nm68$(EXEEXT) ${BIN}nm68$(EXEEXT)

clean:
	$(RM) *.o *.a *.i *.1 *.2 $(PROGRAMS)
