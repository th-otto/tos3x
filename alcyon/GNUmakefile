include GNUmakefile.common
include Makefile.silent

SUBDIRS = libsrc as cgen cpp link68 optimize parser util

EXTRA_DIST = \
	GNUmakefile \
	GNUmakefile.common \
	Makefile.common \
	Makefile.silent \
	functions.sh \
	README \
	$(empty)

all::
	@:

dist::
	for i in $(SUBDIRS); do $(MKDIR_P) $(DISTDIR)/$$i; done

all clean distclean dist::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done

check::

dist::
	$(CP) -a $(EXTRA_DIST) $(DISTDIR)
	$(CP) -a -r include $(DISTDIR)
	$(CP) -a -r lib $(DISTDIR)
	$(CP) -a -r bin $(DISTDIR)
