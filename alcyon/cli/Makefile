include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = command.tos newcom.tos

all: $(PROGRAMS)

include SRCFILES
include optimize.mak

command.tos: $(COMA_OBJS) $(LIB1OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ $(COMA_OBJS) $(LIB1OBJS)

newcom.tos: coma.o newcom.o $(LIB1OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LDFLAGS) -h 7 -o $@ coma.o newcom.o $(LIB1OBJS)

lmul.o:	../libsrc/lmul.o
	$(CP) $< $@

lrem.o:	../libsrc/lrem.o
	$(CP) $< $@

ldiv.o:	../libsrc/ldiv.o
	$(CP) $< $@

uldiv.o:	../libsrc/uldiv.o
	$(CP) $< $@

ulrem.o:	../libsrc/ulrem.o
	$(CP) $< $@

bios.o:	../libsrc/bios.o
	$(CP) $< $@

gemdos.o:	../libsrc/gemdos.o
	$(CP) $< $@

check::
	cmp command.tos ${BIN}command.tos

clean:
	$(RM) *.o *.a command.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
