top_srcdir=..

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil

SUBDIRS = src

all::

include $(top_srcdir)/config.mak

FLAGSTOPASS = COUNTRY=$(COUNTRY) TOSVERSION=$(TOSVERSION)

all clean distclean dist check::
	$(MAKE) -C src $(FLAGSTOPASS) $@
