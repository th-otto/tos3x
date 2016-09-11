include ../Makefile.common
include ../Makefile.silent

cache100_ASFLAGS = -n

LIBS = ${LIBC}

PROGRAMS = cache100.prg

all: $(PROGRAMS)

include SRCFILES

cache100.prg: $(CACHE_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ ${CACHE_OBJS}

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
