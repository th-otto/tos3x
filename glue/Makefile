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

tos.img: $(STARTUP) $(LIBS) tosimg.tmp tosrsc$(COUNTRY).o vars.o
	$(LD) -o $@ $(SYMBOLS) -a $(rombase) x 0 $(STARTUP) -x ..$(BS)bios$(BS)bios.a -x ..$(BS)vdi$(BS)vdi.a -x ..$(BS)bdos$(BS)bdos.a -x ..$(BS)aes$(BS)aes.a -x ..$(BS)desk$(BS)desk.a tosrsc$(COUNTRY).o vars.o

tosimg.tmp: $(STARTUP) $(LIBS) rscend.o vars.o
	$(LD) -o $@ -s -a $(rombase) x 0 $(STARTUP) -x ..$(BS)bios$(BS)bios.a -x ..$(BS)vdi$(BS)vdi.a -x ..$(BS)bdos$(BS)bdos.a -x ..$(BS)aes$(BS)aes.a -x ..$(BS)desk$(BS)desk.a rscend.o vars.o

# For the PAK/BootJmp option, we need to know the start of the resource area first
rscend.h: tosimg.tmp
	$(BIN)cnm $< > rscend.1; \
	$(GREP) tosrsc rscend.1 > rscend.2; \
	$(SED) -e 's/ .*$$//' -e 's/^00//' rscend.2 > rscend.1; \
	$(TR) abcdef ABCDEF < rscend.1 > rscend.2; \
	echo "16 i E40030" > rscend.1; cat rscend.2 >> rscend.1; echo "- p" >> rscend.1; \
	$(TR) -d '\015' < rscend.1 > rscend.2; \
	$(DC) < rscend.2 > rscend.1; \
	$(SED) -e 's/^/#define RSCGAP /' rscend.1 > $@; \
	$(RM) rscend.1 rscend.2

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
	$(RM) *.o *.i $(IMG) tos.img tosimg.tmp cartridg.tmp cartridg.img $(PROGRAMS) tosrsc*.c glue.* rscend.h $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak *.img
