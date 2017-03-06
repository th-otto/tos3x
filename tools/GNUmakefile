top_srcdir=..
subdir=tools

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

LIBS =

CPPFLAGS =
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cmprom$(BUILD_EXEEXT) newglue$(BUILD_EXEEXT) findrsc$(BUILD_EXEEXT) mkbininc$(BUILD_EXEEXT) mkrom$(BUILD_EXEEXT)

all: $(PROGRAMS)

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

cmprom$(BUILD_EXEEXT): cmprom.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

newglue$(BUILD_EXEEXT): newglue.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

findrsc$(BUILD_EXEEXT): findrsc.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

mkbininc$(BUILD_EXEEXT): mkbininc.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

mkrom$(BUILD_EXEEXT): mkrom.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
