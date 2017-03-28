include GNUmakefile.cmn
include Makefile.sil

SUBDIRS = libsrc as cgen cp68 link68 optimize parser util cli

EXTRA_DIST1 = \
	Makefile.cmn \
	Makefile.sil \
	README \
	$(empty)

EXTRA_DIST2 = \
	GNUmakefile \
	GNUmakefile.cmn \
	functions.sh \
	$(empty)

all::
	@:

dist::
	for i in $(SUBDIRS); do $(MKDIR_P) $(DISTDIR2)/$$i; done

all clean check dist::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done

check::

dist::
	$(CP) -a $(EXTRA_DIST1) $(EXTRA_DIST2) $(DISTDIR2)
	$(CP) -a -r include $(DISTDIR2)
	$(CP) -a -r lib $(DISTDIR2)
	$(CP) -a -r bin $(DISTDIR2)
