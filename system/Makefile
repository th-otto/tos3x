include ../Makefile.common
include ../Makefile.silent

cache100_ASFLAGS = -n
cache101_ASFLAGS = -n

LIBS = ${LIBC}

PROGRAMS = cache100.prg cache101.prg

all: $(PROGRAMS)

include SRCFILES

cache100.prg: $(CACHE100_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ ${CACHE100_OBJS}

cache101.prg: $(CACHE101_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 1 -o $@ ${CACHE101_OBJS}

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
