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

link68.rel::
	cp ../libsrc/crt0.o .
	cp ../libsrc/libc.a .
	cp ../libsrc/libc.ndx .
	cp ../libsrc/nofloat.o .
	./link68$(EXEEXT) '[s]' $@=crt0.o,nofloat.o,link68.o,preproc.o,lnkmess.o,libc.a

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
