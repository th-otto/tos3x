include ../Makefile.common
include ../Makefile.silent
include optimize.mak

all: bios.a bios.o

include ../config.mak
include ../language.mak

include SRCFILES

CPPFLAGS +=

bios.a: tosvars.o $(BIOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ tosvars.o $(BIOS_OBJS)

#
# need to repeat the default suffix rule here,
# because on a FAT filesystem, .S cannot be
# distinguished from .s
#
startup.o: startup.S config.h header.h scrdmp.inc mmu030.inc romcrc.inc dma.inc nvram.inc ikbdclock.inc floppy.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

vars.o: vars.S linea.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

prt.o: prt.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

memtest.o: memtest.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

bios.o: checklang tosvars.o vars.o $(BIOS_OBJS) biosext.o ${MAKEFILE}
	$(LD) -u -s -l -o $@ -a $(bios_tbase) $(bios_dbase) 0 tosvars.o vars.o $(BIOS_OBJS) biosext.o

biosext.o: biosext.S config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

keytbl.o: config.h

prtkeys$(EXEEXT): prtkeys.o
	$(AM_V_LD)${LD} ${LDFLAGS} -o $@ prtkeys.o

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)

distclean: clean
	$(RM) config.h
