include Makefile.common
include Makefile.silent

SUBDIRS = common bios vdi bdos aes tools system

all clean distclean dist::
	$(MAKE) -C common $@
	$(MAKE) -C bios $@
	$(MAKE) -C vdi $@
	$(MAKE) -C bdos $@
	$(MAKE) -C aes $@
	$(MAKE) -C tools $@
	$(MAKE) -C system $@
