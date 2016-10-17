top_srcdir=..
subdir=common

all: $(top_srcdir)/common/sections.mak

include SRCFILES

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/common

clean::

distclean: clean
	$(RM) config.h sections.mak
