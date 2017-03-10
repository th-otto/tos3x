include GNUmakefile.cmn
include Makefile.sil

SUBDIRS = common bios vdi bdos aes desk tools system glue
EXTRA_SUBDIRS = alcyon alcyon/orig

EXTRA_DIST = \
	GNUmakefile \
	GNUmakefile.cmn \
	Makefile \
	Makefile.cmn \
	Makefile.sil \
	config.mak \
	README \
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
	for i in $(SUBDIRS); do $(MAKE) --no-print-directory -C $$i $@; done

dist::
	$(CP) -a $(EXTRA_DIST) $(DISTDIR)
	$(CP) -a -r include $(DISTDIR)
	$(CP) -a -r bin $(DISTDIR)
	$(CP) -a listings/tos306de.s $(DISTDIR)/listings
	$(CP) -a -r aes/rsc $(DISTDIR)/aes
	$(CP) -a -r desk/rsc $(DISTDIR)/desk
	$(CP) -a lib/*.o lib/*.a lib/*.ndx $(DISTDIR)/lib
	for i in as68 cp68 c068 c168 link68 size68 optimize relmod nm68 ar68; do $(RM) $(DISTDIR)/bin/$$i; done
	(cd $(DISTDIR)/..; rm -f tos306de.tar.bz2; tar cvfj tos306de.tar.bz2 tos306de)
	test -d "$(WWWDIR)" && cp $(DISTDIR)/../tos306de.tar.bz2 "$(WWWDIR)"
