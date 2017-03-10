top_srcdir=..
subdir=common

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

all: $(top_srcdir)/common/sections.mak

include SRCFILES

check::
	@:

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/common

clean::

distclean: clean
	$(RM) config.h sections.mak
