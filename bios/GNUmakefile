top_srcdir=..
subdir=bios

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

LIBS =

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: bios.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

bios.a: $(BIOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BIOS_OBJS)

startup.o: startup.S $(top_srcdir)/common/config.h header.h scrdmp.inc mmu030.inc romcrc.inc dma.inc nvram.inc ikbdclk.inc floppy.inc
	$(CC) -c $(CPPFLAGS) -o $@ $<

vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(CC) -c $(CPPFLAGS) -o $@ $<

tosvars.o: $(top_srcdir)/common/tosvars.S $(top_srcdir)/common/config.h
	$(CC) -c $(CPPFLAGS) -o $@ $<

keytbl.o: $(top_srcdir)/common/config.h

prtkeys$(EXEEXT): prtkeys.o
	$(AM_V_LD)${LD} ${LDFLAGS} -o $@ prtkeys.o

bios.o: tosvars.o vars.o $(BIOS_OBJS) biosext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
	# sections.mak might have just been generated, make sure it is reread
	echo MAKE=$(MAKE)
	$(MAKE) TOSVERSION=$(TOSVERSION) COUNTRY=$(COUNTRY) single

single:
	$(LD) -u -s -l -o bios.o -a $(bios_tbase) $(bios_dbase) 0 tosvars.o vars.o $(BIOS_OBJS) biosext.o

check:: bios.o $(top_srcdir)/common/sections.mak
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(bios_toff):0x24 --offsets=0x$(bios_tbase):0 --bytes=0x$(bios_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(bios_doff):0x$(bios_tleno) --offsets=0x$(bios_dbase):0x$(bios_tlen) --bytes=0x$(bios_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/$(subdir)

clean:
	$(RM) *.o *.a *.ndx $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
