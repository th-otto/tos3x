include ../Makefile.common
include ../Makefile.silent

PROGRAMS = optimize$(EXEEXT)
LIBS = ${LIBC}

all: $(PROGRAMS)

include SRCFILES
include optimize.mak

LIB1OBJS = \
	$(LIBSRC)osbind.o \
	$(LIBSRC)lmul.o \
	$(LIBSRC)trap.o \
	$(LIBSRC)xread.o \
	$(LIBSRC)xwrite.o \
	$(LIBSRC)uldiv.o \
	$(LIBSRC)salloc.o \
	$(LIBSRC)stricmp.o \
	$(LIBSRC)ctype.o \
	$(LIBSRC)errno.o \
	$(LIBSRC)strcpy.o \
	$(LIBSRC)strncpy.o \
	$(LIBSRC)strcat.o \
	$(LIBSRC)fixxopen.o \
	$(LIBSRC)bugtty.o \
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
	$(LIBSRC)fdecls.o \
	$(LIBSRC)close.o \
	$(LIBSRC)fclose.o \
	$(LIBSRC)cleanup.o \
	$(LIBSRC)exit.o \
	$(empty)

LIB2OBJS = \
	$(LIBSRC)printf.o \
	$(LIBSRC)fprintf.o \
	$(LIBSRC)xmain.o \
	$(empty)

LIBOBJS = $(LIB1OBJS) $(LIB2OBJS)

STARTUP = gemst0.o

