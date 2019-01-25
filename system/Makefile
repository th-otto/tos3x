include ../Makefile.cmn
include ../Makefile.sil

cache100_ASFLAGS = -n
cache100_old_ASFLAGS = -n
foldr100_ASFLAGS = -n

LIBS = ${LIBC}

PROGRAMS = cache100.prg foldr100.prg vdi_fix.prg

all:: $(PROGRAMS)

include SRCFILES

cache100.prg: $(CACHE100_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ ${CACHE100_OBJS}

foldr100.prg: $(FOLDR100_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ ${FOLDR100_OBJS}

cacheold.prg: $(CACHE100_OLD_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 1 -o $@ ${CACHE100_OLD_OBJS}

vdi_fix.prg: $(VDI_FIX_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ ${VDI_FIX_OBJS}

vdi_fix.o: vdifix11/vdi_fix.S
	$(AM_V_AS)${CPP} $< $(CPPOUT) $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

check::

clean::
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
