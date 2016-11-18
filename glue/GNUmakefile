top_srcdir=..
subdir=glue

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.common
include $(top_srcdir)/Makefile.silent

CC += -mshort

LIBS =

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

IMG = tos$(TOSVERSION)$(COUNTRY).img

all: $(IMG)

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

#
# Base of the internal cartridge ROM of St-Book TOS
#
CARTBASE = E40000

cartridge.img: cartridge.o
	$(LD) -nostartfiles -nostdlib -o cartridge.tmp -Wl,--script=../common/rom.ld,-Ttext=0x$(CARTBASE),-Tbss=0 cartridge.o
	$(MV) cartridge.tmp $@
	$(RM) cartridge.tmp

check::
	$(CMP) $(CMPL) -x --ignore-initial=0x0:0x0 --offsets=0x$(rombase):0 $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $(IMG)

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(IMG) tos.img cartridge.tmp cartridge.img $(PROGRAMS) tosrsc*.c glue.* $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak *.img
