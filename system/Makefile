include ../Makefile.common
include ../Makefile.silent

cache100_ASFLAGS = -n
cache100_old_ASFLAGS = -n
foldr100_ASFLAGS = -n

LIBS = ${LIBC}

PROGRAMS = cache100.prg foldr100.prg

all: $(PROGRAMS)

include SRCFILES

cache100.prg: $(CACHE100_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ ${CACHE100_OBJS}

foldr100.prg: $(FOLDR100_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ ${FOLDR100_OBJS}

cache100_old.prg: $(CACHE100_OLD_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 1 -o $@ ${CACHE100_OLD_OBJS}

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
