include GNUmakefile.common
include Makefile.silent

SUBDIRS = common bios vdi bdos aes desk tools system glue
EXTRA_SUBDIRS = alcyon alcyon/orig

EXTRA_DIST = \
	GNUmakefile \
	GNUmakefile.common \
	Makefile \
	Makefile.common \
	Makefile.silent \
	config.mak \
	$(empty)

all::
	@:

dist::
	for i in $(SUBDIRS) listings lib $(EXTRA_SUBDIRS); do $(MKDIR_P) $(DISTDIR)/$$i; done

all clean distclean dist::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done

dist::
	for i in $(EXTRA_SUBDIRS); do $(MAKE) -C $$i $@; done

check::
	$(MAKE) -C glue $@

dist::
	$(CP) -a $(EXTRA_DIST) $(DISTDIR)
	$(CP) -a -r include $(DISTDIR)
	$(CP) -a -r bin $(DISTDIR)
	$(CP) -a listings/tos306de.s $(DISTDIR)/listings
	$(CP) -a -r aes/rsc $(DISTDIR)/aes
	$(CP) -a -r desk/rsc $(DISTDIR)/desk
	$(CP) -a lib/*.o lib/*.a lib/*.ndx $(DISTDIR)/lib
	(cd $(DISTDIR)/..; rm -f tos306de.tar.bz2; tar cvfj tos306de.tar.bz2 tos306de)
	test -d "$(WWWDIR)" && cp $(DISTDIR)/../tos306de.tar.bz2 "$(WWWDIR)"
