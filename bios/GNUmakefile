top_srcdir=..
subdir=bios

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

all: bios.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

bios.a: $(BIOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BIOS_OBJS)

startup.o: startup.S $(top_srcdir)/common/config.h header.h scrdmp.inc mmu030.inc romcrc.inc dma.inc nvram.inc ikbdclock.inc floppy.inc

keytbl.o: $(top_srcdir)/common/config.h

prtkeys$(EXEEXT): prtkeys.o
	$(AM_V_LD)${LD} ${LDFLAGS} -o $@ prtkeys.o

check:: bios.o $(top_srcdir)/common/sections.mak
	$(AM_V_at)offset=`echo 16 i $(bios_tbase) $(rombase) - p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=$$offset:0x24 --offsets=0x$(rombase):0x24 --bytes=0x$(bios_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<
	$(AM_V_at)offset=`echo 16 i $(bios_dbase) $(rombase) - p | dc`; \
	offset2=`echo 16 i $(bios_tlen) 24 + p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=$$offset:$$offset2 --offsets=0x$(rombase):0x24 --bytes=0x$(bios_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