# symbols etoa and ftoa are unresolved
optimize$(EXEEXT): $(OPTIMIZE_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(STARTUP) optimize.o $(LIB1OBJS) xomain.o $(LIB2OBJS)


optimize.rel: $(OPTIMIZE_OBJS) optimize.lnk ${MAKEFILE}
	$(CP) $(LIBSRC)$(STARTUP) $(LIBSRC)$(LIBC) $(LIBOBJS) .
	$(RM) $@
	$(LINK68) [un,co[optimize.lnk]]
	$(RM) $(STARTUP) $(LIBC)

#optimize$(EXEEXT): optimize.rel
#	$(RELMOD) $< $@
#	$(PRGFLAGS) 7 7 $@


fixnops$(EXEEXT): fixnops.c
	$(AM_V_CC)${BIN}cp68${EXEEXT} ${CPPFLAGS} $*.c $*.i
	$(AM_V_at)${BIN}c068${EXEEXT} $*.i $*.1 $*.2 $*.3 ${CFLAGS}
	$(AM_V_at)${BIN}c168${EXEEXT} $*.1 $*.2 $*.s
	$(AM_V_at)${RUN_OPTIMIZE_${$*_OPTIMIZE}}
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.s
	$(AM_V_at)${RM} $*.s $*.i $*.1 $*.2 $*.3
	$(AM_V_LD)$(LD) $(LDFLAGS) -o $@ crt0.o $(FIXNOPS_OBJS) $(LIBS)

#
# all the ugly sed scripts are only neccessary to get a binary identical version.
# The code produced by the included compiler is not wrong,
# but apparently the program included in the package was compiled
# by an older version.
# Most of the fixes are assembler related (suppressing optimizing
# of add immediate to addq), some are code generator related
# (current version prefers to use address registers as index)
#
optimize.o: optimize.c fixnops$(EXEEXT)
	$(AM_V_CC)${BIN}cp68${EXEEXT} ${CPPFLAGS} $*.c $*.i
	$(AM_V_at)${BIN}c068${EXEEXT} $*.i $*.1 $*.2 $*.3 ${CFLAGS}
	$(AM_V_at)${BIN}c168${EXEEXT} $*.1 $*.2 $*.s
	$(AM_V_at)${RUN_OPTIMIZE_${$*_OPTIMIZE}}
	$(AM_V_at)$(SED) -e 's/add.l #2,(sp)/dc.w 1687,0,2/' \
	    -e '/move.l \(_[a-z_]*\),R8/N' \
	    -e '/move.l \(_[a-z_]*\),R8\nadd.l \(_[a-z_]*\),R8/N' \
	    -e 's/move.l \(_[a-z_]*\),R8\nadd.l \(_[a-z_]*\),R8\ntst.b (R8)/dc.w 8313\ndc.l \1\ndc.w 8761\ndc.l \2\ndc.w 18992,6144/' \
	    -e '/move \(-[0-9]*\)(R14),R9/N' \
	    -e '/add.l R9,R9/N' \
	    -e 's/move \(-[0-9]*\)(R14),R9.add.l R9,R9.add.l R9,R9/dc.w 12846,\1,18625,58753/' \
	    -e 's/move.l 0(R8,R9.l),(sp)/dc.w 11952,6144/' \
	    -e 's/add.l #\$$\([1-8]\),R0/dc.w 53436,0,\1/' \
	    -e 's/sub.l #\(\$$[0-9]*\),R0/dc.w 37052\ndc.l \1/' \
	    -e 's/sub.l #\(\$$[0-9]*\),R7/dc.w 40636\ndc.l \1/' \
	    -e 's/sub #\([0-9]*\),R0/dc.w 36988,\1/' \
	    -e 's/sub #\([0-9]*\),\(-[0-9]*\)(R14)/dc.w 1134,\1,\2/' \
	    -e 's/sub.l #\(\$$[0-9]*\),\(-[0-9]*\)(R14)/dc.w 1198\ndc.l \1\ndc.w \2/' \
	    -e 's/sub.l #\(\$$[0-9]*\),\(_[a-z_]*\)/dc.w 1209\ndc.l \1,\2/' \
	    -e 's/sub.b #\([0-9]*\),\([0-9]*\)(R8)/dc.w 1064,\1,\2/' \
	    -e 's/add.b #\([0-9]*\),\([0-9]*\)(R8)/dc.w 1576,\1,\2/' \
	    -e 's/add.l #\(\$$[0-9]*\),\(_[a-z_]*\)/dc.w 1721\ndc.l \1,\2/' \
	    -e 's/add #\([0-9]*\),\(-[0-9]*\)(R14)/dc.w 1646,\1,\2/' \
	    -e 's/add #\([0-9]*\),\(_[a-z_]*\)/dc.w 1657,\1\ndc.l \2/' \
	    -e 's/add.l #\(\$$[0-9]*\),\(-[0-9]*\)(R14)/dc.w 1710\ndc.l \1\ndc.w \2/' \
	    -e 's/add.l #\([0-9]*\),\(-[0-9]*\)(R14)/dc.w 1710,0,\1,\2/' \
	    -e 's/add.l #\$$\([1-8]\),R7/dc.w 57020,0,\1/' \
	    -e 's/add #\([0-9]*\),(sp)/dc.w 1623,\1/' \
	    -e 's/move #1,R0/dc.w 12348,1/' \
	    -e '/move -[0-9]*(R14),R9/N' \
	    -e 's/move \(-[0-9]*\)(R14),R9.move.b 0(R8,R9[.l]*),R0/dc.w 12846,\1,18625,4144,6144/' \
	    -e '/move.l _[a-z_]*,R9/N' \
	    -e 's/move.l \(_[a-z_]*\),R9.move.b 3(R8,R9[.l]*),R0/dc.w 8761\ndc.l \1\ndc.w 4144,6147/' \
	    -e '/move -[0-9]*(R14),R9/N' \
	    -e '/move -[0-9]*(R14),R9\nadd.l _[a-z_]*,R9/N' \
	    -e 's/move \(-[0-9]*\)(R14),R9\nadd.l \(_[a-z_]*\),R9\nmove.b 1(R8,R9.l),R0/dc.w 12846,\1,18625,53945\ndc.l \2\ndc.w 4144,6145/' \
	    -e '/move.l R13,R8/N' \
	    -e 's/move.l R13,R8\nadd.l \(-[0-9]*\)(R14),R8/dc.w 8302,\1,53709/' \
	    -e '/move.l -[0-9]*(R14),R8/N' \
	    -e 's/move.l \(-[0-9]*\)(R14),R8\nmove.b \([0-9]*\)(R13,R8.l),R0/dc.w 8238,\1,4149,2048+\2/' \
	    -e '/move.l R13,R8/N' \
	    -e 's/move.l R13,R8\nadd.l \(_[a-z_]*\),R8/dc.w 8313\ndc.l \1\ndc.w 53709/' \
	    $*.s > $*.1; mv $*.1 $*.s
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.s
	$(AM_V_at)${RM} $*.s $*.i $*.1 $*.2 $*.3
	.$(BS)fixnops$(EXEEXT) $@

xomain.o: xomain.c fixnops$(EXEEXT)
	$(AM_V_CC)${BIN}cp68${EXEEXT} ${CPPFLAGS} $*.c $*.i
	$(AM_V_at)${BIN}c068${EXEEXT} $*.i $*.1 $*.2 $*.3 ${CFLAGS}
	$(AM_V_at)${BIN}c168${EXEEXT} $*.1 $*.2 $*.s
	$(AM_V_at)${RUN_OPTIMIZE_${$*_OPTIMIZE}}
	$(AM_V_at)$(SED) -e '/move \(-[0-9]*\)(R14),R8/N' \
	    -e 's/move \(-[0-9]*\)(R14),R8\nmove.b -1(R13,R8),R0/dc.w 12334,\1,36988,1,18624,4149,2048/' \
	    -e 's/sub #\([0-9]*\),\(-[0-9]*\)(R14)/dc.w 1134,\1,\2/' \
	    -e 's/add #\([0-9]*\),\(-[0-9]*\)(R14)/dc.w 1646,\1,\2/' \
	    -e 's/add #\([0-9]*\),(sp)/dc.w 1623,\1/' \
	    $*.s > $*.1; mv $*.1 $*.s
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.s
	$(AM_V_at)${RM} $*.s $*.i $*.1 $*.2 $*.3
	.$(BS)fixnops$(EXEEXT) $@

check::
	cmp optimize$(EXEEXT) ${BIN}optimize$(EXEEXT)

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS) fixnops$(EXEEXT)
