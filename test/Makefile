include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = hello.tos

all::	$(PROGRAMS)

hello.tos: hello.o
	$(LD) -o $@ $(CRT0) hello.o gemlib.a vdibind.a aesbind.a osbind.o libf.a

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
