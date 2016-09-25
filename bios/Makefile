include ../Makefile.common
include ../Makefile.silent
include optimize.mak

all: bios.a bios.o vars.o

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
startup.o: startup.S header.h scrdmp.inc mmu030.inc romcrc.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

uistart.o: uistart.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P '-DGTSTART=$$'$(gemdos_tbase) '-DGTLEN=$$'$(gemdos_tlen) $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

vars.o: vars.S linea.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

header.h:
	$(AWK) -f ../tools/mkheader.awk $(LANG) > $@

bios.o: checklang tosvars.o vars.o $(BIOS_OBJS) uistart.o ${MAKEFILE}
	$(LD) -u -s -l -o $@ -a $(bios_tbase) $(bios_dbase) 0 tosvars.o vars.o $(BIOS_OBJS) uistart.o

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
