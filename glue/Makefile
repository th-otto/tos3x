top_srcdir=..
subdir=glue

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil

IMG = tos$(TOSVERSION)$(COUNTRY).img

all: $(IMG)

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

STARTUP = startup.o

LIBS = ../bios/bios.a ../vdi/vdi.a ../bdos/bdos.a ../aes/aes.a ../desk/desk.a
LIBSNDX = ../bios/bios.ndx ../vdi/vdi.ndx ../bdos/bdos.ndx ../aes/aes.ndx ../desk/desk.ndx
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

$(IMG): tos.img $(MKROM)
	$(MKROM) pad $(ROMSIZE) $< $@

tos.img: $(STARTUP) $(LIBS) tosrsc$(COUNTRY).o vars.o
	$(LD) -o $@ $(SYMBOLS) -a $(rombase) x 0 $(STARTUP) -x ..$(BS)bios$(BS)bios.a -x ..$(BS)vdi$(BS)vdi.a -x ..$(BS)bdos$(BS)bdos.a -x ..$(BS)aes$(BS)aes.a -x ..$(BS)desk$(BS)desk.a tosrsc$(COUNTRY).o vars.o

cartridge_ASFLAGS=-n

cartridg.img: cartridg.o
	$(LD) -o cartridg.tmp -a $(CARTBASE) x 0 cartridg.o
	dd bs=1 skip=36 < cartridg.tmp > $@
	$(RM) cartridg.tmp

../bios/bios.ndx: ../bios/bios.a
	$(RANLIB) $<

../vdi/vdi.ndx: ../vdi/vdi.a
	$(RANLIB) $<

../bdos/bdos.ndx: ../bdos/bdos.a
	$(RANLIB) $<

../aes/aes.ndx: ../aes/aes.a
	$(RANLIB) $<

../desk/desk.ndx: ../desk/desk.a
	$(RANLIB) $<

startup.o: startup.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i
	
vdivar.o: ../vdi/vdivar.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i
	
vars.o: ../common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

cartridg.o: cartridg.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i
	
include gluer.mak

check: $(IMG)
	$(CMP) $(CMPL) -x --ignore-initial=0x0:0x0 --offsets=0x$(rombase):0 $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $(IMG)

clean:
	$(RM) *.o *.i $(IMG) tos.img cartridg.tmp cartridg.img $(PROGRAMS) tosrsc*.c glue.* $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak *.img
