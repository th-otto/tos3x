include ../Makefile.common
include ../Makefile.silent

PROGRAMS = as68$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

as68$(EXEEXT): $(AS68_OBJS) $(LIB1OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -h 7 -o $@ $(CRT0) nofloat.o $(AS68_OBJS) $(LIBS)

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
