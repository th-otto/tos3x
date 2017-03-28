top_srcdir=..
subdir=common

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

all: $(top_srcdir)/common/sections.mak

include SRCFILES

check::
	@:

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/common
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/common

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/common

clean::

distclean: clean
	$(RM) config.h sections.mak
