top_srcdir=.

include $(top_srcdir)/Makefile.common
include $(top_srcdir)/Makefile.silent

SUBDIRS = common bios vdi bdos aes tools system glue

all::

include $(top_srcdir)/config.mak

FLAGSTOPASS = COUNTRY=$(COUNTRY) TOSVERSION=$(TOSVERSION) SYMBOLS=$(SYMBOLS)

all clean distclean dist::
	$(MAKE) -C common $(FLAGSTOPASS) $@
	$(MAKE) -C bios $(FLAGSTOPASS) $@
	$(MAKE) -C vdi $(FLAGSTOPASS) $@
	$(MAKE) -C bdos $(FLAGSTOPASS) $@
	$(MAKE) -C aes $(FLAGSTOPASS) $@
	$(MAKE) -C tools $(FLAGSTOPASS) $@
	$(MAKE) -C system $(FLAGSTOPASS) $@
	$(MAKE) -C glue $(FLAGSTOPASS) $@

maps:
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=306 COUNTRY=de
	cnm -g glue/tos306de.img > glue/tos306de.map
	$(MAKE) clean
	$(MAKE) SYMBOLS=-s TOSVERSION=306 COUNTRY=us
	cnm -g glue/tos306us.img > glue/tos306us.map
	$(MAKE) clean
