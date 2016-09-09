include ../Makefile.common
include ../Makefile.silent

LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: bdos.a

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BDOS_OBJS)

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
