include GNUmakefile.common
include Makefile.silent

SUBDIRS = bios bdos

EXTRA_DIST = \
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

