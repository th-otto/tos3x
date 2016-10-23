include GNUmakefile.common
include Makefile.silent

SUBDIRS = common bios vdi bdos tools system

EXTRA_DIST = \
	GNUmakefile \
	GNUmakefile.common \
	Makefile.common \
	Makefile.silent \
	config.mak \
	language.mak \
	$(empty)

all clean distclean dist::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done

dist::
	$(CP) -a $(EXTRA_DIST) $(DISTDIR)

