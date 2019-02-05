top_srcdir=..
subdir=tospatch

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil

PROGRAMS = tospatch.ttp loadtos.prg romdump.tos

all:: $(PROGRAMS)

include SRCFILES

tospatch.ttp: tospatch.o ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ tospatch.o

loadtos.prg: loadtos.o ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ loadtos.o

romdump.tos: romdump.o ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -h 7 -o $@ romdump.o

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean::
	$(RM) *.o *.a *.i *.1 *.2 *.rel $(PROGRAMS)

distclean: clean
