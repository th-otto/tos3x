top_srcdir=..
subdir=tools

include $(top_srcdir)/Makefile.common
include $(top_srcdir)/Makefile.silent

LIBS = $(LIBC)

PROGRAMS = newglue$(BUILD_EXEEXT)

# didnt bother to port this to alcyon
#PROGRAMS += cmprom$(BUILD_EXEEXT)

all: $(PROGRAMS)

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

cmprom$(BUILD_EXEEXT): cmprom.o
	$(LD) -u -o $@ $(CRT0) $< $(LIBS)

newglue$(BUILD_EXEEXT): newglue.o
	$(LD) -u -o $@ $(CRT0) $< $(LIBS)

check::

dist::

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
