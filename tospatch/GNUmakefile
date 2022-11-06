top_srcdir=..
subdir=tospatch

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

SUBDIRS = src

EXTRA_DIST1 = \
	Makefile \
	data \
	patches \
	$(empty)

EXTRA_DIST2 = \
	GNUmakefile \
	$(empty)

all::
	@:

include $(top_srcdir)/config.mak

FLAGSTOPASS = COUNTRY=$(COUNTRY) TOSVERSION=$(TOSVERSION)

dist::
	for i in $(SUBDIRS); do $(MKDIR_P) $(top_srcdir)/$(DISTDIR1)/$(subdir)/$$i; done
	$(CP) -a -r $(EXTRA_DIST1) $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR1)/$(subdir)

all dist::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $(FLAGSTOPASS) $@ || exit 1; done

clean distclean::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $(FLAGSTOPASS) $@; done

check::
	@:

dosdir::
	@:
