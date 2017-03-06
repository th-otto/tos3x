include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = optimize$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

# symbols etoa and ftoa are unresolved
optimize$(EXEEXT): $(OPTIMIZE_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -h 7 -o $@ $(CRT0) nofloat.o $(OPTIMIZE_OBJS) $(LIBS)

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::
	for i in $(PROGRAMS); do cmp $$i ${BIN}$$i; done

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
