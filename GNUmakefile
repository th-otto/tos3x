include GNUmakefile.common
include Makefile.silent

SUBDIRS = common bios vdi bdos aes tools system

EXTRA_DIST = \
	GNUmakefile \
	GNUmakefile.common \
	Makefile.common \
	Makefile.silent \
	config.mak \
	$(empty)

all::
	@:

dist::
	for i in $(SUBDIRS) desk listings; do $(MKDIR_P) $(DISTDIR)/$$i; done

all clean distclean dist::
	for i in $(SUBDIRS); do $(MKDIR_P) $(DISTDIR)/$$i; $(MAKE) -C $$i $@; done

dist::
	$(CP) -a $(EXTRA_DIST) $(DISTDIR)
	$(CP) -a -r include $(DISTDIR)
	$(CP) -a -r bin $(DISTDIR)
	touch $(DISTDIR)/desk/deskdefs.h
	cp -a listings/tos306de.s $(DISTDIR)/listings
	(cd $(DISTDIR)/..; rm -f tos306de.tar.bz2; tar cvfj tos306de.tar.bz2 tos306de)
	test -d "$(WWWDIR)" && cp $(DISTDIR)/../tos306de.tar.bz2 "$(WWWDIR)"
