top_srcdir=..
subdir=common

include $(top_srcdir)/Makefile.common
include $(top_srcdir)/Makefile.silent

all: $(top_srcdir)/common/sections.mak

include $(top_srcdir)/config.mak

include SRCFILES

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/common

clean::

distclean: clean
	$(RM) config.h sections.mak
