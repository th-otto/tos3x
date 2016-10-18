top_srcdir=..
subdir=tools

include $(top_srcdir)/Makefile.common
include $(top_srcdir)/Makefile.silent

LIBS = $(LIBC)

PROGRAMS = cmprom$(BUILD_EXEEXT)

all: $(PROGRAMS)

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

cmprom$(BUILD_EXEEXT): cmprom.o ${MAKEFILE}
	$(LD) -s -l -o $@ $(CRT0) cmprom.o $(LIBS)

check::

dist::

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
