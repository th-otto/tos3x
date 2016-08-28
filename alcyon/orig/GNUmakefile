SUBDIRS = libsrc link68 cp68 util

all clean check::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done
