include ../Makefile.common
include ../Makefile.silent

PROGRAMS = size68$(EXEEXT)
LIBS = ${LIBC}

all: size68$(EXEEXT)

include SRCFILES

size68$(EXEEXT): $(SIZE_OBJS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -o $@ $(CRT0) $(SIZE_OBJS) $(NOFLOAT) $(LIBS)

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 $(PROGRAMS)
