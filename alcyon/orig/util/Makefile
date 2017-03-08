include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = size68$(EXEEXT) nm68$(EXEEXT) ar68$(EXEEXT)
LIBS = ${LIBC}

all: $(PROGRAMS)

include SRCFILES

size68$(EXEEXT): $(SIZE_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(CRT0) $(SIZE_OBJS) \
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
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(CRT0) $(NM_OBJS) \
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
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ arcrt.o $(AR_OBJS) \
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
		$(LIBSRC)ffpcp.o \
		$(LIBSRC)ffpdiv.o \
		$(LIBSRC)ffpmul2.o \
		$(LIBSRC)ffpcpyrt.o \
		$(empty)


#
# all the ugly sed scripts are only neccessary to get a binary identical version.
# The code produced by the included compiler is not wrong,
# but apparently the program included in the package was compiled
# by an different compiler version.
#
ar.o: ar.c ${MAKEFILE}
	$(AM_V_CC)${BIN}cp68${EXEEXT} ${CPPFLAGS} $*.c $*.i
	$(AM_V_at)${BIN}c068${EXEEXT} $*.i $*.1 $*.2 $*.3 ${CFLAGS}
	$(AM_V_at)${BIN}c168${EXEEXT} $*.1 $*.2 $*.s
	$(AM_V_at)${RUN_OPTIMIZE_${$*_OPTIMIZE}}
	$(AM_V_at)$(SED) \
	    -e '/move \(-[0-9]*\)(R14),R8/N' \
	    -e '/move \(-[0-9]*\)(R14),R8\nadd.l R8,R8/N' \
	    -e '/move \(-[0-9]*\)(R14),R8\nadd.l R8,R8\nadd.l R8,R8/N' \
	    -e 's/move \(-[0-9]*\)(R14),R8\nadd.l R8,R8\nadd.l R8,R8\n\(move.l 0(R11,R8[.l]*)\)/dc.w 12043,12398,\1,53704,53704,9823\n\2/' \
	    -e '/move R6,R0/N' \
	    -e 's/move R6,R0\nand (R13)+,R0/move (R13)+,R0\nand R6,R0/' \
	    $*.s > $*.1; mv $*.1 $*.s
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.s
	$(AM_V_at)${RM} $*.s $*.i $*.1 $*.2 $*.3

check::
	cmp size68$(EXEEXT) ${BIN}size68$(EXEEXT)
	cmp nm68$(EXEEXT) ${BIN}nm68$(EXEEXT)

clean:
	$(RM) *.o *.a *.i *.1 *.2 $(PROGRAMS)
