include ../Makefile.common
include ../Makefile.silent

PROGRAMS = c168$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

c168$(EXEEXT): $(C168_OBJS) $(LIB1OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -s -h 7 -o $@ $(CRT0) nofloat.o $(C168_OBJS) $(LIBS)

check::

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
