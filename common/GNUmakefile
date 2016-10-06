top_srcdir=..
subdir=common

all:

include SRCFILES

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/common

clean::

distclean: clean
	$(RM) $(top_srcdir)/common/config.h
