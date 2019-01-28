top_srcdir=..
subdir=tospatch

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil

PROGRAMS = tospatch.ttp tosload.prg

all:: $(PROGRAMS)

include SRCFILES

tospatch.ttp: tospatch.o ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ tospatch.o

tosload.prg: tosload.o ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ tosload.o

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean::
	$(RM) *.o *.a *.i *.1 *.2 *.rel $(PROGRAMS)

distclean: clean
