include ../Makefile.common
include ../Makefile.silent

PROGRAMS = c068$(EXEEXT)
LIBS = ${LIBC} ${LIBM}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

c068$(EXEEXT): $(C068_OBJS) $(LIB1OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(CRT0) nofloat.o $(C068_OBJS) $(LIBS)

check::
	for i in $(PROGRAMS); do cmp $$i ${BIN}$$i; done

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
