include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = lo68$(EXEEXT) reloc$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

lo68$(EXEEXT): $(LO68_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -h 7 -o $@ $(CRT0) nofloat.o $(LO68_OBJS) $(LIBS)

reloc$(EXEEXT): $(RELOC_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -h 7 -o $@ $(CRT0) nofloat.o $(RELOC_OBJS) $(LIBS)

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

clean::
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
