include ../Makefile.cmn
include ../Makefile.sil

inst_PROGRAMS = size68$(EXEEXT) nm68$(EXEEXT)
PROGRAMS = $(inst_PROGRAMS) analyze$(EXEEXT)
LIBS = ${LIBC}

all: $(PROGRAMS)

include SRCFILES

size68$(EXEEXT): $(SIZE_OBJS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -o $@ $(CRT0) nofloat.o $(SIZE_OBJS) $(LIBS)

nm68$(EXEEXT): $(NM_OBJS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -o $@ $(CRT0) nofloat.o $(NM_OBJS) $(LIBS)

analyze$(EXEEXT): $(ANALYZE_OBJS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -o $@ $(CRT0) nofloat.o $(ANALYZE_OBJS) $(LIBS)

install: all
	$(CP) $(inst_PROGRAMS) $(BIN)

check::

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 $(PROGRAMS)
