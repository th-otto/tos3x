include GNUmakefile.cmn
include Makefile.sil

SUBDIRS = as cgen cpp link68 optimize parser util cli gemlib
DIST_SUBDIRS = libsrc as cgen cpp link68 optimize parser util cli gemlib

EXTRA_DIST1 = \
	Makefile.cmn \
	Makefile.sil \
	functions.sh \
	README \
	$(empty)

EXTRA_DIST2 = \
	GNUmakefile \
	GNUmakefile.cmn \
	$(empty)

all::
	@:

dist::
	for i in $(SUBDIRS); do $(MKDIR_P) $(DISTDIR2)/$$i; done

dist clean distclean::
	for i in $(DIST_SUBDIRS); do $(MAKE) -C $$i $@; done

all::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done

install64::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done

check::

dist::
	$(CP) -a $(EXTRA_DIST1) $(EXTRA_DIST2) $(DISTDIR2)
	$(CP) -a -r include $(DISTDIR2)
	$(CP) -a -r lib $(DISTDIR2)
	$(CP) -a -r bin $(DISTDIR2)
