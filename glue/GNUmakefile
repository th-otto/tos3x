top_srcdir=..
subdir=glue

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

IMG = tos$(TOSVERSION)$(COUNTRY).img

all::

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

all:: $(IMG)

include SRCFILES

STARTUP = startup.o

LIBS = ../bios/bios.a ../vdi/vdi.a ../bdos/bdos.a ../aes/aes.a ../desk/desk.a
OBJS = $(STARTUP) $(BIOS_OBJS) $(VDI_OBJS) $(BDOS_OBJS) $(AES_OBJS) $(DESK_OBJS)

ROMSIZE_206 = 256k
# use 257 for ST-book, since mkrom can't distinguish it from 2.06
ROMSIZE_208 = 257k
ROMSIZE_306 = 512k

ROMSIZE = $(ROMSIZE_$(TOSVERSION))

#
# Base of the internal cartridge ROM of St-Book TOS
#
CARTBASE = E40000

ifeq ($(USE_CROSS_ALCYON),1)
tos.img: $(STARTUP) $(LIBS) tosimg.tmp tosrsc$(COUNTRY).o vars.o tosvars.o
	$(LINK68) '[abs,symbols,locals,text[$(rombase)],bss[0]]' $@=$(STARTUP),vars.o,tosvars.o,../bios/bios.a[all],../vdi/vdi.a[all],../bdos/bdos.a[all],../aes/aes.a[all],../desk/desk.a[all],tosrsc$(COUNTRY).o

# For the PAK/BootJmp option, we need to know the start of the resource area first
tosimg.tmp: $(STARTUP) $(LIBS) rscend.o vars.o tosvars.o
	$(LINK68) '[abs,symbols,text[$(rombase)],bss[0]]' $@=$(STARTUP),vars.o,tosvars.o,../bios/bios.a[all],../vdi/vdi.a[all],../bdos/bdos.a[all],../aes/aes.a[all],../desk/desk.a[all],rscend.o

rscend.h: tosimg.tmp
	tosrsc=`$(BIN)cnm $< | $(GREP) tosrsc | $(SED) -e 's/ .*$$//' -e 's/^00//' | $(TR) abcdef ABCDEF`; \
	fill=`echo "16 i E40030 $$tosrsc - p" | $(DC)`; \
	echo "#define RSCGAP $$fill" > $@
endif

cartridge.img: cartridge.o
ifeq ($(USE_CROSS_ALCYON),1)
	$(LINK68) -o cartridg.tmp -a $(CARTBASE) x 0 cartridg.o
	dd bs=1 skip=36 < cartridg.tmp > $@
else
	$(LD) -nostartfiles -nostdlib -o cartridge.tmp -Wl,--script=../common/rom.ld,-Ttext=0x$(CARTBASE),-Tbss=0 cartridge.o
endif
	$(MV) cartridge.tmp $@
	$(RM) cartridge.tmp

$(IMG): tos.img $(MKROM)
	$(MKROM) pad $(ROMSIZE) $< $@

vdivar.o: ../vdi/vdivar.S
	$(AM_V_AS)$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<
	
vars.o: ../common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

tosvars.o: ../common/tosvars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

include gluer.mak

check:: $(IMG)
	$(CMP) $(CMPL) -x --ignore-initial=0x0:0x0 --offsets=0x$(rombase):0 $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $(IMG)

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(IMG) tos.img tosimg.tmp cartridge.tmp cartridge.img $(PROGRAMS) tosrsc*.c glue.* rscend.h $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak *.img
