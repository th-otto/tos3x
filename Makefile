top_srcdir=.

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil

SUBDIRS = common bios vdi bdos aes desk tools system glue

all::

include $(top_srcdir)/config.mak

FLAGSTOPASS = COUNTRY=$(COUNTRY) TOSVERSION=$(TOSVERSION) SYMBOLS=$(SYMBOLS)

all clean distclean dist check::
	$(MAKE) -C common $(FLAGSTOPASS) $@
	$(MAKE) -C bios $(FLAGSTOPASS) $@
	$(MAKE) -C vdi $(FLAGSTOPASS) $@
	$(MAKE) -C bdos $(FLAGSTOPASS) $@
	$(MAKE) -C aes $(FLAGSTOPASS) $@
	$(MAKE) -C desk $(FLAGSTOPASS) $@
	$(MAKE) -C tools $(FLAGSTOPASS) $@
	$(MAKE) -C system $(FLAGSTOPASS) $@
	$(MAKE) -C glue $(FLAGSTOPASS) $@

maps:
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=306 COUNTRY=de
	cnm -g glue/tos.img > glue/tos306de.map
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=306 COUNTRY=us
	cnm -g glue/tos.img > glue/tos306us.map
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=206 COUNTRY=de
	cnm -g glue/tos.img > glue/tos206de.map
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=206 COUNTRY=us
	cnm -g glue/tos.img > glue/tos206us.map
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=208 COUNTRY=de
	cnm -g glue/tos.img > glue/tos208de.map
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=208 COUNTRY=us
	cnm -g glue/tos.img > glue/tos208us.map
	$(MAKE) clean
	$(RM) glue/*.img glue/glue.*

help::
	@echo ""
	@echo "targets:"
	@echo "   all       - build default configuration TOSVERSION=$(TOSVERSION) COUNTRY=$(COUNTRY)
	@echo "   clean     - remove temporary files"
	@echo "   distclean - remove all generated files"
	@echo ""
	@echo "The resulting output file will be glue/tos$(TOSVERSION)$(COUNTRY).img"
	@echo ""
	@echo "See $(top_srcdir)/config.mak for a list of valid configrations"
