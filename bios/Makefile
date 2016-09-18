include ../Makefile.common
include ../Makefile.silent

all: bios.a

include ../language.mak

include SRCFILES

bios.a: $(BIOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BIOS_OBJS)

startup.o: startup.S header.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

header.h:
	$(AWK) -f ../tools/mkheader.awk $(LANG) > $@

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
