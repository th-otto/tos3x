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
	
tosrsc$(COUNTRY).o: tosrsc$(COUNTRY).c

tosrsc$(COUNTRY).c: $(MKBININC) glue.$(COUNTRY)
	$(AM_V_GEN)$(MKBININC) glue.$(COUNTRY) tosrsc $@

glue.us: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemus.rsc ../desk/rsc/$(TOSVERSION)/deskus.rsc ../desk/rsc/$(TOSVERSION)/deskus.inf
	$(AM_V_GEN)$(MKGLUE) us $(TOSVERSION)

glue.de: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemde.rsc ../desk/rsc/$(TOSVERSION)/deskde.rsc ../desk/rsc/$(TOSVERSION)/deskde.inf
	$(AM_V_GEN)$(MKGLUE) de $(TOSVERSION)

glue.fr: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemfr.rsc ../desk/rsc/$(TOSVERSION)/deskfr.rsc ../desk/rsc/$(TOSVERSION)/deskfr.inf
	$(AM_V_GEN)$(MKGLUE) fr $(TOSVERSION)

glue.uk: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemuk.rsc ../desk/rsc/$(TOSVERSION)/deskuk.rsc ../desk/rsc/$(TOSVERSION)/deskuk.inf
	$(AM_V_GEN)$(MKGLUE) uk $(TOSVERSION)

glue.es: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemes.rsc ../desk/rsc/$(TOSVERSION)/deskes.rsc ../desk/rsc/$(TOSVERSION)/deskes.inf
	$(AM_V_GEN)$(MKGLUE) es $(TOSVERSION)

glue.it: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemit.rsc ../desk/rsc/$(TOSVERSION)/deskit.rsc ../desk/rsc/$(TOSVERSION)/deskit.inf
	$(AM_V_GEN)$(MKGLUE) it $(TOSVERSION)

glue.se: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemse.rsc ../desk/rsc/$(TOSVERSION)/deskse.rsc ../desk/rsc/$(TOSVERSION)/deskse.inf
	$(AM_V_GEN)$(MKGLUE) se $(TOSVERSION)

glue.sf: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemsf.rsc ../desk/rsc/$(TOSVERSION)/desksf.rsc ../desk/rsc/$(TOSVERSION)/desksf.inf
	$(AM_V_GEN)$(MKGLUE) sf $(TOSVERSION)

glue.sg: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemsg.rsc ../desk/rsc/$(TOSVERSION)/desksg.rsc ../desk/rsc/$(TOSVERSION)/desksg.inf
	$(AM_V_GEN)$(MKGLUE) sg $(TOSVERSION)

glue.tr: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemtr.rsc ../desk/rsc/$(TOSVERSION)/desktr.rsc ../desk/rsc/$(TOSVERSION)/desktr.inf
	$(AM_V_GEN)$(MKGLUE) tr $(TOSVERSION)

glue.fi: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemfi.rsc ../desk/rsc/$(TOSVERSION)/deskfi.rsc ../desk/rsc/$(TOSVERSION)/deskfi.inf
	$(AM_V_GEN)$(MKGLUE) fi $(TOSVERSION)

glue.no: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemno.rsc ../desk/rsc/$(TOSVERSION)/deskno.rsc ../desk/rsc/$(TOSVERSION)/deskno.inf
	$(AM_V_GEN)$(MKGLUE) no $(TOSVERSION)

glue.dk: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemdk.rsc ../desk/rsc/$(TOSVERSION)/deskdk.rsc ../desk/rsc/$(TOSVERSION)/deskdk.inf
	$(AM_V_GEN)$(MKGLUE) dk $(TOSVERSION)

glue.pl: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gempl.rsc ../desk/rsc/$(TOSVERSION)/deskpl.rsc ../desk/rsc/$(TOSVERSION)/deskpl.inf
	$(AM_V_GEN)$(MKGLUE) pl $(TOSVERSION)

glue.gr: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemgr.rsc ../desk/rsc/$(TOSVERSION)/deskgr.rsc ../desk/rsc/$(TOSVERSION)/deskgr.inf
	$(AM_V_GEN)$(MKGLUE) gr $(TOSVERSION)

glue.ru: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemru.rsc ../desk/rsc/$(TOSVERSION)/deskru.rsc ../desk/rsc/$(TOSVERSION)/deskru.inf
	$(AM_V_GEN)$(MKGLUE) ru $(TOSVERSION)

check: $(IMG)
	$(CMP) $(CMPL) -x --ignore-initial=0x0:0x0 --offsets=0x$(rombase):0 $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $(IMG)

clean:
	$(RM) *.o *.i $(IMG) tos.img cartridg.tmp cartridg.img $(PROGRAMS) tosrsc*.c glue.* $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak *.img
