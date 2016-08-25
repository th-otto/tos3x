include ../Makefile.common
include ../Makefile.silent

PROGRAMS = link68$(EXEEXT)
LIBS = ${LIBC}
CPPFLAGS += -DGEMDOS

all: $(PROGRAMS)

include SRCFILES

link68$(EXEEXT): $(LINK_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -u -h 7 -o $@ $(CRT0) nofloat.o $(LINK_OBJS) $(LIBS)

check::
	for i in $(PROGRAMS); do cmp $$i ${BIN}$$i; done

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
