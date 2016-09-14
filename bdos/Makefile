include ../Makefile.common
include ../Makefile.silent
include optimize.mak

LIBS = ${LIBC}

all: bdos.a

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS3)

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
