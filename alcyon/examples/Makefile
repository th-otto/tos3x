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

#apskel.prg: apskel.rel
#	$(RELMOD) $< $@
#	$(PRGFLAGS) 7 7 $@

apskel.rel: $(APSKEL_OBJS) ${MAKEFILE} gemstart.o $(LIBS)
	$(LINK68) $@=gemstart.o,$(APSKEL_OBJS),${GEMLIB},${LIBC}

doodle.app: $(DOODLE_OBJS) ${MAKEFILE} $(DEPLIBS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ gemstart.o $(NOFLOAT) $(DOODLE_OBJS) $(LIBS)

gemstart.o: $(GEMLIBSRC)gemstart.o
	$(CP) $< $@

${LIBC}: $(LIBSRC)${LIBC}
	$(CP) $< $(LIBSRC)$(LIBC_NDX) .

${GEMLIB}: $(GEMLIBSRC)${GEMLIB}
	$(CP) $< $(GEMLIBSRC)$(GEMLIB_NDX) .

install: all
	@:

check::

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
