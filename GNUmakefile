top_srcdir=.

include GNUmakefile.cmn
include Makefile.sil

SUBDIRS = common tools tospatch bios vdi bdos aes desk system glue
EXTRA_SUBDIRS = alcyon alcyon/orig

EXTRA_DIST1 = \
	Makefile \
	Makefile.cmn \
	Makefile.sil \
	config.mak \
	README \
	$(empty)

EXTRA_DIST2 = \
	GNUmakefile \
	GNUmakefile.cmn \
	$(empty)

all::
	@:

include $(top_srcdir)/config.mak

FLAGSTOPASS = COUNTRY=$(COUNTRY) TOSVERSION=$(TOSVERSION)

dist::
	rm -rf $(DISTDIR1) $(DISTDIR2)
	for i in $(SUBDIRS) lib; do $(MKDIR_P) $(DISTDIR1)/$$i; done
	for i in $(SUBDIRS) $(EXTRA_SUBDIRS) listings; do $(MKDIR_P) $(DISTDIR2)/$$i; done

all dist::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $(FLAGSTOPASS) $@ || exit 1; done

clean distclean::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $(FLAGSTOPASS) $@; done

dist::
	for i in $(EXTRA_SUBDIRS); do $(MAKE) -C $$i $@; done

check::
	for i in $(SUBDIRS); do $(MAKE) --no-print-directory -C $$i $(FLAGSTOPASS) all; done
	for i in $(SUBDIRS); do $(MAKE) --no-print-directory -C $$i $(FLAGSTOPASS) $@; done

rsync::
	for i in $(SUBDIRS) include GNUmakefile GNUmakefile.cmn config.mak; do sudo rsync -vzrlpt $$i $(LOCAL_WWWDIR); done
	sudo chown -R wwwrun:www $(LOCAL_WWWDIR)
	sudo chmod -R g+w $(LOCAL_WWWDIR)

maps:
	for version in 306 206; do \
		for lang in us de fr es it se sf sg; do \
			$(MAKE) clean; \
			$(MAKE) SYMBOLS=-s TOSVERSION=$${version} COUNTRY=$${lang} || exit 1; \
			cnm glue/tos.img | sort | uniq > glue/tos$${version}$${lang}.map; \
		done; \
	done
	for version in 208; do \
		for lang in de us fr; do \
			$(MAKE) clean; \
			$(MAKE) SYMBOLS=-s TOSVERSION=$${version} COUNTRY=$${lang} || exit 1; \
			cnm glue/tos.img | sort | uniq > glue/tos$${version}$${lang}.map; \
		done; \
	done
	$(MAKE) clean
	$(RM) glue/*.img glue/glue.*

dosdir::
	for i in $(SUBDIRS) lib; do $(MKDIR_P) $(DOSDIR)/$$i; done
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done
	$(CP) -a $(EXTRA_DIST1) $(DOSDIR)
	$(CP) -a -r include $(DOSDIR)
	$(CP) -a -r bin $(DOSDIR)
	$(CP) -a -r aes/rsc $(DOSDIR)/aes
	$(CP) -a -r desk/rsc $(DOSDIR)/desk
	$(CP) -a lib/*.o lib/*.a lib/*.ndx lib/as68symb.dat $(DOSDIR)/lib
	for i in as68 cp68 c068 c168 link68 size68 optimize relmod nm68 ar68; do $(RM) $(DOSDIR)/bin/$$i; done

dist::
	$(CP) -a $(EXTRA_DIST1) $(DISTDIR1)
	$(CP) -a -r include $(DISTDIR1)
	$(CP) -a -r bin $(DISTDIR1)
	$(CP) -a -r bin64 $(DISTDIR1)
	$(CP) -a -r aes/rsc $(DISTDIR1)/aes
	$(CP) -a -r desk/rsc $(DISTDIR1)/desk
	$(CP) -a lib/*.o lib/*.a lib/*.ndx lib/as68symb.dat $(DISTDIR1)/lib
	$(CP) -a listings/tos306de.s $(DISTDIR2)/listings
	(cd $(DISTDIR1); rm -f ../tos306de.tar.bz2; tar cvfj ../tos306de.tar.bz2 .)
	(cd $(DISTDIR2); rm -f ../alcyon.tar.bz2; tar cvfj ../alcyon.tar.bz2 .)
	test -d "$(WWWDIR)" && cp $(DISTDIR1)/../tos306de.tar.bz2 "$(WWWDIR)"
	test -d "$(WWWDIR)" && cp $(DISTDIR2)/../alcyon.tar.bz2 "$(WWWDIR)"

help::
	@echo ""
	@echo "targets:"
	@echo "   all       - build default configuration TOSVERSION=$(TOSVERSION) COUNTRY=$(COUNTRY)"
	@echo "   clean     - remove temporary files"
	@echo "   distclean - remove all generated files"
	@echo ""
	@echo "The resulting output file will be glue/tos$(TOSVERSION)$(COUNTRY).img"
	@echo ""
	@echo "See $(top_srcdir)/config.mak for a list of valid configurations"
