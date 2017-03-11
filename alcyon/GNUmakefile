include GNUmakefile.cmn
include Makefile.sil

SUBDIRS = libsrc as cgen cpp link68 optimize parser util cli

EXTRA_DIST = \
	GNUmakefile \
	GNUmakefile.cmn \
	Makefile.cmn \
	Makefile.sil \
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
