top_srcdir=..
subdir=tools

include $(top_srcdir)/GNUmakefile.common
include $(top_srcdir)/Makefile.silent

LIBS =

CPPFLAGS =
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cmprom$(BUILD_EXEEXT)

all: $(PROGRAMS)

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

cmprom$(BUILD_EXEEXT): cmprom.o ${MAKEFILE}
	$(CC) $(CFLAGS) -o $@ cmprom.o

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
