top_srcdir=.

include GNUmakefile.cmn
include Makefile.sil

SUBDIRS = common tools bios vdi bdos aes desk system glue
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

FLAGSTOPASS = COUNTRY=$(COUNTRY) TOSVERSION=$(TOSVERSION) SYMBOLS=$(SYMBOLS)

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

WWWDIR=/srv/www/htdocs/tospatch/src

rsync::
	for i in $(SUBDIRS) include tospatch GNUmakefile GNUmakefile.cmn config.mak; do sudo rsync -vzrlp $$i $(WWWDIR); done
	sudo chown -R wwwrun:www $(WWWDIR)
	sudo chmod -R g+w $(WWWDIR)

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
