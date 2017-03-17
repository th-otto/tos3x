include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = test.prg apskel.prg doodle.app
LIBS = ${LIBC} ${GEMLIB}
DEPLIBS = ${DEPLIBC} ${DEPGEMLIB}

all: $(PROGRAMS)

include SRCFILES

test.prg: $(TEST_OBJS) ${MAKEFILE} $(DEPLIBS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ gemstart.o $(NOFLOAT) $(TEST_OBJS) $(LIBS)

apskel.prg: $(APSKEL_OBJS) ${MAKEFILE} $(DEPLIBS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ gemstart.o $(NOFLOAT) $(APSKEL_OBJS) $(LIBS)

doodle.app: $(DOODLE_OBJS) ${MAKEFILE} $(DEPLIBS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ gemstart.o $(NOFLOAT) $(DOODLE_OBJS) $(LIBS)

install: all
	@:

check::

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
