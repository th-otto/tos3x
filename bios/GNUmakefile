# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.common
include ../Makefile.silent

CC += -mshort

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: bios.a

include ../config.mak
include ../language.mak

include SRCFILES

bios.a: $(BIOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BIOS_OBJS)

startup.o: startup.S header.h

$(BIOS_OBJS) tosvars.o vars.o: config.h

check::

dist::
	$(CP) -a $(BIOS_SRCS) $(EXTRA_DIST) ../$(DISTDIR)/bios

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
	$(RM) config.h
